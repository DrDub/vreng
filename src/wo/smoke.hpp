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
#ifndef SMOKE_HPP
#define SMOKE_HPP

#define SMOKE_TYPE 	76
#define SMOKE_NAME 	"Smoke"

#include "vector3.hpp"
#include <vector>
#include "fire.hpp"

#define NEW_SMOKE

#ifdef NEW_SMOKE
#else
#define TILE 1.
#define NTILEZ 8
#define NTILEY 4
#define NTILEX 4
#define WARPF (TILE*5.)
#define AFFF ((TILE/16.)/WARPF)
#define SPRINGF 0.01f
#define DYNF 0.998f
#define MAXLEN (WARPF*4)
#define ANGF 1
#define ANGRND 0.16f
#define ANG0 0.16f
#define SMOKESIZE .25	// 25 cm
#define SMOKELIFE 3.	// 3 sec
#define SMOKEDELTA 0.005
#define SMOKEALPHA 0.35

#define SQRT_5 2.236067977f
#define SQRT_6 2.449489743f
#define D0     0
#define D1     1
#define D2     2
#define D1_1   M_SQRT2
#define D2_1   SQRT_5
#define D2_2   2*M_SQRT2
#define D1_1_1 M_SQRT3
#define D2_1_1 SQRT_6
#define D2_2_1 3
#define D2_2_2 3*M_SQRT2
#define FLOATTOINTCONST (((1.5*65536*256)))

inline int f2int(float f) { f+=FLOATTOINTCONST; return ((*((int*)&f))&0x007fffff)-0x00400000; }
#define rnd2(f) ((float)rand()*(f/(float)RAND_MAX)) // Floating point random number generator (0->1)
#endif


/**
 * Smoke class
 */
class Smoke: public Fire {

public:
#ifdef NEW_SMOKE
  //class Particle {
private:
    Vector3 pos;
    Vector3 vel;
    Vector3 acc;
    float life;
public:
    static const float size;
    static const float _pi;
    static const float angle[10];
    static const float _cos[10];
    static const float _sin[10];
    //Particle();
    //Particle(Vector3 l);
    void run();
    //void update();
    bool isDead();
    float random(float upper, float lower);
  //};
  //std::vector<Particle> particles;
  Smoke(Vector3 l);
  std::vector<Smoke> particles;
  Vector3 emitter;
  //ParticleSystem();
  //ParticleSystem(Vector3 l);
  void addParticle();   
  Vector3 random();
  //void run();
#endif
  static const OClass oclass;	///< class variable.
  virtual const OClass* getOClass() {return &oclass;}

  static void funcs();	///< init funclist.

  Smoke(char *l);	///< Constructor.

  static WObject * (creator)(char *l);
  ///< Creates from fileline.

  virtual void changePermanent(float dt);

  virtual void render();

private:
  virtual void defaults();
  /**< Sets defaults values. */

  virtual void parser(char *l);
  /**< Parses. */

  virtual void behavior();

  virtual void inits();
  /**< Initializations. */

  virtual void draw();

  virtual void draw(float ex, float ey, float dx, float dy, float a);
  /**< Draws smoke particules. */

  virtual void motionAnimate(float dt);
  virtual void motionWarp(Vector3 &p, float dt);

};

#endif
