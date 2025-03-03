//---------------------------------------------------------------------------
// VREng (Virtual Reality Engine)	http://vreng.enst.fr/
//
// Copyright (C) 1997-2009 Philippe Dax
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
#ifndef WORLD_HPP
#define WORLD_HPP

#include "matvec.hpp"   // V3 M4
#include <list>

using namespace std;

//notused #define MAX_WORLDS 256


/**
 * World class
 */
class World {

 private:
  static class World * worldList;

  /* states */
  enum world_state {
    TOLOAD,
    LOADING,
    LOADED,
    SIMULATION,
    STOPPED
  };

 public:
  static const uint8_t GRIDX;
  static const uint8_t GRIDY;
  static const uint8_t GRIDZ;
  static const float   DISTX;
  static const float   DISTY;
  static const float   DISTZ;
  static const uint8_t WORLD_LEN;
  static class OList *gridArray[4][4][2];

  char *url;			///< url of world description.
  char *name;			///< world name.
  char *chan;			///< channel string addr/port/ttl.
  uint8_t state;		///< current state.
  uint16_t num;			///< world number.
  uint32_t group;		///< current group addr.
  uint32_t ssrc;		///< current ssrc.
  bool linked;			///< linked world or not.
  bool persistent;		///< persistent world or not.
  float ground;			///< ground level
  V3 bbmin;			///< bb min of the world.
  V3 bbmax;			///< bb max of the world.
  V3 bbslice;			///< slice units of the grid.
  V3 bbcent;			///< bb center of the world.
  V3 bbsize;			///< bb size of the world.
  uint8_t dimgrid[3];		///< grid dimensions.
  World *prev;			///< prev world.
  World *next;			///< next world.

  class Universe *universe;	///< universe pointer.
  class Bgcolor *bgcolor;	///< background color.
  class User *user;		///< localuser.
  class OList ****grid;		///< matrix grid pointer.
  struct GuiItem *guip;		///< gui handle.
  class Clock *clock;		///< internal clock.
  class Vjc *vjc;		///< vjc.
  class Wind *wind;		///< wind.

  World();
  /**< Constructor. */

  virtual ~World() {};
  /**< Destructor. */

  void compute(time_t sec, time_t usec);
  /**< Processes the world. */

  void quit();
  /**< Quits current world. */

 private:
  void addList();
  /**< Adds world into world list. */

  bool call(World *wprev);
  /**<
   * Switches channels.
   * deleteChannel -> enter() -> join()
   */

  void checkIcons();
  /**< Checks whether Icon objects are locally presents. */

  void checkPersist();
  /**< Checks whether objects are persistents. */

 public:

  enum {
    OLD,
    NEW
  };

  //
  // Accessors
  //

  const char* getName() const;		///< Gets current world name.
  void setName(const char* name);	///< Builds world name from url.

  uint8_t getState() const;		///< Gets the world state.
  void setState(int _state);		///< Sets a world state.

  const char* getChan() const;		///< Current channel string.
  bool setChan(const char *chanstr);	///< Sets the channel name.
  void setChanAndJoin(char *chanstr);	///< Sets and joins the channel.

  const char* getUrl() const;		///< Gets the world url.
  void setUrl(const char* _url);	///< Sets the world url.

  void setGround(float level);		///< Sets the world level.
  float getGround() const;		///< Gets the world level.

  bool isDead() const;			///< Checks if world is dead.

  bool isLinked() const;
  void setLinked();
  struct GuiItem * getGui() const;
  bool isGui() const;
  void resetGui();
  User* localUser() const;
  Bgcolor* backgroundColor() const;
  uint32_t getGroupAdr() const;
  void setGroupAdr(uint32_t _group);
  void setSsrc(uint32_t _ssrc);

  //
  // Grid
  //
private:
  void initGrid();
  /**< Inits the grid by default. */

  void clearGrid();
  /**< Clears the grid. */

  void localGrid();
  /**< Sets local grid for the current world. */

  //
  // World methods
  // static methods
  //
public:
  static World* current();
  /**< Return the current world. */

  static void init(const char *urlvre);
  /**< General World initialization. */

  static World* enter(const char* _url, const char* _chanstr, bool _new_one);
  /**< New World initialization. */

  static World* goPrev();
  /**< Go to the previous world and return it. */

  static World* goNext();
  /**< Go to the next world and return it. */

  static World* swap(World *w);
  /**< Exchanges Worlds in the list. */

  static void worldReader(void *urlvre, class Http *http);
  /**< World reader. */

  static World* worldByUrl(const char *_url);
  /**< Gets world by url. */

  static World* worldByGroup(uint32_t group);
  /**< Gets world by group addr. */

  static void deleteObjects();
  /**< Deletes all objects dropped in the todeletelist. */

  static void initFunc();
  /**< Initializes table of general functions. */

  static void setManagerChanAndJoin(const char *chanstr);
  /**< Sets the manager channel name and joins it. */

  static void clearLists();
  /**< Clears all lists. */

  static void dumpworldList(const char *note);

  //notused static const char * getManagerChan();
  //notused /**< Gets the channel name of the manager. */
};

#endif
