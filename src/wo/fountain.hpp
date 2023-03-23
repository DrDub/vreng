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
#ifndef FOUNTAIN_HPP
#define FOUNTAIN_HPP

#include "wobject.hpp"
#include "particle.hpp"

#define FOUNTAIN_TYPE	63
#define FOUNTAIN_NAME	"Fountain"


/**
 * Fountain class
 */
class Fountain: public Particle {

public:
  static const OClass oclass;   ///< class variable

  const OClass* getOClass() {return &oclass;}

  Fountain(char *l);	///< Constructor

  static void funcs();	///< init funclist

  static WObject * (creator)(char *l);
  /**< Creates from file line */

  //dax void render();	///< rendering

private:
  void parser(char *l);
  /**< Parses file line. */

  void defaults();
  /**< Sets default values. */

  void makeSolid();
  /**< Make a pseudo solid. */

  static void pause(Fountain *fountain, void* d, time_t s, time_t u);
  /** Action on fountain. */

};

#endif
