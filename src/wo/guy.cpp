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
#include "guy.hpp"
#include "http.hpp"	// httpOpen
#include "draw.hpp"	// Draw
#include "timer.hpp"	// checkRate
#include "user.hpp"	// localuser
#include "cache.hpp"	// openCache
#include "file.hpp"	// closeFile


const OClass Guy::oclass(GUY_TYPE, "Guy", Guy::creator);
uint16_t Guy::RATE = 10;
uint8_t Guy::BODY_PARTS = 9;
uint8_t Guy::OVERSAMPLE = 10;
const char Guy::DEF_URL[] = "/cset/walking.cset";
const float Guy::SKIN_COLOR[] = {.8, .6, .6, 1};
const float Guy::BUST_COLOR[] = {1, .5, .0, 1};
const float Guy::LEGS_COLOR[] = {.1, .3, .9, 1};
const float Guy::FEET_COLOR[] = {.0, .0, .0, 1};


/* Creation from a file */
WObject * Guy::creator(char *l)
{
  return new Guy(l);
}

void Guy::defaults()
{
  animing = walking = 0;
  sex = 0; //male
  showing = false;
  flying = false;
  control = false;
  for (int i=0; i<4; i++) {
    skin_color[i] = SKIN_COLOR[i];
    bust_color[i] = BUST_COLOR[i];
    legs_color[i] = LEGS_COLOR[i];
    feet_color[i] = FEET_COLOR[i];
  }
}

void Guy::parser(char *l)
{
  defaults();
  l = tokenize(l);
  begin_while_parse(l) {
    l = parse()->parseAttributes(l, this);
    if (!l) break;
    if      (!stringcmp(l, "url"))   l = parse()->parseUrl(l, names.url);
    else if (!stringcmp(l, "anim=")) l = parse()->parseUInt8(l, &animing, "anim");
    else if (!stringcmp(l, "walk=")) l = parse()->parseUInt8(l, &walking, "walk");
    else if (!stringcmp(l, "sex="))  l = parse()->parseUInt8(l, &sex, "sex");
    else if (!stringcmp(l, "skin=")) l = parse()->parseVector3f(l, skin_color, "skin");
    else if (!stringcmp(l, "bust=")) l = parse()->parseVector3f(l, bust_color, "bust");
    else if (!stringcmp(l, "legs=")) l = parse()->parseVector3f(l, legs_color, "legs");
    else if (!stringcmp(l, "feet=")) l = parse()->parseVector3f(l, feet_color, "feet");
  }
  end_while_parse(l);
}

void Guy::makeSolid()
{
  char s[256];
  sprintf(s, "solid shape=\"bbox\" dim=\"%.2f %.2f %.2f\" />",
          BUST_W, HIP_R*2, HEAD_R*2+NECK_H+BUST_H+ULEG_H+LLEG_H+FOOT_H);
  parse()->parseSolid(s, SEP, this);
}

void Guy::behavior()
{
  enableBehavior(PERSISTENT);
  enableBehavior(NO_ELEMENTARY_MOVE);
  enableBehavior(COLLIDE_NEVER);
  enableBehavior(SPECIFIC_RENDER);
  setRenderPrior(RENDER_HIGH);

  initializeMobileObject(0);
  enablePermanentMovement();
}

void Guy::inits()
{
  step = CYCLE/2;
  incstep = 1.;
  dlist = -1;

  draw();

  Http::httpOpen(names.url, httpReader, this, 0);

  for (int j=0; j < numjoints; j++)
    computeCurve(j);
}

/* constructor from xml file */
Guy::Guy(char *l)
{
  parser(l);
  control = false;
  behavior();
  inits();
}

/* constructor from user */
Guy::Guy()
{
  defaults();
  //dax makeSolid();
  control = true;
  behavior();

  strcpy(names.url, DEF_URL);
  inits();

  setActionFunc(GUY_TYPE, 1, WO_ACTION NULL, "");  // cancel
  setActionFunc(GUY_TYPE, 2, WO_ACTION NULL, "");  // cancel
}

const char * Guy::getUrl() const
{
  return (const char *) names.url;
}

void Guy::httpReader(void *_guy, Http *http)
{
  int points = 0;
  Guy *guy = (Guy *) _guy;
  if (! guy) return;

  FILE *f = Cache::openCache(guy->getUrl(), http);
  if (! f) { error("can't open %s", guy->getUrl()); return; }

  char *l, line[256];
  fgets(line, sizeof(line), f);	 // numjoints
  line[strlen(line) - 1] = '\0';
  guy->numjoints = atoi(line);
  guy->curve = new tGuyCtrlPts[guy->numjoints];

  fgets(line, sizeof(line), f);	 // skip mirror_flag
  for (int j=0; j < guy->numjoints; j++) {
    fgets(line, sizeof(line), f);
    line[strlen(line) - 1] = '\0';
    points = atoi(line);	// numpoints
    if (points < 4 || points > MAX_CPOINTS) goto abort;
    guy->curve[j].numpoints = points;
    fgets(line, sizeof(line), f);  // coords
    line[strlen(line) - 1] = '\0';
    l = strtok(line, " ");
    for (int p=0; p < points; p++) {
      guy->curve[j].coords[p] = (float) atof(l);
      l = strtok(NULL, " ");
    }
    fgets(line, sizeof(line), f);  // angles
    line[strlen(line) - 1] = '\0';
    l = strtok(line, " ");
    for (int p=0; p < points; p++) {
      guy->curve[j].angles[p] = (float) atof(l);
      l = strtok(NULL, " ");
    }
  }
  File::closeFile(f);
  return;

abort:
  error("wrong while reading CSet file, numjoints=%d, points=%d", guy->numjoints, points);
  guy->setPose();
  File::closeFile(f);
  return;
}

void Guy::computeCurve(int join)
{
  float pointset[3][4];
  float prod[3][4], tm[4], vec[3];
  float tinc = 1./CYCLE/OVERSAMPLE;
  float basis[4][4] = {{-1, 3, -3, 1}, {3, -6, 3, 0}, {-3, 3,  0, 0}, {1,  0, 0, 0}};

  for (int i=0; i<4; i++)  // z's are always zero, only 2D
    pointset[2][i] = 0;

  int newindex, lastindex = -1;
  for (int p=0; p < curve[join].numpoints; p += 3) {
    float t = 0.;
    for (int i=0; i<4; i++) {
      pointset[0][i] = curve[join].coords[p + i];
      pointset[1][i] = curve[join].angles[p + i];
    }
    MulM3M4(pointset, basis, prod);

    while (t <= 1) {
      tm[0] = t*t*t;
      tm[1] = t*t;
      tm[2] = t;
      tm[3] = 1;
      MulM3V4(prod, tm, vec);
      newindex = (int) (vec[0] * (CYCLE-1));
      if (newindex > lastindex) {  // go at least one
        newindex %= CYCLE;  // avoid out of bounds
        cycles[0][join][newindex] = vec[1];
        lastindex++;
      }
      t += tinc;
    }
  }
  for (int c=0; c < CYCLE; c++) {  // copy to other leg, out-o-phase
    cycles[1][join][c] = cycles[0][join][(c+(CYCLE/2))%CYCLE];
  }
}

void Guy::setPose()
{
  for (int j=0; j < numjoints; j++) {
    curve[j].numpoints = 4;
    curve[j].coords[0] = 0.0;
    curve[j].angles[0]  = 0.0;
    curve[j].coords[1] = 0.2;
    curve[j].angles[1]  = 0.0;
    curve[j].coords[2] = 0.8;
    curve[j].angles[2]  = 0.0;
    curve[j].coords[3] = 1.0;
    curve[j].angles[3]  = 0.0;
  }
}

void Guy::draw_bust()
{
  glPushMatrix();
  //ORIG glScalef(BUST_W, BUST_H, ULEG_R);
  glScalef(.8, 1, .5);
  glRotatef(-90, 1, 0, 0);
  Draw::cylinder(HIP_R*2, BUST_W, BUST_H, 16, 16, 0);
  glPopMatrix();
}

void Guy::draw_breath()
{
  glPushMatrix();
  glTranslatef(0, BUST_H - NECK_H*3, BREATH_R);
  glRotatef(180, 0, 1, 0);
  Draw::cylinder(BREATH_R, 0, BREATH_R, 16, 16, 0);
  glPopMatrix();
}

void Guy::draw_neck()
{
  glPushMatrix();
  glTranslatef(0, BUST_H, 0);
  glRotatef(-90, 1, 0, 0);
  Draw::cylinder(NECK_R, NECK_R, NECK_H, 16, 16, 0);
  glPopMatrix();
}

void Guy::draw_head()
{
  glPushMatrix();
  glTranslatef(0, BUST_H + NECK_H + HEAD_R, 0);
  Draw::sphere(HEAD_R, 16, 16, 0);
  glPopMatrix();
}

void Guy::draw_uleg()
{
  // knee
  glPushMatrix();
  glTranslatef(0, -(HIP_R + ULEG_H -KNEE_R), 0); //DAX KNEE
  Draw::sphere(KNEE_R, 16, 16, 0);
  glPopMatrix();

  // hip
  //glPushMatrix(); //DAX
  glTranslatef(0, -HIP_R, 0);
  Draw::sphere(HIP_R, 16, 16, 0);
  //glPopMatrix(); //DAX

  // thig
  glPushMatrix();
  glRotatef(90, 1, 0, 0);
  Draw::cylinder(HIP_R, KNEE_R, ULEG_H, 16, 16, 0);
  glPopMatrix();
}

void Guy::draw_lleg()
{
  // calf
  glPushMatrix();
  glRotatef(90, 1, 0, 0);
  Draw::cylinder(KNEE_R, ANKLE_R, LLEG_H, 16, 16, 0);
  glPopMatrix();
}

void Guy::draw_foot()
{
  glPushMatrix();
  glTranslatef(0, 0, -ANKLE_R/2);
  Draw::sphere(ANKLE_R, 16, 16, 0);//DAX
  glRotatef(90, 1, 0, 0);
  glScalef(.5, 1, .5); //DAX
  Draw::cylinder(ANKLE_R, ANKLE_R, FOOT_H, 16, 16, 0);
  glPopMatrix();
}

void Guy::draw_uarm()
{
  // elbow
  glPushMatrix();
  glTranslatef(0, -(SHOULDER_R + UARM_H), 0);
  Draw::sphere(ELBOW_R, 16, 16, 0);
  glPopMatrix();

  // shoulder
  glTranslatef(0, -SHOULDER_R, 0);
  Draw::sphere(SHOULDER_R, 16, 16, 0);

  // upper arm
  glPushMatrix();
  glRotatef(90, 1, 0, 0);
  Draw::cylinder(SHOULDER_R, ELBOW_R, UARM_H, 16, 16, 0);
  glPopMatrix();
}

void Guy::draw_larm()
{
  glPushMatrix();
  glRotatef(90, 1, 0, 0);
  Draw::cylinder(ELBOW_R, WRIST_R, LARM_H, 16, 16, 0);
  glPopMatrix();
}

void Guy::draw()
{
  if (dlist != -1) glDeleteLists(dlist, BODY_PARTS);
  dlist = glGenLists(BODY_PARTS);
  glNewList(dlist+BUST, GL_COMPILE); draw_bust(); glEndList();
  glNewList(dlist+NECK, GL_COMPILE); draw_neck(); glEndList();
  glNewList(dlist+BREATH, GL_COMPILE); draw_breath(); glEndList();
  glNewList(dlist+HEAD, GL_COMPILE); draw_head(); glEndList();
  glNewList(dlist+ULEG, GL_COMPILE); draw_uleg(); glEndList();
  glNewList(dlist+LLEG, GL_COMPILE); draw_lleg(); glEndList();
  glNewList(dlist+FOOT, GL_COMPILE); draw_foot(); glEndList();
  glNewList(dlist+UARM, GL_COMPILE); draw_uarm(); glEndList();
  glNewList(dlist+LARM, GL_COMPILE); draw_larm(); glEndList();
}

void Guy::display_bust()
{
  glMaterialfv(GL_FRONT, GL_AMBIENT, bust_color);
  glPushMatrix();
  glCallList(dlist+BUST);
  glPopMatrix();
}

void Guy::display_neck()
{
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, skin_color);
  glPushMatrix();
  glCallList(dlist+NECK);
  glPopMatrix();
}

void Guy::display_breath(int side)
{
  if (sex) {
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, bust_color);
    glPushMatrix();
    if (side == 0) glTranslatef(BREATH_R*1.5, 0, 0);
    else glTranslatef(-BREATH_R*1.5, 0, 0);
    glCallList(dlist+BREATH);
    glPopMatrix();
  }
}

void Guy::display_head()
{
  if (control && localuser->hasHead()) return;
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, skin_color);
  glPushMatrix();
  glCallList(dlist+HEAD);
  glPopMatrix();
}

void Guy::display_leg(int side)
{
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, legs_color);
  glPushMatrix();
  if (side == 0) glTranslatef(BUST_L * BUST_W/2., 0, 0);
  else glTranslatef(-BUST_L * BUST_W/2., 0, 0);

  // Upper leg: rotates about the x axis only
  glRotatef(cycles[side][0][step], 1, 0, 0);
  glPushMatrix();
  glCallList(dlist+ULEG);
  glPopMatrix();

  // Lower leg: rotates about the x axis only
  glTranslatef(0, -(ULEG_H + KNEE_R), 0);
  glRotatef(cycles[side][1][step], 1, 0, 0);
  glPushMatrix();
  glCallList(dlist+LLEG);
  glPopMatrix();

  // Foot: rotates about the x axis only
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, feet_color);
  glTranslatef(0, -(ULEG_H + LLEG_H + ANKLE_R)/2, 0); //DAX
  glRotatef(cycles[side][2][step], 1, 0, 0);
  glPushMatrix();
  glCallList(dlist+FOOT);
  glPopMatrix();
  glPopMatrix();
}

void Guy::display_arm(int side)
{
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, bust_color);
  glPushMatrix();
  glTranslatef(0, BUST_H, 0);
  if (side == 0) glTranslatef(BUST_W -SHOULDER_R/2, 0, 0);
  else glTranslatef(-BUST_W + SHOULDER_R/2, 0, 0);

  // Upper arm: rotates about the x axis only
  if (flying)
    glRotatef(135, 1, 0, 0);
  else if (showing && side == 0)  // right arm
    glRotatef(90, 1, 0, 0);
  else
    glRotatef(cycles[side][3][step], 1, 0, 0);
  glPushMatrix();
  glCallList(dlist+UARM);
  glPopMatrix();

  // Lower arm: rotates about the x axis only
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, skin_color);
  glTranslatef(0, -(UARM_H + ELBOW_R), 0);
  if (flying || (showing && side == 0))
    glRotatef(0, 1, 0, 0);
  else
    glRotatef(cycles[side][4][step], 1, 0, 0);
  glPushMatrix();
  glCallList(dlist+LARM);
  glPopMatrix();
  glPopMatrix();
}

void Guy::changePermanent(float lasting)
{
  if (animing) {
    step = (int) fmod((double) step + incstep, CYCLE);
  }
  if (control) {
    if (localuser) {
      pos.x = localuser->pos.x;
      pos.y = localuser->pos.y;
      pos.z = localuser->pos.z;
      pos.az = localuser->pos.az - M_PI_2;
      updatePosition();
    }
  }
}

void Guy::display_body()
{
  display_bust();
  display_neck();
  display_breath(0);
  display_breath(1);
  display_head();
  display_leg(0);
  display_leg(1);
  display_arm(0);
  display_arm(1);
}

void Guy::render()
{
  float dx, dz;
  static       float guy_radian = 3 * M_PI/2;	// radian
  static const float guy_step   = 72;		// number of steps
  static const float guy_radius = 1.5;		// space unit

  //glPushAttrib(GL_ALL_ATTRIB_BITS);
  glPushMatrix();

  glTranslatef(pos.x, pos.y, pos.z);
  glRotatef(RAD2DEG(pos.az), 0, 0, 1);
  glRotatef(RAD2DEG(pos.ax), 1, 0, 0);
  glRotatef(90, 1, 0, 0);

  if (::g.times.checkRate(RATE)) guy_radian -= M_2PI / guy_step;
  if (guy_radian <= 0) guy_radian = M_2PI;
  if (walking == 1) {
    dx =  guy_radius * cos(guy_radian);
    dz = -guy_radius * sin(guy_radian);
    glTranslatef(-dx, 0, -dz);
    glRotatef(RAD2DEG(guy_radian), 0, 1, 0);
  }

  display_body();

  glPopMatrix();
  //glPopAttrib();
}

void Guy::setAniming(bool flag)
{
  animing = flag;
}

void Guy::setFlying(bool flag)
{
  flying = flag;
}

void Guy::setShowing(bool flag)
{
  showing = flag;
}

void Guy::quit()
{
  if (curve) delete[] curve;
  flushMySqlPosition();
}

void Guy::animate_cb(Guy *po, void *d, time_t s, time_t u)
{ po->animing = 1 - po->animing; }

void Guy::walking_cb(Guy *po, void *d, time_t s, time_t u)
{ po->walking = 1 - po->walking; }

void Guy::funcs()
{
  setActionFunc(GUY_TYPE, 0, WO_ACTION animate_cb, "Anim");
  setActionFunc(GUY_TYPE, 1, WO_ACTION walking_cb, "Walk");
  setActionFunc(GUY_TYPE, 2, WO_ACTION moveObject, "Move");
}
