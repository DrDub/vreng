//---------------------------------------------------------------------------
// VREng (Virtual Reality Engine)	http://www.vreng.enst.fr/
//
// Copyright (C) 1997-2021 Philippe Dax
// Telecom-Paris (Ecole Nationale Superieure des Telecommunications)
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
#ifndef USER_HPP
#define USER_HPP

#include "wobject.hpp"
#include "move.hpp"	// MAXKEYS

#define USER_TYPE	1
#define USER_NAME	"User"

#define DEF_URL_FRONT	"/gif/default.gif"	// user front box
#define DEF_URL_BACK	"/gif/default1.gif"	// ball by default

class Bullet;
class Dart;
class Carrier;
class Cart;
class Bubble;

extern class User *localuser;	// global

/**
 * User class
 */
class User: public WO {

 public:
  static const float LASTING;
  static const float DEF_WIDTH;
  static const float DEF_DEPTH;
  static const float DEF_HEIGHT;
  static const float LSPEED;
  static const float ASPEED;
  static const float DELTAZ;
  static const float GOTHROUGH;
  static const float FOVY;
  static const float NEAR;
  static const float FAR;
  static const float DENSITY;
  static const uint8_t MESS_LEN = 255;
  static const uint8_t MENSURATIONLEN = 64;

  class Human *human;		///< Human avatar
  class Guy *guy;		///< Guy avatar
  class Humanoid *humanoid;	///< Humanoid avatar
  class Face *face;		///< Head avatar
  
 protected:
  friend class WO;

  uint32_t ssrc;	///< ssrc id.
  char  *front;		///< url body front.
  char  *back;		///< url body back.
  char  *left;		///< url body left.
  char  *right;		///< url body right.
  char  *vre;		///< url vre.
  char  *host;		///< url host.
  char  *avatar;	///< avatar str.
  char  *uface;		///< url face.
  char  *sex;		///< sex.
  char  *headurl;	///< url head.
  char  *skin;		///< skin color.
  char  *bust;		///< bust color.
  char  *color;		///< color.
  char  *baps;		///< vaps server.

  WO* lastSelected;
  float lastDistance;

  char  mensuration[MENSURATIONLEN];
  char  message[MESS_LEN];	///< message buffer.
  char  request[MESS_LEN];
  int   lastmess;		///< last message number.
  int   lastrequest;

#if 0
  uint8_t weight;
  /* Adding aura (=sphere) of user object */
  float aura_center[3]; // center of sphere
  float aura_radius;    // sphere's radius
#endif

 public:
  float width;		///< body width.
  float depth;		///< body depth.
  float height;		///< body height.
  char  *web;		///< url web.
  char  *cname;		///< RTP cname.
  char  *rtcpname;	///< RTCP name.
  char  *email;		///< user's email.
  char  *tool;		///< tool used.
  V3 ray;		///< ray extremity.
  uint8_t hit;		///< hit.
  Carrier *carrier;	///< Carrier.
  Cart *cart;		///< Cart.
  Bubble *bubble;	///< bubble text.
  uint8_t current_view;	///< current_view.

  /* props */
  enum {
    PROPHNAME,		//00
    PROPMAPFRONT,	//01
    PROPMAPBACK,	//02
    PROPXY,		//03
    PROPZ,		//04
    PROPAZ,		//05
    PROPAY,		//06
    PROPAX,		//07
    PROPMSG,		//08
    PROPMENSURATION,	//09
    PROPMAPLEFT,	//10
    PROPMAPRIGHT,	//11
    PROPVRE,		//12
    PROPWEB,		//13
    PROPSSRC,		//14
    PROPRTCPNAME,	//15
    PROPRTCPEMAIL,	//16
    PROPMODEL,		//17
    PROPFACE,		//18
    PROPRAY,		//19
    PROPSEX,		//20
    PROPHEAD,		//21
    PROPSKIN,		//22
    PROPBUST,		//23
    PROPS
  };
  enum {
    PROPBEGINVAR = PROPXY,
    PROPENDVAR = PROPMSG
  };

  static const OClass oclass;	///< class variable.

  static void funcs();		///< init funclist.

  const OClass* getOClass() { return &oclass; }
  /**< Returns instance of this class. */

  User();
  /**< Constructor builtin. */

  User(uint8_t type_id, Noid noid, Payload *pp);
  /**< Constructor replicator. */

  virtual ~User();
  /**< Destructor. */

  void inits();
  /**< Makes needed initializations. */

  static WO * replicator(uint8_t type_id, Noid noid, Payload *pp);
  /**< Replicates a remote avatar. */

  void resetBubble();
  /**< Resets bubble text. */

  Bubble * getBubble() const;
  /**< Gets bubble text. */

  void changePosition(float lasting);
  /**< Imposed movement. */

  void changePermanent(float lasting);
  /**< Permanent movement. */

  void updateTime(time_t sec, time_t usec, float *last);
  /**< Updates times. */

  bool updateToNetwork(const Pos &oldpos);
  /**< Publishes position to the network. */

  bool whenIntersect(WO *pcur, WO *pold);
  /**< When an other object intersects. */

  void updateTime(float lasting[]);
  /**< Fills delays's array for each user motion direction. */

  void changePosition(const float lasting[]);
  /**< Does the motion in each direction. */

  void userMovement(time_t sec, time_t usec);
  /**< User general motion. */

  void setRayDirection(GLint x, GLint y);
  /**< Lanche a ray in thar direction. */

  void specialAction(int act_id, void *data, time_t sec, time_t usec);
  /**< Special action to do. */

  void enableGravity();
  /**< enables gravity. */

  void disableGravity();
  /**< disables gravity. */

  void resetPosition();
  /**< Resets user position. */

  void setPosition();
  /**< Sets user position. */

  bool hasHead();
  /**< Returns boolean. */

  void setView(uint8_t view);
  /**< User View managment. */

  // static methods

  static void userWriting(const char *usermsg);
  /**< Signals a message sent by an user. */

  static void userRequesting(const char *usermsg);
  /**< Signals a message sent by an user. */

  static void bulletPutHit(class Bullet *po, Payload *pp);
  static void dartPutHit(class Dart *po, Payload *pp);

 private:
  void getMemory();
  /**< Gets needed memory. */

  void defaults();
  /**< Sets default values. */

  void geometry();
  /**< User geometry. */

  void setName();
  /**< Sets user name. */

  void elemUserMovement(const float lastings[]);
  /**< User motion limited by the maxlast. */

  void checkPosition();
  /**< Checks user position. */

  void checkPersist();
  /**< Checks Persist objects. */

  void setCamera();
  /**< Sets user camera. */

  void setRtcp();
  /**< Sets RTCP parameters. */

  void addGui();
  /**< Adds user to GUI. */

  // GUI callbacks
  static void createBullet(User *pu, void *d, time_t s, time_t u);
  static void createDart(User *pu, void *d, time_t s, time_t u);
  static void defaultZoom(User *pu, void *d, time_t s, time_t u);
  static void increaseZoom(User *pu, void *d, time_t s, time_t u);
  static void decreaseZoom(User *pu, void *d, time_t s, time_t u);
  static void setZoom(User *pu, void *d, time_t s, time_t u);
  static void defaultLinearSpeed(User *pu, void *d, time_t s, time_t u);
  static void decreaseLinearSpeed(User *pu, void *d, time_t s, time_t u);
  static void increaseLinearSpeed(User *pu, void *d, time_t s, time_t u);
  static void setLspeed(User *pu, void *d, time_t s, time_t u);
  static void defaultAngularSpeed(User *pu, void *d, time_t s, time_t u);
  static void decreaseAngularSpeed(User *pu, void *d, time_t s, time_t u);
  static void increaseAngularSpeed(User *pu, void *d, time_t s, time_t u);
  static void setAspeed(User *pu, void *d, time_t s, time_t u);
  static void switchView(User *pu, void *d, time_t s, time_t u);
  static void papView(User *pu, void *d, time_t s, time_t u);
  static void firstPersonView(User *pu, void *d, time_t s, time_t u);
  static void thirdPersonView(User *pu, void *d, time_t s, time_t u);
  static void thirdPersonView_RotL(User *pu, void *d, time_t s, time_t u);
  static void thirdPersonView_RotR(User *pu, void *d, time_t s, time_t u);
  static void thirdPersonView_RotU(User *pu, void *d, time_t s, time_t u);
  static void thirdPersonView_RotD(User *pu, void *d, time_t s, time_t u);
  static void thirdPersonView_Near(User *pu, void *d, time_t s, time_t u);
  static void thirdPersonView_Far(User *pu, void *d, time_t s, time_t u);
  static void pause(User *pu, void *d, time_t s, time_t u);
  static void pauseOn(User *pu, void *d, time_t s, time_t u);
  static void pauseOff(User *pu, void *d, time_t s, time_t u);
  static void mapView(User *pu, void *d, time_t s, time_t u);
  static void setRoll(User *pu, void *d, time_t s, time_t u);
 public:
  static void setPitch(User *pu, void *d, time_t s, time_t u);
 private:
  static void increasePitch(User *pu, void *d, time_t s, time_t u);
  static void decreasePitch(User *pu, void *d, time_t s, time_t u);
  static void increaseRoll(User *pu, void *d, time_t s, time_t u);
  static void decreaseRoll(User *pu, void *d, time_t s, time_t u);
  static void flyaway(User *pu, void *d, time_t s, time_t u);
  static void toland(User *pu, void *d, time_t s, time_t u);

  // network change callbacks
  static void get_username(User *pu, Payload *pp);
  static void u_get_xy(WO *po, Payload *pp);
  static void u_get_z(WO *po, Payload *pp);
  static void u_get_ax(WO *po, Payload *pp);
  static void u_get_ay(WO *po, Payload *pp);
  static void u_get_az(WO *po, Payload *pp);
  static void get_msg(User *pu, Payload *pp);
  static void get_infos(User *pu, Payload *pp);
  static void get_mensuration(User *pu, Payload *pp);
  static void get_mapfront(User *pu, Payload *pp);
  static void get_mapback(User *pu, Payload *pp);
  static void get_mapleft(User *pu, Payload *pp);
  static void get_mapright(User *pu, Payload *pp);
  static void get_vre(User *pu, Payload *pp);
  static void get_web(User *pu, Payload *pp);
  static void get_avatar(User *pu, Payload *pp);
  static void get_uface(User *pu, Payload *pp);
  static void get_sex(User *pu, Payload *pp);
  static void get_head(User *pu, Payload *pp);
  static void get_skin(User *pu, Payload *pp);
  static void get_bust(User *pu, Payload *pp);
  static void get_color(User *pu, Payload *pp);
  static void get_ssrc(User *pu, Payload *pp);
  static void get_rtcpname(User *pu, Payload *pp);
  static void get_rtcpemail(User *pu, Payload *pp);
  static void get_ray(User *pu, Payload *pp);
  static void put_username(User *pu, Payload *pp);
  static void put_msg(User *pu, Payload *pp);
  static void put_infos(User *pu, Payload *pp);
  static void put_mensuration(User *pu, Payload *pp);
  static void put_mapfront(User *pu, Payload *pp);
  static void put_mapback(User *pu, Payload *pp);
  static void put_mapleft(User *pu, Payload *pp);
  static void put_mapright(User *pu, Payload *pp);
  static void put_vre(User *pu, Payload *pp);
  static void put_web(User *pu, Payload *pp);
  static void put_avatar(User *pu, Payload *pp);
  static void put_uface(User *pu, Payload *pp);
  static void put_sex(User *pu, Payload *pp);
  static void put_head(User *pu, Payload *pp);
  static void put_skin(User *pu, Payload *pp);
  static void put_bust(User *pu, Payload *pp);
  static void put_color(User *pu, Payload *pp);
  static void put_ssrc(User *pu, Payload *pp);
  static void put_rtcpname(User *pu, Payload *pp);
  static void put_rtcpemail(User *pu, Payload *pp);
  static void put_ray(User *pu, Payload *pp);

//---------------------------------------------------------------------------
public:
  enum UserAction {
    UA_BULLET,
    UA_DART,
    UA_FOVYDEF,
    UA_FOVYLESS,
    UA_FOVYMORE,
    UA_FOVYSET,
    UA_LSPEEDDEF,
    UA_LSPEEDLESS,
    UA_LSPEEDMORE,
    UA_ASPEEDDEF,
    UA_ASPEEDLESS,
    UA_ASPEEDMORE,
    UA_PAUSE,
    UA_PAUSEON,
    UA_PAUSEOFF,
    UA_SETLSPEED,
    UA_SETASPEED,
    UA_SWITCHVIEW,
    UA_FIRSTVIEW,
    UA_THIRDVIEWFAR,
    UA_MAPVIEW,
    UA_TPVIEWROTL,
    UA_TPVIEWROTR,
    UA_TPVIEWROTU,
    UA_TPVIEWROTD,
    UA_TPVIEWROTN,
    UA_TPVIEWROTF,
    UA_SETROLL,
    UA_SETPITCH,
    UA_PITCHMORE,
    UA_PITCHLESS,
    UA_ROLLMORE,
    UA_ROLLLESS,
    UA_FLYAWAY,
    UA_TOLAND,
    UA_NONE
  };

};

#endif
