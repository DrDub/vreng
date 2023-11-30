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
#include "humanhead.hpp"
#include "format.hpp"
#include "user.hpp"	// localuser
#include "model.hpp"	// Model


const OClass Humanhead::oclass(HUMANHEAD_TYPE, "Humanhead", Humanhead::creator);

//local
static Humanhead *phead = NULL;
static uint16_t oid = 0;

struct sHumanhead {
  uint8_t head_id;
  const char head_str[16];
};

static struct sHumanhead heads[] = {
  {Humanhead::MALE, "male"},
  {Humanhead::FEMALE, "female"},
  {Humanhead::NONE, ""},
};


WO * Humanhead::creator(char *l)
{
  return new Humanhead(l);
}

void Humanhead::defaults()
{
  model_e = MALE;
  scale = 1;
  color[0] = 1.; color[1] = .75; color[2] = .7;
}

void Humanhead::parser(char *l)
{
  defaults();
  l = tokenize(l);
  begin_while_parse(l) {
    l = parseAttributes(l);
    if (!l) break;
    if      (! stringcmp(l, "url="))   l = parseUrl(l, names.url);
    else if (! stringcmp(l, "scale=")) l = parseFloat(l, &scale, "scale");
    else if (! stringcmp(l, "color=")) l = parseVector3f(l, color, "color");
    else if (! stringcmp(l, "model=")) {
      char str[16];
      l = parseString(l, str, "model");
      if      (! stringcmp(str, "male"))   { model_e = MALE; }
      else if (! stringcmp(str, "female")) { model_e = FEMALE; }
    }
  }
  end_while_parse(l);
}

/* solid geometry */
void Humanhead::geometry()
{
  char s[128];

  sprintf(s,"solid shape=\"bbox\" dim=\"%f %f %f\" />",.15,.15,.2);
  parseSolid(s);
}

uint8_t Humanhead::getGender(const char *name)
{
  if (name) {
    for (struct sHumanhead *pheads = heads ; pheads; pheads++) {
      if (! strcmp(name, pheads->head_str)) {
        return pheads->head_id;
      }
    }
  }
  return NONE;
}

void Humanhead::behaviors()
{
  enableBehavior(SPECIFIC_RENDER);
  enableBehavior(DYNAMIC);
  enableBehavior(COLLIDE_NEVER);
  //dax enableBehavior(PERMANENT_MOVEMENT);	// follows user

  initMobileObject(0);
}

/* Sets an unique name */
void Humanhead::setName(const char *modelname)
{
  sprintf(names.given, "%s&%s", modelname, localuser->getInstance());
  updateNames();
}

void Humanhead::setName()
{
  setName(typeName());
}

/* special initializations */
void Humanhead::inits()
{
  model_t = Format::getModelByUrl(names.url);
  switch (model_t) {
  case MODEL_LWO: scale = LWO_SCALE; break;
  case MODEL_3DS: scale = _3DS_SCALE; break;
  }

  model = new Model(localuser, names.url, scale);
  dz -= .10;

  // sets position near the avatar
  pos.x = localuser->pos.x + dx;
  pos.y = localuser->pos.y + dy;
  pos.z = localuser->pos.z + dz;
  pos.ax = localuser->pos.ax + dax;
  pos.ay = localuser->pos.ay + day;
  pos.az = localuser->pos.az + daz;
  updatePosition();
}

/* Creation from xml file */
Humanhead::Humanhead(char *l)
{
  dx = dy = dz = 0;
  dax = day = daz = 0;
  visible = true;
  model_t = 0;
  strcpy(modelname, "male");
  defaults();
  parser(l);
  geometry();
  behaviors();
}

/* Regenerated by world via VSql */
Humanhead::Humanhead(User *user, void *d, time_t s, time_t u)
{
  char *str = (char *) d;       // name transmitted
  if (!str) return;

  strcpy(names.given, str);
  strcpy(names.type, typeName());     // need names.type for VSql
  char *p = strchr(str, '&');
  *p = '\0';
  strcpy(modelname, str);

  defaults();
  model_e = getGender(modelname);
  setName(modelname);
  setOwner();
  geometry();
  behaviors();
}

/* Launched by user via prefs */
Humanhead::Humanhead(User *user, const char *url, const float *skin)
{
  if (! url) return;

  dx = dy = dz = 0;
  dax = day = daz = 0;
  visible = true;
  model_t = 0;
  strcpy(names.url, url);
  sprintf(modelname, "head");
  defaults();
  for (int i=0; i<3; i++) color[i] = skin[i];
  setName(modelname);
  setOwner();
  geometry();
  behaviors();
  inits();
  phead = this;		// if commented Fire is allways selected FIXME!
}

void Humanhead::changePermanent(float lasting)
{
  pos.x = localuser->pos.x + dx;
  pos.y = localuser->pos.y + dy;
  switch (model_t) {
  case MODEL_LWO: pos.z = localuser->pos.z + (localuser->height / 2) - .17; break;
  case MODEL_OFF: pos.z = localuser->pos.z - (localuser->height / 2) - .03; break;
  case MODEL_3DS: pos.z = localuser->pos.z + (localuser->height / 2) - .15; break;
  case MODEL_OBJ: pos.z = localuser->pos.z - (localuser->height / 2) - .10; break;
  default: pos.z = localuser->pos.z - (localuser->height / 2) - .10; break;
  }
  switch (model_t) {
  case MODEL_LWO: /* dax = M_PI_2; daz = -M_PI_2;*/ break;
  case MODEL_OFF: daz = M_PI_2; break;
  case MODEL_3DS: dax = M_PI_2; break;
  case MODEL_OBJ: daz = M_PI; break;
  }
  pos.az = localuser->pos.az + daz;
  pos.ax = localuser->pos.ax + dax;
  pos.ay = localuser->pos.ay + day;
  //echo("head: %.1f %.1f %.1f %.1f %.1f %.1f", pos.x,pos.y,pos.z,pos.ax,pos.ay,pos.az);
  updatePosition();
}

void Humanhead::visibility(bool flag)
{
  visible = flag;
}

void Humanhead::render()
{
  if (visible)
    model->render();
    //dax model->render(pos, color);
}

void Humanhead::quit()
{
  oid = 0;
}

/* Creation: this method is invisible: called by the World */
void Humanhead::recreate_cb(User *user, void *d, time_t s, time_t u)
{
  new Humanhead(user, d, s, u);
}

void Humanhead::funcs()
{
  setActionFunc(HUMANHEAD_TYPE, RECREATE, _Action recreate_cb, "");
}
