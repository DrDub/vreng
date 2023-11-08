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
#include "vreng.hpp"
#include "step.hpp"
#include "user.hpp"	// localuser
#include "ball.hpp"	// BALL_TYPE
#include "move.hpp"	// gotoFront

using namespace std;

const OClass Step::oclass(STEP_TYPE, "Step", Step::creator);

vector<Step*> Step::stepList;	// steps vector
vector<Step*> Step::escaList;	// escalator steps vector
vector<Step*> Step::travList;	// travelator steps vector

const float Step::JUMP = 0.10;
const float Step::LSPEED = 0.5;	// 1/2 ms

// local
static uint16_t oid = 0;


/* creation from a file */
WO * Step::creator(char *l)
{
  return new Step(l);
}

void Step::defaults()
{
  mobile = false;
  escalator = false;
  travelator = false;
  stair = false;
  spiral = false;
  stuck = false;
  dir = 1;
  height = 0;
  length = 0;
  speed = LSPEED;
}

void Step::parser(char *l)
{
  defaults();
  l = tokenize(l);
  begin_while_parse(l) {
    l = parseAttributes(l);
    if (!l) break;
    if (! stringcmp(l, "mode=")) {
      char modestr[16];
      l = parseString(l, modestr, "mode");
      if      (! stringcmp(modestr, "escalator"))  { escalator = true; mobile = true; }
      else if (! stringcmp(modestr, "travelator")) { travelator = true; mobile = true; }
      else if (! stringcmp(modestr, "stair"))      { stair = true; }
      else if (! stringcmp(modestr, "spiral"))     { stair = true; spiral = true; }
    }
    else if (! stringcmp(l, "dir=")) {
      char modestr[16];
      l = parseString(l, modestr, "dir");
      if      (! stringcmp(modestr, "up"))   dir = 1;	// up
      else if (! stringcmp(modestr, "down")) dir = -1;	// down
    }
    else if (! stringcmp(l, "height")) l = parseFloat(l, &height, "height");
    else if (! stringcmp(l, "length")) l = parseFloat(l, &length, "length");
    else if (! stringcmp(l, "speed"))  l = parseFloat(l, &speed, "speed");
  }
  end_while_parse(l);

  if (travelator) {
    height = 0;
    dir = 0;
  }
}

void Step::build()
{
  float size = 0;
  float sx = 2 * pos.bbs.v[0];  // step width
  float sy = 2 * pos.bbs.v[1];  // step depth
  float sz = 2 * pos.bbs.v[2];  // step height

  // steps are interlaced (sx sy sz)
  if (travelator) {
    nsteps = (int) ceil(length / MIN(sx, sy));
    size = length;
  }
  else {  // escalator stair spiral
    if (height) {
      if (escalator) {
        height += sz;  // add the top step
      }
    }
    else if (length && pos.ax) {
      height = length * tan(pos.ax);
    }
    nsteps = (int) ceil(height / sz);
    //echo("nsteps: %d", nsteps);
    size = height;
  }

  ipos = pos;  // save initial step position for other steps

  for (int n=0; n < nsteps; n++) {
    Pos newpos;
    newpos.az = pos.az;
    newpos.ax = pos.ax;
    newpos.ay = pos.ay;

    if (spiral) {		// spiral
      float deltaspiral = atan(sy / sx);
      newpos.x = pos.x + (sx * (cos(deltaspiral * n) - 1));
      newpos.y = pos.y + (sy * (sin(deltaspiral * n) - 1));
      newpos.z = pos.z + (sz * n);
      newpos.az = pos.az + (deltaspiral * n);
      nextstep = new Step(newpos, ipos, "spiral", geomsolid, mobile, size, speed, dir);
      stepList.push_back(nextstep);
    }
    else {
      if (travelator) {		// travelator
        //travList.push_back(this);
        //FIXME: wrong position
        newpos.x = pos.x - (sx * n);
        newpos.y = pos.y - (sy * n);
        newpos.z = pos.z;
        nextstep = new Step(newpos, ipos, "travelator", geomsolid, mobile, size, speed, dir);
        travList.push_back(nextstep);
      }
      else {			// escalator
        //escaList.push_back(this);
        newpos.x = pos.x + (sx * n);
        newpos.y = pos.y + (sy * n);
        newpos.z = pos.z + dir*(sz * n);
        nextstep = new Step(newpos, ipos, "escalator", geomsolid, mobile, size, speed, dir);
        escaList.push_back(nextstep);
      }
    }
  }

  if (mobile) {
    enablePermanentMovement(speed);
  }
}

void Step::behaviors()
{
  initMobileObject(1);
  createPermanentNetObject(PROPS, ++oid);
}

Step::Step(char *l)
{
  parser(l);
  behaviors();
  if (stair || escalator || travelator || spiral) {
    build();	// build the structure
  }
}

Step::Step(Pos& newpos, Pos& _ipos, const char *name, const char *geom, bool _mobile, float _size, float _speed, int _dir)
{
  pos = newpos;

  char *s = new char[strlen(geom)];
  strcpy(s, geom);
  parseSolid(s);

  mobile = _mobile;
  dir = _dir;
  if (dir == 0) {
    length = _size;  // travelator
  }
  else {
    height = _size;  // stair, escalator
  }
  speed = _speed;
  ipos = _ipos;
  stuck = false;

  initMobileObject(1);
  forceNames(name);

  if (mobile) {    // escalator or travelator
    enablePermanentMovement(speed);
    //dax state = ACTIVE;	// only one step !!!
  }
}

/* creation from Gui addobj */
Step::Step(WO *user, char *geom)
{
  parseSolid(geom);

  behaviors();
  enableBehavior(DYNAMIC);

  /* position in front of localuser */
  pos.x = user->pos.x + 0.7;
  pos.y = user->pos.y;
  pos.z = user->pos.z + 0.5;
  updatePosition();
}

void Step::updateTime(time_t sec, time_t usec, float *lasting)
{
  updateLasting(sec, usec, lasting);
}

void Step::changePermanent(float lasting)
{
  if (! mobile) return;

  // only escalator and travelator
  if (state == INACTIVE) return;	// not running

  float sx = 2 * pos.bbs.v[0];  // step width
  float sy = 2 * pos.bbs.v[1];  // step depth
  float sz = 2 * pos.bbs.v[2];  // step height

  if (dir > 0) { 	// escalator upwards
    pos.x += lasting * move.lspeed.v[0] * sin(pos.az);
    pos.y += lasting * move.lspeed.v[1] * cos(pos.az);
    pos.z += lasting * move.lspeed.v[2];
    if (pos.z > (ipos.z + height - sz)) {	// switch step
      //echo("+ %.1f %s", pos.z,getInstance());
      pos.x = ipos.x - (sin(pos.az) * sx);
      pos.y = ipos.y - (cos(pos.az) * sy);
      pos.z = ipos.z; //orig - sz;
    }
    if (stuck) {	// user follows up this step
      localuser->pos.x = pos.x;
      localuser->pos.y = pos.y;
      localuser->pos.z = pos.z + localuser->height/2;
      if (pos.z >= (ipos.z + height /*- sz*/)) {	// user stops at top
        localuser->pos.x += (sin(pos.az) * sx);
        localuser->pos.y += (cos(pos.az) * sy);
        localuser->pos.z += sz;
        stuck = false;
      }
      localuser->updatePosition();
    }
  }
  else if (dir < 0) {	// escalator downwards
    pos.x -= lasting * move.lspeed.v[0] * sin(pos.az);
    pos.y -= lasting * move.lspeed.v[1] * cos(pos.az);
    pos.z -= lasting * move.lspeed.v[2];
    if (pos.z < (ipos.z - height + sz)) {	// switch step
      pos.x = ipos.x;
      pos.y = ipos.y;
      pos.z = ipos.z;
    }
    if (stuck) {	// user follows down this step
      localuser->pos.x = pos.x;
      localuser->pos.y = pos.y;
      localuser->pos.z = pos.z + localuser->height/2;
      if (pos.z <= (ipos.z - height /*+ sz*/)) {	// user stops at bottom
        localuser->pos.x -= (sin(pos.az) * sx);
        localuser->pos.y -= (cos(pos.az) * sy);
        stuck = false;
      }
      localuser->updatePosition();
    }
  }
  else {		// travelator horizontal
    pos.x -= lasting * move.lspeed.v[0] * sin(pos.az);
    pos.y -= lasting * move.lspeed.v[1] * cos(pos.az);	// FIXME!
    if (pos.x >= (ipos.x + length - sx)) {	// switch step
      pos.x = ipos.x;
      pos.y = ipos.y;
    }
    if (stuck) {	// user follows this step
      localuser->pos.x = pos.x;
      localuser->pos.y = pos.y;
      if (pos.x >= (ipos.x + length - sx)) {	// user stops end
        localuser->pos.x -= (sin(pos.az) * sx);
        localuser->pos.y -= (cos(pos.az) * sy);
        stuck = false;
      }
      localuser->updatePosition();
    }
  }
  updatePositionAndGrid(pos);
}

bool Step::updateToNetwork(const Pos &oldpos)
{
  return updatePosToNetwork(oldpos, PROPXY, PROPZ, PROPAZ, PROPAX, PROPAY);
}

/* object intersects: up */
bool Step::whenIntersect(WO *pcur, WO *pold)
{
  switch (pcur->type) {
    case USER_TYPE:
      if (mobile) {	// escalator | travelator
        if (! stuck) {
          stuck = true;
          return true;
        }
        return false;
      }
      else {		// normal case
        if (pcur->pos.x == pold->pos.x && pcur->pos.y == pold->pos.y) {
          pold->copyPositionAndBB(pcur);
        }
        else {
          if (pos.bbs.v[2] < JUMP/8) pcur->pos.z += pos.bbs.v[2];
          else pcur->pos.z += JUMP;  // up
          pcur->updatePositionAndGrid(pold);
        }
      }
      break;
    case STEP_TYPE:
      return false;	// ignore this collision
      break;
    case BALL_TYPE:
      pold->copyPositionAndBB(pcur);
      break;
    default:
      pold->copyPositionAndBB(pcur);
  }
  return true;
}

bool Step::whenIntersectOut(WO *pcur, WO *pold)
{
  if (pcur->type == USER_TYPE) {
    stuck = false;
    return true;
  }
  return false;
}

void Step::quit()
{
  stepList.clear();
  escaList.clear();
  travList.clear();
  oid = 0;
}

void Step::running()
{
  if (! mobile) {
    for (vector<Step*>::iterator it = stepList.begin(); it != stepList.end(); it++) {
      (*it)->state = ACTIVE;
    }
  }
  else {
    if (dir) {
      for (vector<Step*>::iterator it = escaList.begin(); it != escaList.end(); it++) {
        (*it)->state = ACTIVE;
      }
    }
    else {
      for (vector<Step*>::iterator it = travList.begin(); it != travList.end(); it++) {
        (*it)->state = ACTIVE;
      }
    }
  }
}

void Step::pause()
{
  if (dir) {
    //echo("escalator pause: size=%d", escaList.size());	// escaList is empty!
    for (vector<Step*>::iterator it = escaList.begin(); it != escaList.end(); it++) {
      if ((*it)->state & ACTIVE) {
        (*it)->state = INACTIVE;
      }
      else {
        (*it)->state = ACTIVE;
      }
    }
  }
  else {
    //echo("travelator pause: size=%d", travList.size());
    for (vector<Step*>::iterator it = travList.begin(); it != travList.end(); it++) {
      if ((*it)->state & ACTIVE) {
        (*it)->state = INACTIVE;
      }
      else {
        (*it)->state = ACTIVE;
      }
    }
  }
}

void Step::pause_cb(Step *step, void *d, time_t s, time_t u)
{
  step->pause();
}

void Step::funcs()
{
  setActionFunc(STEP_TYPE, 0, _Action pause_cb, "Pause/Running");
  setActionFunc(STEP_TYPE, 1, _Action gotoFront, "Approach");
}
