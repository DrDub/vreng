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
#include "vreng.hpp"
#include "drone.hpp"
#include "wings.hpp"


const OClass Drone::oclass(DRONE_TYPE, "Drone", Drone::creator);

const float Drone::DRONE_SCALE = .4;
const float Drone::DRONE_ZONE = 3;	// flying zone
const float Drone::DRONE_DELTA = .01;	// elem motion


/* Creation from a file */
WObject * Drone::creator(char *l)
{
  return new Drone(l);
}

void Drone::defaults()
{
  flying = false;
  model = Wings::HELICOPTER;
  wings = NULL;
  radius = DRONE_ZONE;
  scale = DRONE_SCALE;
}

/* Parser */
void Drone::parser(char *l)
{
  defaults();
  l = tokenize(l);
  begin_while_parse(l) {
    l = parse()->parseAttributes(l, this);
    if (!l) break;
    if (! stringcmp(l, "model=")) {
      l = parse()->parseString(l, modelname, "model");
      if      (! stringcmp(modelname, "helicopter"))  model = Wings::HELICOPTER;
    }
    else if (! stringcmp(l, "radius")) {
      l = parse()->parseFloat(l, &radius, "radius");
    }
    else if (! stringcmp(l, "scale")) {
      l = parse()->parseFloat(l, &scale, "scale");
    }
  }
  end_while_parse(l);
}

/* Behavior */
void Drone::behavior()
{
  enableBehavior(NO_ELEMENTARY_MOVE);
  enableBehavior(COLLIDE_NEVER);
  enableBehavior(SPECIFIC_RENDER);

  initMobileObject(0);
}

/* Specific inits */
void Drone::inits()
{
  posinit = pos;
  wings = new Wings(model, scale);
  pos.x += DRONE_DELTA;
  pos.y += DRONE_DELTA;
  pos.z += DRONE_DELTA;
  updatePosition();
}

/* Constructor */
Drone::Drone(char *l)
{
  parser(l);
  behavior();
  inits();
}

/* Computes something at each loop */
void Drone::changePermanent(float lasting)
{
  static bool expansionx = true;
  static bool expansiony = true;
  static bool expansionz = true;
  static int signx = -1;
  static int signy = 1;
  static int signz = 1;

  // x
  if (expansionx) {
    signx = -1;
    if ( (pos.x < (posinit.x - radius)) || (pos.x > (posinit.x + radius)) ) {
      expansionx = false;
      signx = 1;
    }
  }
  else { // collapsex
    signx = 1;
    if ( (pos.x < (posinit.x - radius)) || (pos.x > (posinit.x + radius)) ) {
      expansionx = true;
      signx = -1;
    }
  }
  pos.x += (signx * DRONE_DELTA);

  // y
  if (expansiony) {
    signy = 1;
    if ( (pos.y < (posinit.y - radius)) || (pos.y > (posinit.y + radius)) ) {
      expansiony = false;
      signy = -1;
    }
  }
  else { // collapsey
    signy = -1;
    if ( (pos.y < (posinit.y - radius)) || (pos.y > (posinit.y + radius)) ) {
      expansiony = true;
      signy = 1;
    }
  }
  pos.y += (signy * DRONE_DELTA);

  // z
  if (expansionz) {
    signz = 1;
    if (pos.z > (posinit.z + radius)) {
      expansionz = false;
      signz = -1;
    }
  }
  else { // collapsez
    signz = -1;
    if (pos.z < posinit.z) {
      expansionz = true;
      signz = 1;
    }
  }
  pos.z += (signz * DRONE_DELTA);

  updatePosition();
}

/* Renders at each loop */
void Drone::render()
{
  // push
  glPushMatrix();
  glEnable(GL_CULL_FACE);
  glTranslatef(pos.x, pos.y, pos.z);
  glRotatef(-90, 1, 0, 0);
  //glRotatef(-90, 0, 1, 0);	// keep commented else bad orient
  glRotatef(-90, 0, 0, 1);
  glScalef(scale, scale, scale);

  // render wings
  wings->render();

  // pop
  glDisable(GL_CULL_FACE);
  glPopMatrix();
}

void Drone::quit()
{
}

void Drone::fly()
{
  enableBehavior(SPECIFIC_RENDER);
  enablePermanentMovement();
  wings->start();
  flying = true;
}

void Drone::pause()
{
  flying = false;
  disablePermanentMovement();
  wings->stop();
}

void Drone::reset()
{
  pause();
  pos = posinit;
}

void Drone::fly_cb(Drone *drone, void *d, time_t s, time_t u)
{
  drone->fly();
}

void Drone::pause_cb(Drone *drone, void *d, time_t s, time_t u)
{
  drone->pause();
}

void Drone::reset_cb(Drone *drone, void *d, time_t s, time_t u)
{
  drone->reset();
}

void Drone::funcs()
{
  setActionFunc(DRONE_TYPE, 0, WO_ACTION fly_cb, "fly");
  setActionFunc(DRONE_TYPE, 1, WO_ACTION pause_cb, "pause");
  setActionFunc(DRONE_TYPE, 2, WO_ACTION reset_cb, "reset");
}
