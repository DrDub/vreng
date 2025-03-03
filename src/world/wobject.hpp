//---------------------------------------------------------------------------
// VREng (Virtual Reality Engine)	http://vreng.enst.fr/
//
// Copyright (C) 1997-2011 Philippe Dax
// Telecom-ParisTech (Ecole Nationale Superieure des Telecommunications)
//
// VREng is a free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public Licence as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// VREng is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//---------------------------------------------------------------------------
#ifndef WOBJECT_HPP
#define WOBJECT_HPP

#include "oclass.hpp"	// OClass
#include "olist.hpp"	// OList
#include "parse.hpp"	// Parse
#include "move.hpp"	// MAXKEYS

#include <list>
#include <vector>

/* special behavior masks */
#define NO_BEHAVIOR		0	// null
#define NO_ELEMENTARY_MOVE	1<<0	// bit 0
#define NO_BBABLE		1<<1	// bit 1
#define UNSELECTABLE		1<<2	// bit 2
#define UNVISIBLE		1<<3	// bit 3
#define SPECIFIC_RENDER		1<<4	// bit 4
#define PERSISTENT		1<<5	// bit 5
#define DYNAMIC			1<<6	// bit 6
#define TAKABLE			1<<7	// bit 7
#define REMOVABLE		1<<8	// bit 8
#define PARTICLE		1<<9	// bit 9
#define LIQUID			1<<10	// bit 10
#define MIX_RENDER		1<<11	// bit 11 double render (common + specific)
#define PERMANENT_MOVEMENT	1<<12	// bit 12
#define TRANSCIENT		1<<13	// bit 13

/* collide behaviors */
#define COLLIDE_EVER		0	// bits 29,30,31
#define COLLIDE_MASK		3<<29	// bits 29,30,31
#define COLLIDE_ONCE		1<<29	// bit 29
#define COLLIDE_NEVER		1<<30	// bit 30
#define COLLIDE_GHOST		1U<<31	// bit 31	// James Addison C++14 compatible

/**
 * objects lists
 */
extern std::vector<WO*> objectList;
extern std::list<WO*> mobileList;
extern std::vector<WO*> stillList;
extern std::vector<WO*> invisList;
extern std::vector<WO*> fluidList;
extern std::vector<WO*> clothList;
extern std::vector<WO*> deleteList;
extern std::vector<WO*> lightList;


/**
 * WOId class
 *
 * identifies a distributed object.
 */
class WOId {
 public:
  uint32_t src_id;	///< IPaddr src
  uint16_t port_id;	///< port
  uint16_t obj_id;	///< object id -- network format
};


#define NAME_HASH_SIZE  5423    
#define NAME_DELETED    "XXXXXXX"

/**
 * hash entry
 */
struct hash_elt {
  WO *po;                  ///< object ptr
  char name[OBJNAME_LEN];       ///< object name
};

/**
 * Names struct
 *
 * container for object's names.
 */
struct Names {
  char type[HNAME_LEN];		///< name of an object class.
  char given[OBJNAME_LEN];	///< given name.
  char *instance;		///< name of an instancied object.
  char *implicit;		///< implicit name.
  char *category;		///< category name.
  char *infos;			///< infos of an instancied object.
  const char *world;		///< name of world where is this object.
  char url[URL_LEN];		///< url.
  char owner[USER_LEN];		///< owner.
};

/**
 * Pos struct
 *
 * spatial position and bounding-box.
 */
struct Pos {
 public:
  float x;		///< x absolute position.
  float y;		///< y absolute position.
  float z;		///< z absolute position.
  float az;		///< angle plan xy axis z.
  float ay;		///< angle plan xz axis y.
  float ax;		///< angle plan yz axis x.
  V3 bbc;		///< bounding-box's center.
  V3 bbs;		///< bounding-box's dimension.
  bool alter;		///< position has changed or not.
  uint8_t st;		///< state.
};

/**
 * Move struct
 *
 * motion parameters of the object.
 */
struct Move {
  float ttl;		///< time to live.
  V3 lspeed;		///< linear speed.
  V3 aspeed;		///< angular speed.
  time_t sec;		///< timestamp.
  time_t usec;		///< timestamp.
  time_t perm_sec;	///< time in sec of last permanent movement.
  time_t perm_usec;	///< time in usec of last permanent movement.
  bool nocol;		///< flag without collision.
  bool manip;		///< flag manipulated.
  struct Move *next;	///< next movement.
};

/**
 * WO class
 *
 * common class for all the objects.
 */
class WO {

public:
  class NetObj *netop;		///< reserved field for network.
  struct GuiItem *guip;		///< reserved field for GUI.
  struct Names names;		///< names.
  struct Pos pos;		///< position in the space.
  struct Move move;		///< movement specific.

  uint8_t  type;		///< object type.
  uint8_t  mode;		///< object mode (STILL | MOBILE | INVISIBLE |...).
  uint16_t num;			///< object sequence number.
  int16_t  state;		///< current state.
  uint32_t behavior;		///< behavior flags.

  float lspeed;         	///< linear speed.
  float aspeed;         	///< angular speed.
  bool visible;			///< flag visible or not.
  bool inlist;			///< true if it is already in an OList.
  bool removed;			///< flag removed or not.
  bool deleted;			///< flag deleted or not.
  char *geomsolid;		///< geometry string of the solid.

  class Solid *solid;		///< solid pointer
  class WOId noid;		///< WO Id.
  list<Solid*> _solidList;	///< list of solids.
  class Flare *flare;		///< flare instance.
  class Carrier *carrier;	///< move via carrier.
  class VSql *vsql;		///< VSql handle.

  /* object's list modes */
  enum object_mode {
    STILL,
    MOBILE,
    EPHEMERAL,
    INVISIBLE,
    FLUID,
    MOBILEINVISIBLE,
    CLOTH,
    END_MODE
  };

  /* object's states */
  enum object_state {
    DELETED = -1,
    NONE = 0,
    INACTIVE = 0,
    ACTIVE,
    PLAYING,
    TAKEN,
    DROPPED,
    MOVED,
    CARRYING,
    IN_CART,
    END_STATE
  };

  /* keys */
  bool   kpressed[MAXKEYS];     ///< is the key pressed ?
  time_t kpstart_s[MAXKEYS];    ///< key press starting time (sec).
  time_t kpstart_u[MAXKEYS];    ///< key press starting time (usec).
  time_t kpdur_s[MAXKEYS];      ///< key press duration from starting time sec.
  time_t kpdur_u[MAXKEYS];      ///< key press duration from starting time usec.

  //////////////////////////////////////////////////////////////////////////////
  //
  // Methods
  //

  WO();
  /**< Constructor. */

  virtual ~WO();
  /**< Destructor. */

  virtual const OClass* getOClass()	{ return NULL; }
  /**< Abstract class. */

  uint8_t typeId()			{ return getOClass()->type_id; }
  const char* typeName()		{ return getOClass()->type_name; }
  WCreator* getCreator()		{ return getOClass()->creator; }
  WReplicator* getReplicator() 		{ return getOClass()->replicator; }

  ////////////////////////////////////////////////////////////////
  //
  // Virtual Methods of Instances of general object handlers
  //
  virtual bool isMoving()				{ return testMoving(); }
  /**< Checks whether object is moving. */

  virtual void changePosition(float lasting)		{}
  /**< Changes the position after a triggered movement. */

  virtual void changePermanent(float lasting)		{}
  /**< Changes the position during a permanent movement. */

  virtual void updateTime(time_t s, time_t us, float *lasting)	{}
  /**< Updates remaining times of the movement. */

  virtual bool publish(const Pos &oldpos)		{ return false; }
  /**< Publishes changes to the network. */

  virtual bool whenIntersect(WO *pcur, WO *pold)	{ return false; }
  /**< Handles an ingoing collision with another object. */

  virtual bool whenIntersectOut(WO *pcur, WO *pold)	{ return false; }
  /**< Handles an outgoing collision with another object. */

  virtual void whenWallsIntersect(WO *pold, V3 *norm)	{}
  /**< Handles collisions with walls. */

  virtual void render()					{}
  /**< Makes special rendering. */

  virtual void lighting()				{}
  /**< Makes special lighting. */

  virtual void click(V3 norm)				{}
  /**< Intercepts a click. */

  virtual void quit()					{}
  /**< Makes all thing when leaving the object. */


  //
  // Accessors to Solid
  //
  Solid* getSolid() const;
  /**< Returns first solid. */

  void addSolid(class Solid* solid);
  /**< Adds to solidList. */

  virtual void delSolids();
  /**< Deletes solids from solidList. */

  //
  // Intersections
  //
public:
  int interAABB(WO *o1, WO *o2);

private:
  int interAABB(V3 center1, V3 size1, V3 center2, V3 size2);	// static?
  /**< Computes intersection between 2 AABB (Axis Aligned Bounding Box). */

  int interAABBVertical(V3 center1, V3 size1, V3 center2, V3 size2);	// static?
  int interAABBHorizontal(V3 center1, V3 size1, V3 center2, V3 size2);	// static?

  //
  // Actions
  //
public:
  void specialAction(int action, void *data, time_t sec, time_t usec);
  /**< Calls methods dedicated to each object.
   * Called by GUI.
   */

  bool runAction(const char *action);
  /**< Runs action everwhere if available. */

  //
  // Set, Get, Is
  //
public:
  void setType(uint8_t type);
  /**< Sets object type. */

  uint16_t getNum();
  /**< Gets a new object number. */

  void getPosition(M4& mpos);
  /**< Gives solid's position. */

  void getDim(V3 &dim);
  void getRelBB(V3 &center, V3 &size);
  //notused void getAbsBB(V3 &center, V3 &size);
  //notused void getCent(V3 &center);
  /**< Gets relative or absolute center and size of solid. */

  uint8_t getFrames();
  /**< Gets number of frames of this solid. */

  uint8_t getFrame();
  /**< Gets index of current frame of this solid. */

  void setFrame(uint8_t _frame);
  /**< Sets current frame. */

  bool isValid() const;
  /**< Checks if valid object type. */

  const char * named() const;
  /**< Gets names.given. */

  const char * getInstance() const;
  /**< Gets names.instance. */

  const char * urlName() const;
  /**< Gets names.url. */

  const char * ownerName() const;
  /**< Gets names.owner. */

  const char * worldName() const;
  /**< Gets name of world. */

  bool givenName() const;
  /**< Checks whether the object is explicitly named. */

  void toDelete();
  /**< Delete this object later. */

  bool removeFromScene();
  /**< Removes an object. */

  void setOwner();
  /**< Initialize local user as owner. */

  void click(GLint x, GLint y);
  /**< Intercepts a click. */

  bool isPermanent() const;
  /**< Checks if permanent object. */

private:
  bool isOwner() const;
  /**< Returns true if owner. */

  void setOwner(const char *_owner);
  /**< Initialize owner. */

  //
  // keys
  //
public:
  void clearKeyTab();
  /**< Clears keys times array. */

  void updateKeys(time_t sec, time_t usec);
  /**< Updates the keydifftime arrays. */

  void changePositionOneDir(const uint8_t move_type, const float last);
  /**< Modifies user position in one direction. */

  //
  // Parse
  //
public:
  char * tokenize(char *l);
  /**< Tokenizes the line <object ... >. */

  Parse * parse();
  /**< Gets current parse. */

  char * parseAttributes(char *l);
  /**< Parses attributes. */

  void parseSolid(char *solid);
  /**< Parses solid. */

  void parseSolids(char *solid);
  /**< Parses solids. */

  char * parseUrl(char *ptok, char *url);
  /**< Returns an url */

  char * parseName(char *ptok, char *name);
  /**< Returns name. */

  char * parseChannel(char *ptok, char *chan);
  /**< Returns chan. */

  char * parseColor(char *ptok, Pos &p);
  /**< Returns a color under r,g,b,a format */

  char * parseInt(char *ptok, int *value, const char *attrkey);
  /**< Returns a int under attr=value format */

  char * parseUInt8(char *ptok, uint8_t *value, const char *attrkey);
  /**< Returns a uint8_t under attr=value format */

  char * parseUInt16(char *ptok, uint16_t *value, const char *attrkey);
  /**< Returns a uint16_t under attr=value format */

  char * parseBool(char *ptok, bool *value, const char *attrkey);
  /**< Returns a bool under attr=value format */

  char * parseFloat(char *ptok, float *value, const char *attrkey);
  /**< Returns a float under attr=value format */
  
  char * parseVector3f(char *ptok, float *vector, const char *attrkey);
  /**< Returns a vector under attr=vx,vy,vz format */
  
  char * parseVector3fv(char *ptok, V3 *vector, const char *attrkey);
  /**< Returns a vector under vx,vy,vz format */

  char * parseString(char *ptok, char *str, const char *attrkey);
  /**< Returns a string under attr=string format */

  char * parseString(char *ptok, char *str);
  /**< Returns a string under attr=string format */

  char * parseQuotedString(char *ptok, char *str, const char *attrkey);
  /**< Returns a quoted string under attr=string format */

  char * parseQuotedString(char *ptok, char *str);
  /**< Returns a quoted string. */

  char * parseCaption(char *ptok, char *str, const char *attrkey);
  /**< Returns a caption string. */

  char * parseRotation(char *ptok, Pos &p);
  /**< Returns a rotation under r,X,Y,Z format */
  char * parseTranslation(char *ptok, Pos &p);
  /**< Returns a translation under tx,ty,tz format */

  char * parseGuide(char *ptok, float path[][5], uint8_t *segs);
  /**< Returns an array of position describing the guide */

  //
  // Network - Noid (Network Object Identifier)
  //
public:
  bool publishPos(const Pos &oldpos, int propxy, int propz, int propaz, int propax, int propay);
  /**< Publishes position changes to the network. */

  uint32_t getSrc() const;
  /**< Gets the SrcId. */

  uint16_t getPort() const;
  /**< Gets the PortId. */

  uint16_t getObj() const;
  /**< Gets the ObjId. */

  void setWOId();
  /**< Sets the WOid.
   * Assigns a unique identifier to each Vreng object
   * whether if be a networked object or not.
   */

  //
  // 3D position
  //
public:
  void updateCamera(Pos &pos);
  /**< Updates camera in the 3D. */

  void getSurfVecs(Pos &pos, V3 *v, V3 *w, V3 *norm);
  /**< Returns two vectors that describe the object's surface
   * and a normal vector to that surface
   */

  void getMaterials(GLfloat *dif, GLfloat *amb, GLfloat *spe, GLfloat *emi, GLint *shi, GLfloat *alpha);

private:
  void update3D(Pos &pos);
  /**< Updates object in the 3D. */

  void updateAll3D(Pos &pos);
  /**< Updates object in the 3D. */

  void updateBB();
  /**< Updates object's Bounding Box. */

  //
  // Grid
  //
public:
  void delFromGrid();
  /**< Deletes an object from the vicinity grid. */

  void updateGrid(const float *bbminnew, const float *bbmaxnew, const float *bbminold, const float *bbmaxold);
  void updateGrid(const Pos &oldpos);
  void updateGrid(const WO *pold);
  /**< Updates an object into the vicinity grid. */

private:
  void insertIntoGrid();
  /**< Adds an object into the vicinity grid. */

  //
  // Lists
  //
public:
  OList * addToList(OList * olist);
  /**< Adds an object pointer into a olist. */

  void    addToListOnce(std::list<WO*> &olist);

  OList * addOListOnce(OList * olist); // confuse
  /**< Adds an object pointer into a list only once time. */

  void    delFromList(std::list<WO*> &olist);
  OList * delOList(OList * olist); // confuse
  /**< Deletes an object pointer from a olist. */

  OList * addListToList(OList * list1, OList * list2);
  /**< Concatenation (test of "ispointed") of list pointers on an object. */

  OList * getVicinity(const WO *pold);
  /**< Returns list of pointers on objects touching cell where is the object. */

  void checkVicinity(WO *pold);
  /**< Checks whether vicinity. */

  static WO * byNum(uint16_t num); // to become virtual !
  /**< Gets an object by its num. */

  //
  // Movements
  //
public:
  void enableImposedMovement();
  /**< Enables movement on an object. */

  void enablePermanentMovement();
  void enablePermanentMovement(float lspeed);
  /**< Enables permanent movement on an object. */

  void disablePermanentMovement();
  /**< Disables movement on an object. */

  void setLinearSpeed(float lspeed);
  /**< set permanent linear speed of an object. */

  void setAngularSpeed(float aspeed);
  /**< set permanent angular speed of an object. */

  void initImposedMovement(float lasting);
  /**< Initializes movement on an object. */

  float diffTime(time_t sec, time_t usec);
  /**< Updates times on an object. */

  void stopImposedMovement();
  /**< Stops a movement on an object. */

  bool testMoving();
  /**< Tests if object is moving. */

  bool updateLasting(time_t sec, time_t usec, float *lasting);
  /**< Updates remaining times of the movement. */

  void imposedMovement(time_t sec, time_t usec);
  /**< Handles an object movement. */

  void permanentMovement(time_t sec, time_t usec);
  /**< Handles a permanent object movement. */

  void setLasting(float maxlast);
  /**< Sets the max lasting time of an object. */

  float getLasting() const;
  /**< Gets the lasting time of an object. */

  void moveUserToObject(float val, float _lttl, float _attl);
  /**< Moves the user to the object. */

  static void moveObject(WO *po, void *d, time_t s, time_t u);
  /**< User moves the object. */

private:
  void elemImposedMovement(float lasting);
  /**< Handles an elementary object movement. */

  void elemPermanentMovement(float lasting);
  /**< Handles an elementary permanent object movement. */

  //
  // Names
  //
public:
  static void initNames();
  /**< inits hash_table of names. */

  void forceNames(const char *name);
  /**< Forces names. */

  void updateNames();
  /**< Updates names. */

  WO *getObjectByName(const char *str);
  /**< Gets an object by its name. */

private:
  void setObjectName(const char *str);
  /**< Sets an object name. */

  void getObjectNameById(uint8_t type, char *name);	// static?
  /**< Gets a name by its id. */

  //
  // Initializations
  //
protected:
  void defaults()	{};
  /**< Sets default values. */

  void behaviors()	{};
  /**< Sets behavior. */

  void geometry()	{};
  /**< Sets solid's geometry. */

  void inits()		{};
  /**< Sets initializations. */

public:
  void initObject(uint8_t mode);
  /**< Initializes object. */

  void initMobileObject(float last);
  void initEphemeralObject(float last);
  /**< Initializes mobile object. */

  void initStillObject();
  /**< Initializes still object. */

  void initFluidObject(float last);
  /**< Initializes fluid object. */

  void initClothObject(float last);
  /**< Initializes cloth object. */

  NetObj * createNetObj(uint8_t props, uint16_t oid);
  /**< Creates local permanent NetObj. */

  NetObj * createVolatile(uint8_t props);
  /**< Creates local volatile NetObj. */

  NetObj * replicate(uint8_t props, class Noid _noid);
  /**< Replicate distant volatile NetObj. */

  void enableBehavior(uint32_t flag);
  /**< Enables behavior. */

  void disableBehavior(uint32_t flag);
  /**< Disables behavior. */

  bool isBehavior(uint32_t flag) const;
  /**< Checks if this behavior is on. */

  bool bbBehavior() const;
  /**< Checks if BBox behavior is on. */

  bool isSeen();
  /**< Checks if the object is in the sight view of the user. */

  void setVisible(bool flag);
  /**< Sets visible or not the 3D object. */

  bool isVisible() const;
  /**< Checks if the object is visible. */

  void setRendered(bool flag);
  /**< Sets rendered or not the 3D object. */

  bool isRendered() const;
  /**< Checks if the object is Rendered. */

  bool isOpaque() const;
  /**< Checks whether the object is opaque. */

  void setFlashy(float *color);
  /**< Sets flashy the 3D object. */

  void setFlashy();
  /**< Sets flashy the 3D object. */

  void resetFlashy();
  /**< Resets flashy the 3D object. */

  void setRay(GLint wx, GLint wy);
  void resetRay();
  /**< Resets Ray on the 3D object. */

  void setReflexive(bool flag);

  bool isSelectable() const;
  /**< Checks whether 3D selectable behavior is on. */

  bool isRemoved() const;
  /**< Returns if object has been removed or not. */

  uint32_t collideBehavior() const;
  /**< Returns collide behavior. */

  //
  // Collisions
  //
public:
  void generalIntersect(WO *pold, OList *vicinityList);
  /**< General intersection of objects. */

  void copyPosAndBB(Pos &newpos);
  /**< Copy object position and Bounding Box. */

  void copyPositionAndBB(WO *o);
  /**< Copy object position and Bounding Box. */

  int projectPositionOnObstacle(Pos &pcur, Pos &pold, Pos &obstacle);
  /**< Projects object position on an obstacle. */

  void computeNormal(WO *mobil, V3 *normal);
  /**< Computes the normal of an object. */

  void computeNormal(Pos &mobil, Pos &stil, V3 *normal);
  /**< Computes the normal of still object. */

  bool projectPosition(WO *pcur, WO *pold);
  /**< Projects object position. */

  void bounceTrajectory(WO *pold, V3 *norm);
  /**< Intersects with wall. */

  void updatePosition();
  /**< Updating 3D. */

  void updatePositionAndGrid(Pos &oldpos);
  /**< Updating 3D and grid position. */

  void updatePositionAndGrid(WO *pold);
  /**< Updating 3D and grid position. */

  void updateDist();
  /**< Updating distance to localuser. */

private:
  bool ingoingNeighbor(WO *pold, WO *neighbor);
  /** Checks ingoing intersection with a neighbor. */

  bool outgoingNeighbor(WO *pold, WO *neighbor);
  /** Checks outgoing intersection with a neighbor. */

  void ingoingWalls(WO *pold);
  /** Checks ingoing intersection with walls. */

  void initPosition();
  /**< Init 3D and grid position. */

  //
  // Properties
  //
public:
  void getProperty(uint8_t prop_id, class Payload *pp);
  /**<
   * Gets the local copy property from the payload (pp->data).
   * Il faut renvoyer la position de lecture de la suite,
   * (utiliser getPayload et sa valeur de retour).
   * Typically called after a Delta reception.
   */

  void putProperty(uint8_t prop_id, class Payload *pp);
  /**<
   * Puts the local copy property in the payload.
   * Il faut ecrire pp->data et renvoyer la longueur ecrite
   * Typically called before a Delta emission.
   */

  void deleteReplica();
  /**< Deletes a replicated object. */

  static void resetObjectsNumber();
  /**< Resets object seq. number. */

  uint32_t getObjectsNumber();
  /**< Gets current object sequence number. */


  //
  // GUI
  //
public:
  struct GuiItem * getGui() const;
  /**< Gets the GUI handle. */

  bool isGui() const;
  /**< Checks the GUI handle. */

  void resetGui();
  /**< Resets the GUI handle. */

  void clearObjectBar();
  /**< Clears the ObjectBar in the GUI. */

  void getObjectNames(char **classname, char **instancename, char **actionnames);
  /**< Gives object's class_name & action names.
   * Called by GUI.
   */

  //
  // VSql
  //
public:
  bool checkPersist();
  /**< Checks if rows exists. */

  void getPersist();
  /**< Selects object. */

  int16_t getPersist(int16_t state);
  /**< Gets state from VSql. */

  void updatePersist();
  /**< Checks if position is managed by VSql and update it. */

  void setPersist();
  /**< inserts object. */

  void delPersist();
  /**< Deletes object. */

  void savePersist();
  /**< Flushes position for VSql. */

  //notused void updatePersist(int16_t state);
  //notused /**< Updates state for VSql. */

protected:
  // GUI and network change callbacks
  static void get_xy(WO *po, class Payload *pp);
  static void get_z(WO *po, class Payload *pp);
  static void get_az(WO *po, class Payload *pp);
  static void get_ay(WO *po, class Payload *pp);
  static void get_ax(WO *po, class Payload *pp);
  static void get_hname(WO *po, class Payload *pp);

  static void put_xy(WO *po, class Payload *pp);
  static void put_z(WO *po, class Payload *pp);
  static void put_az(WO *po, class Payload *pp);
  static void put_ay(WO *po, class Payload *pp);
  static void put_ax(WO *po, class Payload *pp);
  static void put_hname(WO *po, class Payload *pp);
};


#include "objects.hpp"


#endif
