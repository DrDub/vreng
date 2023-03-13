//---------------------------------------------------------------------------
// VREng (Virtual Reality Engine)	http://vreng.enst.fr/
//
// Copyright (C) 1997-2008 Philippe Dax
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
#ifndef LWO_HPP
#define LWO_HPP

#include "wobject.hpp"	// Pos

#define LW_MAX_NAME_LEN 500
#define MK_ID(a,b,c,d) ((((uint32_t)(a))<<24)| \
                        (((uint32_t)(b))<<16)| \
                        (((uint32_t)(c))<< 8)| \
                        (((uint32_t)(d))    ))
#define ID_FORM MK_ID('F','O','R','M')
#define ID_LWOB MK_ID('L','W','O','B')
#define ID_PNTS MK_ID('P','N','T','S')
#define ID_SRFS MK_ID('S','R','F','S')
#define ID_SURF MK_ID('S','U','R','F')
#define ID_POLS MK_ID('P','O','L','S')
#define ID_COLR MK_ID('C','O','L','R')


typedef struct sMaterial {
  char name[LW_MAX_NAME_LEN];
  float r,g,b,a;
  struct sMaterial *next;
} tMaterial;

typedef struct sFace {
  int material;		// material of this face
  int index_count;	// number of vertices
  int *index;		// index to vertex
  float *texcoord;	// u,v texture coordinates
  struct sFace *next;
} tFace;


/**
 * Lwo class
 */
class Lwo {

private:
  float *vertex;
  tFace *face;
  tMaterial *material;
  int vertex_count;
  int nbf;
  int nbm;
  GLint dlist;		///< display list.

public:
  Lwo(const char *url);	///< constructor.
  virtual ~Lwo();	///< destructor.

  void displaylist();
  void render();
  void render(const struct Pos &pos);
  float getRadius();
  void setScale(float scale);

private:
  static void reader(void *_lwo, class Http *http);

  void readSrfs(FILE *f, int nbytes);
  void readSurf(FILE *f, int nbytes);
  void readPols(FILE *f, int nbytes);
  void readPnts(FILE *f, int nbytes);
  
};

#endif
