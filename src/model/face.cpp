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
// ------------------------------------------------------------ //
// Author   : This file has been written by Yann Renard         //
// Copyright: This file is totaly free and you may distribute   //
//            it to anyone you want, without modifying this     //
//            header. If you use it in a commercial project (?) //
//            or in bigger project (!), I would be glad to know //
//            about it :) Please mail me...                     //
//            be glad to know about it, please mail me          //
//                myself_yr@hotmail.com                         //
// ------------------------------------------------------------ //
#include "vreng.hpp"
#include "face.hpp"
#include "humanoid.hpp"
#include "fap.hpp"
#include "body.hpp"	// body
#include "http.hpp"	// httpOpen
#include "cache.hpp"	// setCachePath, open
#include "bone.hpp"	// V3d


static const char headRoot[]		= "root";
static const char lipsRoot[]		= "lipsRoot";
static const char eyeLeftRoot[]		= "eyeLeftRoot";
static const char eyeRightRoot[]	= "eyeRightRoot";
static const char eyeLeftTopRoot[]	= "eyeLeftTopRoot";
static const char eyeRightTopRoot[]	= "eyeRightTopRoot";
static const char eyeLeftBotRoot[]	= "eyeLeftBotRoot";
static const char eyeRightBotRoot[]	= "eyeRightBotRoot";
static const char browLeftRoot[]	= "sourcilLeftRoot";
static const char browRightRoot[]	= "sourcilRightRoot";
static const char noseRoot[]		= "noseRoot";

static const char eyeLeftTopL[]		= "eyeLeftTopL";
static const char eyeRightTopL[]	= "eyeRightTopL";
static const char eyeLeftTopR[]		= "eyeLeftTopR";
static const char eyeRightTopR[]	= "eyeRightTopR";
static const char eyeLeftBotL[]		= "eyeLeftBotL";
static const char eyeRightBotL[]	= "eyeRightBotL";
static const char eyeLeftBotR[]		= "eyeLeftBotR";
static const char eyeRightBotR[]	= "eyeRightBotR";
static const char browRightL[]		= "sourcilRightL";
static const char browRightR[]		= "sourcilRightR";
static const char noseLeft[]		= "noseLeft";
static const char noseRight[]		= "noseRight";
static const char lipsTopL[]		= "lipsTopL";
static const char lipsTopR[]		= "lipsTopR";
static const char lipsBotL[]		= "lipsBotL";
static const char lipsBotR[]		= "lipsBotR";

const float Face::SCALE = 0.075;	///< 3/400


Face::Face()
{
  mesh = NULL;
  root = NULL;
  moveYes = false;
  moveNo = false;
  moveMouth = false;
  moveSmile = false;
  moveSulk = false;
  moveEyeL = false;
  moveEyeR = false;
  moveNose = false;
  index = false;
  cachefile[0] = '\0';
}

Face::Face(const char *urlindex)
{
  mesh = NULL;
  root = NULL;
  moveYes = false;
  moveNo = false;
  moveMouth = false;
  moveSmile = false;
  moveSulk = false;
  moveEyeL = false;
  moveEyeR = false;
  moveNose = false;
  index = true;
  cachefile[0] = '\0';
  urlList.empty();
  Http::httpOpen(urlindex, facesHttpReader, this, 0);
  currentUrl = rand() % urlList.count();
}

Face::~Face()
{
  if (mesh) delete mesh;
  mesh = NULL;
  if (root) delete root;
  root = NULL;
}

/** Caching file */
void Face::httpReader(void *_url, Http *http)
{
  char *url = (char *) _url;
  if (! http) {
    error("httpReader: unable to open http connection");
    return;
  }
  Cache *cache = new Cache();
  FILE *f = cache->open(url, http);
  //dax Cache::closeCache(f);
}

/** Download list of faces url */
void Face::facesHttpReader(void *_face, Http *http)
{
  Face *face = (Face *) _face;
  if (! http) {
    error("facesHttpReader: unable to open http connection");
    return;
  }

  char line[URL_LEN];

  http->reset();
  while (http->getLine(line)) {
    char *faceurl = strdup(line);
    trace(DBG_MAN, "facesHttpReader: add url=%s", faceurl);
    face->urlList.addElement(faceurl);
    free(faceurl);
  }
}

void Face::change()
{
  if (! index) return;

  currentUrl++;
  currentUrl %= urlList.count();
  char *urlface = urlList.getElemAt(currentUrl);
  trace(DBG_MAN, "change: urlface=%s", urlface);
  if (! isascii(urlface[0])) {
    error("change: BUG! urlface=%02x", urlface[0]);
    return;
  }
  load(urlface);
}

void Face::load(const char *url)
{
  BoneMesh   *newmesh = new BoneMesh();
  BoneVertex *newroot = new BoneVertex();

  if (Cache::setCachePath(url, cachefile) == 0) {
    error("Face: file=%s url=%s", cachefile, url);
    return;
  }
  Http::httpOpen(url, httpReader, (void *)url, 0);

  V3d::readV3Dfile(newmesh, newroot, cachefile);

  bone.registerMesh(newmesh);
  bone.registerSkeleton(newroot);
  bone.generateLinkList();

  if (mesh) delete mesh;
  mesh = newmesh;
  if (root) delete root;
  root = newroot;
}

void Face::render()
{
  if (! mesh) return;
  if (bone.meshToMove && bone.skeleton) {
    bone.animate();
    bone.render();
  }
}

void Face::animHead(float angle, int x, int y, int z)
{
  BoneVertex *bone;

  // 0,1,0 pitch (yes)
  // 1,0,0 yaw   (no)
  // 0,0,1 roll  (maybe)
  trace(DBG_MAN, "animHead: angle=%.2f", angle);
  if ((bone = root->findBone(headRoot)) != NULL)
    bone->setRot(sin(angle/50.0) *10 , x, y, z);
  else
    error("headRoot not found");
}

void Face::animNose(float angle, const char *_side)
{
  BoneVertex *bone;
  float scale = 1 - cos(angle / 16.) / 4.;

  trace(DBG_MAN, "animNose: angle=%.2f scale=%.2f", angle, scale);
  if ((bone = root->findBone(noseRoot)) != NULL) {
    if ((bone = root->findBone(_side)) != NULL) {
      bone->resetPos();
      bone->setScale(scale, 1, 1);
    }
  }
  else
    error("noseRoot not found");
}

void Face::animEyeBall(float angle, const char *_side, int dir)
{
  BoneVertex *bone;
  float scale = 1 - cos(angle / 16.) /* / 2. */;

  trace(DBG_MAN, "animEyeBall: angle=%.2f scale=%.2f dir=%d", angle, scale, dir);
    if ((bone = root->findBone(_side)) != NULL) {
      bone->resetPos();
      bone->setScale(scale, 1, 1);
      if (dir)
        bone->setRot((1-scale) * 20., 0,1,0);	// pitch
      else
        bone->setRot((1-scale) * 20., 1,0,0);	// yaw
      bone->resetPos();
    }
  else
    error("%s not found", _side);
}

void Face::animEyeLid(float angle, const char *root1, const char *lid, const char *left, const char *right)
{
  BoneVertex *bone;
  float scale = (1 - cos(angle / 10.)) /* / 2 */;

  if ((bone = root->findBone(root1)) != NULL) {
    if ((bone = root->findBone(lid)) != NULL) {
      int sign;
      if (!strcmp(lid, eyeLeftTopRoot) || !strcmp(lid, eyeRightTopRoot))
        sign = 1;
      else
        sign = -1;
      trace(DBG_MAN, "animEyeLid: angle=%.2f scale=%.2f rot=%.2f", angle, scale, sign*(1-scale)*20);
      bone->resetPos();
      bone->setScale(1, scale, 1);
      bone->setRot(sign * (1-scale) * 20., 1,0,0);
    }
    else
      error("%s not found", lid);
    if ((bone = root->findBone(left)) != NULL) {
      bone->resetPos();
      bone->setScale(1, scale, 1);
    }
    if ((bone = root->findBone(right)) != NULL) {
      bone->resetPos();
      bone->setScale(1, scale, 1);
    }
  }
  else
    error("%s not found", root1);
}

void Face::animEyeBrow(float angle, const char *_root, const char *_side)
{
  BoneVertex *bone;
  float scale = cos(angle / 5.0);

  trace(DBG_MAN, "animEyeBrow: angle=%.2f scale=%.2f", angle, scale);
  if ((bone = root->findBone(_root)) != NULL) {
    bone->resetPos();
    bone->setTrans(0, scale / 25.0, 0);
  }
  else
    error("%s not found", _root);
  if ((bone = root->findBone(_side)) != NULL) {
    bone->resetPos();
    bone->setRot(scale * 10, 0,0,1);
    bone->setTrans(0, scale / 25.0, 0);
  }
  else
    error("%s not found", _side);
}

void Face::animLip(float angle, const char *_side)
{
  BoneVertex *bone;

  if ((bone = root->findBone(lipsRoot)) != NULL) {
    Vect3D delta(0, cos(angle/ 10.0) / 4., 0);
    float smile = cos(angle / 10.0) * 20;

    trace(DBG_FORCE, "animLip: angle=%.2f smile=%.2f", angle, smile);
    if ((bone = root->findBone(_side)) != NULL) {
      bone->resetPos();
      bone->setTrans(delta);
      //dax bone->setRot(smile, 0,0,1);
      bone->setRot(angle, 0,0,1);
    }
  }
  else
    error("lipsRoot not found");
}

void Face::animate(int fapn, int a)
{
  //error("fap: %d %d", fapn, a);
  switch (fapn) {

  case VISEME:			// 1
    //if (a) echo("VISEME %s", e_not_implemented);
    break;
  case EXPRESSION:		// 2
    //if (a) echo("EXPRESSION %s", e_not_implemented);
    break;
  case OPEN_JAW:		// 3
    //if (a) echo("OPEN_JAW %s", e_not_implemented);
    break;
  case LOWER_T_MIDLIP:		// 4
    //if (a) echo("LOWER_T_MIDLIP %s a=%d", e_not_implemented, a);
    break;
  case RAISE_B_MIDLIP:		// 5
    //if (a) echo("RAISE_B_MIDLIP %s a=%d", e_not_implemented, a);
    break;
  case STRETCH_L_CORNERLIP:	// 6
    //if (a) echo("STRETCH_L_CORNERLIP %s a=%d", e_not_implemented, a);
    break;
  case STRETCH_R_CORNERLIP:	// 7
    //if (a) echo("STRETCH_R_CORNERLIP %s a=%d", e_not_implemented, a);
    break;
  case LOWER_T_LIP_LM:		// 8
    if (a) trace(DBG_MAN, "LOWER_T_LIP_LM a=%d", a);
    animLip(a, lipsTopL);
    break;
  case LOWER_T_LIP_RM:		// 9
    if (a) trace(DBG_MAN, "LOWER_T_LIP_RM a=%d", a);
    animLip(a, lipsTopR);
    break;
  case RAISE_B_LIP_LM:		// 10
    if (a) trace(DBG_MAN, "LOWER_B_LIP_LM a=%d", a);
    animLip(a, lipsBotL);
    break;
  case RAISE_B_LIP_RM:		// 11
    if (a) trace(DBG_MAN, "LOWER_B_LIP_RM a=%d", a);
    animLip(a, lipsBotR);
    break;
  case RAISE_L_CORNERLIP:	// 12
  case RAISE_R_CORNERLIP:	// 13
    //if (a) echo("RAISE_CORNERLIP %s a=%d", e_not_implemented, a);
    break;
  case THRUST_JAW:		// 14
    //if (a) echo("THRUST_JAW %s a=%d", e_not_implemented, a);
    break;
  case SHIFT_JAW:		// 15
    //if (a) echo("SHIFT_JAW %s a=%d", e_not_implemented, a);
    break;
  case PUSH_B_LIP:		// 16
    //if (a) echo("PUSH_B_LIP %s a=%d", e_not_implemented, a);
    break;
  case PUSH_T_LIP:		// 17
    //if (a) echo("PUSH_T_LIP %s a=%d", e_not_implemented, a);
    break;
  case DEPRESS_CHIN:		// 18 menton
    //if (a) echo("DEPRESS_CHIN %s a=%d", e_not_implemented, a);
    break;
  case CLOSE_T_L_EYELID:	// 19
    animEyeLid(a, eyeLeftRoot, eyeLeftTopRoot, eyeLeftTopL, eyeLeftTopR);
    break;
  case CLOSE_T_R_EYELID:	// 20
    animEyeLid(a, eyeRightRoot, eyeRightTopRoot, eyeRightTopL, eyeRightTopR);
    break;
  case CLOSE_B_L_EYELID:	// 21
    animEyeLid(a, eyeLeftRoot, eyeLeftBotRoot, eyeLeftBotL, eyeLeftBotR);
    break;
  case CLOSE_B_R_EYELID:	// 22
    animEyeLid(a, eyeRightRoot, eyeRightBotRoot, eyeRightBotL, eyeRightBotR);
    break;
  case YAW_L_EYEBALL:		// 23
    //if (a) echo("YAW_L_EYEBALL %s a=%d", e_not_implemented, a);
    //pd animEyeBall(a, eyeLeftRoot, 0);
    break;
  case YAW_R_EYEBALL:		// 24
    //if (a) echo("YAW_R_EYEBALL %s a=%d", e_not_implemented, a);
    //pd animEyeBall(a, eyeRightRoot, 0);
    break;
  case PITCH_L_EYEBALL:		// 25
    //if (a) echo("PITCH_L_EYEBALL %s a=%d", e_not_implemented, a);
    //pd animEyeBall(a, eyeLeftRoot, 1);
    break;
  case PITCH_R_EYEBALL:		// 26
    //if (a) echo("PITCH_R_EYEBALL %s a=%d", e_not_implemented, a);
    //pd animEyeBall(a, eyeRightRoot, 1);
    break;
  case THRUST_L_EYEBALL:	// 27
  case THRUST_R_EYEBALL:	// 28
    //if (a) echo("THRUST_EYEBALL %s a=%d", e_not_implemented, a);
    break;
  case DILATE_L_PUPIL:		// 29
  case DILATE_R_PUPIL:		// 30
    //if (a) echo("DILATE_PUPIL %s a=%d", e_not_implemented, a);
    break;
  case RAISE_L_I_EYEBROW:	// 31
    if (a) trace(DBG_MAN, "RAISE_L_I_EYEBROW a=%d", a);
    animEyeBrow(a, browLeftRoot, browRightL);
    break;
  case RAISE_R_I_EYEBROW:	// 32
    if (a) trace(DBG_MAN, "RAISE_R_I_EYEBROW a=%d", a);
    animEyeBrow(a, browRightRoot, browRightR);
    break;
  case RAISE_L_M_EYEBROW:	// 33
    if (a) trace(DBG_MAN, "RAISE_L_M_EYEBROW a=%d", a);
    animEyeBrow(a, browLeftRoot, browRightL);
    break;
  case RAISE_R_M_EYEBROW:	// 34
    if (a) trace(DBG_MAN, "RAISE_R_M_EYEBROW a=%d", a);
    animEyeBrow(a, browRightRoot, browRightR);
    break;
  case RAISE_L_O_EYEBROW:	// 35
    if (a) trace(DBG_MAN, "RAISE_L_O_EYEBROW a=%d", a);
    animEyeBrow(a, browLeftRoot, browRightL);
    break;
  case RAISE_R_O_EYEBROW:	// 36
    if (a) trace(DBG_MAN, "RAISE_R_O_EYEBROW a=%d", a);
    animEyeBrow(a, browRightRoot, browRightR);
    break;
  case SQUEEZE_L_EYEBROW:	// 37
    //if (a) echo("SQUEEZE_L_EYEBROW %s a=%d", e_not_implemented, a);
    break;
  case SQUEEZE_R_EYEBROW:	// 38
    //if (a) echo("SQUEEZE_R_EYEBROW %s a=%d", e_not_implemented, a);
    break;
  case PUFF_L_CHEEK:		// 39
  case PUFF_R_CHEEK:		// 40
    //if (a) echo("PUFF_CHEEK %s", e_not_implemented);
    break;
  case LIFT_L_CHEEK:		// 41
  case LIFT_R_CHEEK:		// 42
    //if (a) echo("LIFT_CHEEK %s", e_not_implemented);
    break;
  case SHIFT_TONGUE_TIP:	// 43
  case RAISE_TONGUE_TIP:	// 44
  case THRUST_TONGUE_TIP:	// 45
  case RAISE_TONGUE:		// 46
  case TONGUE_ROLL:		// 47
    //if (a) echo("TONGUE %s", e_not_implemented);
    break;
  case HEAD_PITCH:		// 48
    if (a) trace(DBG_MAN, "HEAD_PITCH a=%d", a);
    animHead(a, 0, 1, 0);	// yes
    break;
  case HEAD_YAW:		// 49
    if (a) trace(DBG_MAN, "HEAD_YAW a=%d", a);
    animHead(a, 1, 0, 0);	// no
    break;
  case HEAD_ROLL:		// 50
    if (a) trace(DBG_MAN, "HEAD_ROLL a=%d", a);
    animHead(a, 0, 0, 1);	// maybe
    break;
  case LOWER_T_MIDLIP_O:	// 51
    //if (a) echo("LOWER_T_MIDLIP_O %s a=%d", e_not_implemented, a);
    break;
  case RAISE_B_MIDLIP_O:	// 52
    //if (a) echo("RAISE_B_MIDLIP_O %s a=%d", e_not_implemented, a);
    break;
  case STRETCH_L_CORNERLIP_O:	// 53
    //if (a) echo("STRETCH_L_CORNERLIP_O %s a=%d", e_not_implemented, a);
    break;
  case STRETCH_R_CORNERLIP_O:	// 54
    //if (a) echo("STRETCH_R_CORNERLIP_O %s a=%d", e_not_implemented, a);
    break;
  case LOWER_T_LIP_LM_O:	// 55
    if (a) trace(DBG_MAN, "LOWER_T_LIP_LM_O a=%d", a);
    animLip(a, lipsTopL);
    break;
  case LOWER_T_LIP_RM_O:	// 56
    if (a) trace(DBG_MAN, "LOWER_T_LIP_RM_O a=%d", a);
    animLip(a, lipsTopR);
    break;
  case RAISE_B_LIP_LM_O:	// 57
    if (a) trace(DBG_MAN, "RAISE_B_LIP_LM_O a=%d", a);
    animLip(a, lipsBotL);
    break;
  case RAISE_B_LIP_RM_O:	// 58
    if (a) trace(DBG_MAN, "RAISE_B_LIP_RM_O a=%d", a);
    animLip(a, lipsBotR);
    break;
  case RAISE_L_CORNERLIP_O:	// 59
    //if (a) echo("RAISE_L_CORNERLIP_O %s a=%d", e_not_implemented, a);
    break;
  case RAISE_R_CORNERLIP_O:	// 60
    //if (a) echo("RAISE_R_CORNERLIP_O %s a=%d", e_not_implemented, a);
    break;
  case STRETCH_L_NOSE:		// 61
    if (a) trace(DBG_MAN, "STRETCH_L_NOSE a=%d", a);
    animNose(a, noseLeft);
    break;
  case STRETCH_R_NOSE:		// 62
    if (a) trace(DBG_MAN, "STRETCH_R_NOSE a=%d", a);
    animNose(a, noseRight);
    break;
  case RAISE_NOSE:		// 63
    //if (a) echo("RAISE_NOSE %s", e_not_implemented);
    break;
  case BEND_NOSE:		// 64
    //if (a) echo("BEND_NOSE %s", e_not_implemented);
    break;
  case RAISE_L_EAR:		// 65
  case RAISE_R_EAR:		// 66
  case PULL_L_EAR:		// 67
  case PULL_R_EAR:		// 68
    //if (a) echo("EAR %s", e_not_implemented);
    break;
  default:
    error("bad fap number=%d", fapn);
  }
}

void Face::animate()
{
  static float angle = 0;
  angle += 5.;

  BoneVertex *bone;
  // --- LIPS MANAGEMENT ---
  // == smile then sulk
  if ( moveMouth ) {
    if ((bone = root->findBone(lipsRoot)) != NULL) {
      Vect3D smileDelta(0, cos(angle/10.0) / 4.0, 0);
      float smile = 20 * cos(angle / 10.0);
      if ((bone = root->findBone(lipsTopL)) != NULL) {
        bone->resetPos();
        bone->setTrans(smileDelta);
        bone->setRot(smile, 0,0,1);
      }
      if ((bone = root->findBone(lipsTopR)) != NULL) {
        bone->resetPos();
        bone->setTrans(smileDelta);
        bone->setRot(-smile, 0,0,1);
      }
      if ((bone = root->findBone(lipsBotL)) != NULL) {
        bone->resetPos();
        bone->setTrans(smileDelta);
        bone->setRot(smile, 0,0,1);
      }
      if ((bone = root->findBone(lipsBotR)) != NULL) {
        bone->resetPos();
        bone->setTrans(smileDelta);
        bone->setRot(-smile, 0,0,1);
      }
    }
  }
  // == smile
  if ( moveSmile ) {
    if ((bone = root->findBone(lipsRoot)) != NULL) {
      Vect3D smileDelta(0, cos(angle/10.0) / 4.0, 0);
      float smile = 20 * cos(angle / 10.0);
      if ((bone = root->findBone(lipsTopL)) != NULL) {
        bone->resetPos();
        bone->setTrans(smileDelta);
        bone->setRot(smile, 0,0,1);
      }
      if ((bone = root->findBone(lipsTopR)) != NULL) {
        bone->resetPos();
        bone->setTrans(smileDelta);
        bone->setRot(-smile, 0,0,1);
      }
    }
  }
  // == sulk
  if ( moveSulk ) {
    if ((bone = root->findBone(lipsRoot)) != NULL) {
      Vect3D smileDelta(0, cos(angle/10.0) / 4.0, 0);
      float smile = 20 * cos(angle / 10.0);
      if ((bone = root->findBone(lipsBotL)) != NULL) {
        bone->resetPos();
        bone->setTrans(smileDelta);
        bone->setRot(smile, 0,0,1);
      }
      if ((bone = root->findBone(lipsBotR)) != NULL) {
        bone->resetPos();
        bone->setTrans(smileDelta);
        bone->setRot(-smile, 0,0,1);
      }
    }
  }

  // --- LEFT EYE MANAGEMENT ---
  // == eye glance
  if ( moveEyeL ) {
    if ((bone = root->findBone(eyeLeftRoot)) != NULL) {
      float eyeLeftScale = (1 - cos(angle / 20.0)) / 2.0;
      if ((bone = root->findBone(eyeLeftBotRoot)) != NULL) {
        bone->resetPos();
        bone->setScale(1, eyeLeftScale, 1);
        bone->setRot(-(1-eyeLeftScale)*20.0, 1,0,0);
      }
      if ((bone = root->findBone(eyeLeftTopRoot)) != NULL) {
        bone->resetPos();
        bone->setScale(1, eyeLeftScale, 1);
        bone->setRot((1-eyeLeftScale)*20.0, 1,0,0);
      }
      if ((bone = root->findBone(eyeLeftTopL)) != NULL) {
        bone->resetPos();
        bone->setScale(1, eyeLeftScale, 1);
      }
      if ((bone = root->findBone(eyeLeftTopR)) != NULL) {
        bone->resetPos();
        bone->setScale(1, eyeLeftScale, 1);
      }
      if ((bone = root->findBone(eyeLeftBotL)) != NULL) {
        bone->resetPos();
        bone->setScale(1, eyeLeftScale, 1);
      }
      if ((bone = root->findBone(eyeLeftBotR)) != NULL) {
        bone->resetPos();
        bone->setScale(1, eyeLeftScale, 1);
      }
    }
  }

  // --- RIGHT EYE MANAGEMENT ---
  // eye move
  if ( moveEyeR ) {
    if ((bone = root->findBone(eyeRightRoot)) != NULL) {
      float eyeRightScale = (1 + cos(angle / 5.0)) / 20.0;
      if ((bone = root->findBone(browRightRoot)) != NULL) {
        bone->resetPos();
        bone->setTrans(0, eyeRightScale / 2.0, 0);
      }
      if ((bone = root->findBone(eyeRightBotRoot)) != NULL) {
        bone->resetPos();
        bone->setTrans(0, eyeRightScale / 2.0, 0);
      }
      if ((bone = root->findBone(eyeRightTopRoot)) != NULL) {
        bone->resetPos();
        bone->setTrans(0, eyeRightScale, 0);
      }
      if ((bone = root->findBone(eyeRightTopL)) != NULL) {
        bone->resetPos();
        bone->setTrans(0, eyeRightScale, 0);
      }
      if ((bone = root->findBone(eyeRightTopR)) != NULL) {
        bone->resetPos();
        bone->setTrans(0, eyeRightScale, 0);
      }
      if ((bone = root->findBone(eyeRightBotL)) != NULL) {
        bone->resetPos();
        bone->setTrans(0, eyeRightScale / 2.0, 0);
      }
      if ((bone = root->findBone(eyeRightBotR)) != NULL) {
        bone->resetPos();
        bone->setTrans(0, eyeRightScale / 2.0, 0);
      }
    }
  }

  // --- NOSE MANAGEMENT ---
  // == resserement narines
  if ( moveNose ) {
    if ((bone = root->findBone(noseRoot)) != NULL) {
      float noseScale = 1 - cos(angle / 16.0) / 4.0;
      if ((bone = root->findBone(noseLeft)) != NULL) {
        bone->resetPos();
        bone->setScale(noseScale, 1, 1);
      }
      if ((bone = root->findBone(noseRight)) != NULL) {
        bone->resetPos();
        bone->setScale(noseScale, 1, 1);
      }
    }
  }

  // --- ROOT MANAGEMENT ---
  if ( moveYes ) {
    if ((bone = root->findBone(headRoot)) != NULL)
      bone->setRot(10 * sin(angle/50.0), 1, 0, 0);
  }
  if ( moveNo ) {
    if ((bone = root->findBone(headRoot)) != NULL)
      bone->setRot(10 * sin(angle/50.0), 0, 1, 0);
  }

#undef BROW_MOTION
#ifdef BROW_MOTION
  float browRightScale = cos(angle / 5.0);
  if ((bone = root->findBone(browRightRoot)) != NULL) {
    bone->resetPos();
    bone->setTrans(0, browRightScale / 25.0, 0);
  }
  if ((bone = root->findBone(browRightL)) != NULL) {
    bone->resetPos();
    bone->setRot(10 * browRightScale, 0,0,1);
    bone->setTrans(0, browRightScale / 25.0, 0);
  }
#endif
}

void Face::changeMoveYes(Humanoid *o, void *d, time_t s, time_t u)
{
  if (o->body->face)
    o->body->face->moveYes = ! o->body->face->moveYes;
}

void Face::changeMoveNo(Humanoid *o, void *d, time_t s, time_t u)
{
  if (o->body->face)
    o->body->face->moveNo = ! o->body->face->moveNo;
}

void Face::changeMoveMouth(Humanoid *o, void *d, time_t s, time_t u)
{
  if (o->body->face)
    o->body->face->moveMouth = ! o->body->face->moveMouth;
}

void Face::changeMoveSmile(Humanoid *o, void *d, time_t s, time_t u)
{
  if (o->body->face)
    o->body->face->moveSmile = ! o->body->face->moveSmile;
}

void Face::changeMoveSulk(Humanoid *o, void *d, time_t s, time_t u)
{
  if (o->body->face)
    o->body->face->moveSulk = ! o->body->face->moveSulk;
}

void Face::changeMoveEyeR(Humanoid *o, void *d, time_t s, time_t u)
{
  if (o->body->face)
    o->body->face->moveEyeR = ! o->body->face->moveEyeR;
}

void Face::changeMoveEyeL(Humanoid *o, void *d, time_t s, time_t u)
{
  if (o->body->face)
    o->body->face->moveEyeL = ! o->body->face->moveEyeL;
}

void Face::changeMoveNose(Humanoid *o, void *d, time_t s, time_t u)
{
  if (o->body->face)
    o->body->face->moveNose = ! o->body->face->moveNose;
}

void Face::changeFace(Humanoid *o, void *d, time_t s, time_t u)
{
  if (o->body->face)
    o->body->face->change();
}
