//---------------------------------------------------------------------------
// VREng (Virtual Reality Engine)	http://vreng.enst.fr/
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
#include "movie.hpp"
#include "texture.hpp"	// Texture
#include "cache.hpp"	// download
#include "file.hpp"	// openFile
#include "user.hpp"	// localuser
#include "pref.hpp"	// quality
#include "format.hpp"	// Format
#if HAVE_LIBMPEG
#include <mpeg.h>	// /usr/local/include
#else
#include "mpeg.h"	// local: src/bundle/mpeg_lib
#endif
#include "avi.hpp"	// Avi
#include "theme.hpp"	// theme.playvideo


const OClass Movie::oclass(MOVIE_TYPE, "Movie", Movie::creator);

const float Movie::FPS = 10.;		// max number of frames decoded per rendering
const float Movie::DEF_PROJ = 10;	// distance between projector and screen


WObject * Movie::creator(char *l)
{
  return new Movie(l);
}

void Movie::defaults()
{
  rate = FPS;
  proj = DEF_PROJ;
  anim = false;
}

void Movie::parser(char *l)
{
  defaults();
  l = tokenize(l);
  begin_while_parse(l) {
    l = parseAttributes(l);
    if (!l) break;
    if      (! stringcmp(l, "url"))  l = parseUrl(l, names.url);
    else if (! stringcmp(l, "proj")) l = parseFloat(l, &proj, "proj");
    else if (! stringcmp(l, "rate")) l = parseFloat(l, &rate, "rate");
    else if (! stringcmp(l, "anim")) l = parseBool(l, &anim, "anim");
  }
  end_while_parse(l);
}

Movie::Movie(char *l)
{
  state = INACTIVE;
  texid = -1;
  frame = 0;
  begin = false;
  dlist = -1;
  videobuf = NULL;
  texframe = NULL;
  mpeg = NULL;
  avi = NULL;
  file = NULL;

  parser(l);

  initMobileObject(0);

  vidfmt = Format::getPlayerByUrl(names.url);

  if (anim) {
    play();
  }
}

void Movie::geometry()
{
  char s[128];

  getDim(dim);
  sprintf(s, "solid shape=\"pyramid\" s=\"%f\" h=\"%f\" a=\".3\"", dim.v[0], proj);
  parseSolid(s);
}


void Movie::open_mpeg()
{
  if (mpeg) return;		// an instance is already running

  char *filempeg = new char[MAXHOSTNAMELEN];
  file = new File();

  if (Cache::download(names.url, filempeg) == 0) {	// download Mpeg file
    error("can't download %s", filempeg);
    delete[] filempeg;
    delete file;
    return;
  }
  if ((fp = file->open(filempeg, "r")) == NULL) {
    error("can't open mpeg");
    delete[] filempeg;
    delete file;
    return;
  }

  mpeg = new ImageDesc[1];

  SetMPEGOption(MPEG_DITHER, FULL_COLOR_DITHER); //ORDERED_DITHER);
  if (OpenMPEG(fp, mpeg)) {
    width = mpeg->Width;
    height = mpeg->Height;
    fps = mpeg->PictureRate;
    videobuf = new uint8_t[mpeg->Size];
    //echo("mpeg: w=%d h=%d f=%.3f", width, height, fps);
  }
  else {
    error("can't OpenMPEG");
    delete[] filempeg;
    delete[] mpeg;
    mpeg = NULL;
    file->close();
    delete file;
    return;
  }
}

void Movie::open_avi()
{
  if (avi) return;		// an instance is already running

  avi = new Avi(names.url);	// downloads avi file

  int ret = avi->read_header();
  if (ret) {
    error("avi: no header err=%d", ret);
    delete avi;
    avi = NULL;
    return;
  }
  fp = avi->getFile();
  avi->getInfos(&width, &height, &fps);
  videobuf = new uint8_t[4 * width * height];
  echo("avi: w=%d h=%d f=%.3f", width, height, fps);
}

void Movie::init_tex()
{
  GLint param = (::g.pref.quality3D) ? GL_LINEAR : GL_NEAREST;
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, param);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, param);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  // computes texsiz which must be a power of 2
  int i, power = 0;
  texsiz = MAX(width, height);
  while ((texsiz = texsiz/2) > 0) {
    power++;
  }
  for (i=0, texsiz = 1; i <= power; i++) {
    texsiz *= 2;
  }

  // allocates texture pixmap
  texframe = new GLubyte[3 * texsiz * texsiz];
  frame = 0;
  begin = true;

  // gets texid
  texid = Texture::getIdByObject(this);		// works if texture exists
  if (! texid) {
    texid = Texture::open(names.url);
  }
  //echo("texid=%d (%s)", texid, Texture::getUrlById(texid));
}

void Movie::draw_spot()
{
  V3 elu, elb, eru, erb, slu, slb, sru, srb;
  GLfloat color[4] = { .9, .9, 1, .3 };	// lightblue + alpha

  getDim(dim);
  //echo("dim: %.f %.1f %.1f", dim.v[0], dim.v[1], dim.v[2]);

  // eye left up
  elu.v[0] = localuser->pos.x + 1;
  elu.v[1] = localuser->pos.y + 0.4;	//left
  elu.v[2] = localuser->pos.z * 2 - .6;	//up
  // eye left bot
  elb.v[0] = localuser->pos.x + 1;
  elb.v[1] = localuser->pos.y + 0.4;	//left
  elb.v[2] = localuser->pos.z * 2 - .7;	//bot
  // eye right up
  eru.v[0] = localuser->pos.x + 1;
  eru.v[1] = localuser->pos.y - 0.4;	//right
  eru.v[2] = localuser->pos.z * 2 - .6;	//up
  // eye right bot
  erb.v[0] = localuser->pos.x + 1;
  erb.v[1] = localuser->pos.y - 0.4;	//right
  erb.v[2] = localuser->pos.z * 2 - .7;	//bot
  // screen left up
  slu.v[0] = pos.x;
  slu.v[1] = pos.y - dim.v[1]/2;	//left
  slu.v[2] = pos.z + dim.v[2]/2;	//up
  // screen left bot
  slb.v[0] = pos.x;
  slb.v[1] = pos.y - dim.v[1]/2;	//left
  slb.v[2] = pos.z - dim.v[2]/2;	//bot
  // screen right up
  sru.v[0] = pos.x;
  sru.v[1] = pos.y - dim.v[1]/2;	//right
  sru.v[2] = pos.z + dim.v[2]/2;	//up
  // screen right bot
  srb.v[0] = pos.x;
  srb.v[1] = pos.y - dim.v[1]/2;	//right
  srb.v[2] = pos.z - dim.v[2]/2;	//bot

  dlist = glGenLists(1);
  //glNewList(dlist, GL_COMPILE);
  glBegin(GL_POLYGON);

  // spot
  glColor4fv(color);
  glVertex3f(elu.v[0], elu.v[1], elu.v[2]);	// eye left up
  glColor4fv(color);
  glVertex3f(slu.v[0], slu.v[1], slu.v[2]); 	// scr left up
  //glVertex3f(slb.v[0], slb.v[1], slb.v[2]);	// scr right bot
  //glVertex3f(elb.v[0], elb.v[1], elb.v[2]);	// eye right bot
  glColor4fv(color);
  glVertex3f(eru.v[0], eru.v[1], eru.v[2]);	// eye right up
  glColor4fv(color);
  glVertex3f(sru.v[0], sru.v[1], sru.v[2]);	// scr right up
  //glVertex3f(srb.v[0], srb.v[1], srb.v[2]);	// scr right bot
  //glVertex3f(erb.v[0], erb.v[1], erb.v[2]);	// eye right bot
  glVertex3f(elu.v[0], elu.v[1], elu.v[2]);	// eye left up

  glEnd();
  //glEndList();
}

void Movie::inits()
{
  switch (vidfmt) {
    case PLAYER_MPG:
      open_mpeg();
      break;
    case PLAYER_AVI:
      open_avi();
      break;
    default: return;
  }
  init_tex();
  //draw_spot();
}

void Movie::play_mpeg()
{
  uint8_t r, g, b;

  if (File::littleEndian())
    r = 0, g = 1, b = 2; // RGB
  else
    r = 2, g = 1, b = 0; // BGR

  if (! mpeg) return;
  // get a frame from the mpeg video stream
  if (GetMPEGFrame((char *)videobuf) == false) { // end of mpeg video
    if (state == LOOP) {
      RewindMPEG(fp, mpeg);	// rewind mpeg video
      begin = true;
      return;
    }
    CloseMPEG();
    delete[] mpeg;
    mpeg = NULL;
    state = INACTIVE;
    begin = true;
    dlist = -1;
    return;
  }
  // build pixmap texture
  int wof = (texsiz - width) / 2;
  int hof = (texsiz - height) / 2;
  //echo("f=%d s=%d w=%d h=%d", frame, texsiz, width, height);
  if (mpeg->Colormap) {	// case of Colormap Index
    for (int h=0; h < height; h++) {
      for (int w=0; w < width; w++) {
        int v = videobuf[width * h + w];
        ColormapEntry *color = &mpeg->Colormap[v];
        int t = 3 * (texsiz * (h + hof) + w + wof);	// texframe index
        texframe[t+0] = color->red % 255;	
        texframe[t+1] = color->green % 255;	
        texframe[t+2] = color->blue % 255;
      }
    }
  }
  else {
    for (int h=0; h < height; h++) {
      for (int w=0; w < width; w++) {
        int v = 4 * (width * h + w);		// videobuf index
        int t = 3 * (texsiz * (h + hof) + w + wof);	// texframe index
        texframe[t+0] = videobuf[v+r];
        texframe[t+1] = videobuf[v+g];
        texframe[t+2] = videobuf[v+b];
      }
    }
  }
}

void Movie::play_avi()
{
  int ret, retlen;
  uint8_t r, g, b;

  if (File::littleEndian())
    r = 0, g = 1, b = 2; // RGB
  else
    r = 2, g = 1, b = 0; // BGR

  // get a frame from the avi video stream
  ret = avi->read_data(videobuf, width * height * 4, &retlen);
  //echo("f=%d s=%d l=%d", frame, width*height*4, retlen);
  if (ret == 0) {	// end of avi video
    File::closeFile(fp);
    state = INACTIVE;
    delete avi;
    avi = NULL;
    begin = true;
    dlist = -1;
    return;
  }
  // build pixmap texture : doesn't work !!!
  int wof = (texsiz - width) / 2;
  int hof = (texsiz - height) / 2;
  wof = hof = 0; //dax ??
  //echo("f=%d s=%d w=%d h=%d", frame, texsiz, width, height);
  for (int h=0; h < height; h++) {
    for (int w=0; w < width; w++) {
      int v = 4 * (width * h + w);		// videobuf index
      int t = 3 * (256 * (h+hof) + w + wof);	// texframe index
      //echo("w,h: %d,%d t,v: %d,%d", w,h,t,v);
      texframe[t+0] = videobuf[v+r];
      texframe[t+1] = videobuf[v+g];
      texframe[t+2] = videobuf[v+g];
    }
  }
}

void Movie::bind_frame()
{
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texid);
  switch (vidfmt) {
  case PLAYER_MPG:
    glTexImage2D(GL_TEXTURE_2D, 0, 3, texsiz, texsiz, 0, GL_RGB, GL_UNSIGNED_BYTE, texframe);
    break;
  case PLAYER_AVI:
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 256, 256, GL_RGB, GL_UNSIGNED_BYTE, texframe);
    break;
  }
}

/* Gets stream video frames */
void Movie::changePermanent(float lasting)
{
  if (state == INACTIVE || state == PAUSE) return;

  static struct timeval tstart;

  if (begin) {
    gettimeofday(&tstart, NULL);
      enableBehavior(SPECIFIC_RENDER);
      enableBehavior(MIX_RENDER);
      geometry();
    begin = false;
  }
  uint16_t finter = frame;	// previous frame

  struct timeval tcurr;
  gettimeofday(&tcurr, NULL);
  	
  // current frame
  frame = (uint16_t) floor(((((float) (tcurr.tv_sec - tstart.tv_sec) * 1000.) +
	                     ((float) (tcurr.tv_usec - tstart.tv_usec) / 1000.)) / 1000.
                           ) * (float) rate);

  for (uint16_t fdelta=0; fdelta < (frame-finter); fdelta++) {
    if (fdelta >= (uint16_t) fps) {
      return;	// ignore this frame
    }
    switch (vidfmt) {
    case PLAYER_MPG:
      play_mpeg();
      break;
    case PLAYER_AVI:
      play_avi();
      break;
    }
  }

  bind_frame();

  //glBindTexture(GL_TEXTURE_2D, texid);
  //glPushMatrix();
  //glCallList(dlist);
  //draw_spot();
  //glPopMatrix();
}

void Movie::render()
{
  //echo("spot");
  glPushMatrix();
  glTranslatef(pos.x, pos.y, pos.z);
  //glRotatef(-90, 1, 0, 0);
  glRotatef(90, 0, 1, 0);
  glPopMatrix();
}

/*
 * Actions
 */

void Movie::play()
{
  if (state == INACTIVE) {
    state = PLAYING;
    enablePermanentMovement();	// to get frames
    inits();
  }
}

void Movie::stop()
{
  if (state == INACTIVE) {
    return;	// nothing to stop
  }
  state = INACTIVE;
  switch (vidfmt) {
    case PLAYER_MPG:
      CloseMPEG();
      file->close();
      delete file;
      if (mpeg) delete[] mpeg;
      mpeg = NULL;
      break;
    case PLAYER_AVI:
      if (avi) delete avi;
      avi = NULL;
      break;
  }

  disablePermanentMovement();

  if (videobuf) delete[] videobuf;
  videobuf = NULL;
  if (texframe) delete[] texframe;
  texframe = NULL;
}

/* Pause  / Continue */
void Movie::pause()
{
  if (state == PLAYING || state == LOOP) {
    state = PAUSE;
  }
  else if (state == PAUSE) {
    state = PLAYING;	// leaves pause and continues
  }
}

/* Replay one time */
void Movie::rewind()
{
  if (state != PLAYING && state != LOOP && fp != NULL) {
    if (vidfmt == PLAYER_MPG) {
      RewindMPEG(fp, mpeg);
      frame = 0;
      begin = true;
      state = PLAYING;
    }
  }
}

/* Play continuesly */
void Movie::loop()
{
  if (state == INACTIVE) {
    if (vidfmt == PLAYER_MPG) {
      state = LOOP;
      enablePermanentMovement();	// to get frames
      inits();
    }
  }
}

void Movie::debug()
{
  Texture::listTextures();
}

/* callbacks */

void Movie::play_cb(Movie *movie, void *d, time_t s, time_t u)
{
  movie->play();
}

void Movie::stop_cb(Movie *movie, void *d, time_t s, time_t u)
{
  movie->stop();
}

void Movie::pause_cb(Movie *movie, void *d, time_t s, time_t u)
{
  movie->pause();
}

void Movie::rewind_cb(Movie *movie, void *d, time_t s, time_t u)
{
  movie->rewind();
}

void Movie::loop_cb(Movie *movie, void *d, time_t s, time_t u)
{
  movie->loop();
}

void Movie::debug_cb(Movie *movie, void *d, time_t s, time_t u)
{
  movie->debug();
}

void Movie::funcs()
{
  //setActionFunc(MOVIE_TYPE, 0, _Action play_cb, (const char *) uitem(ulabel(g.theme.Playvideo)));
  setActionFunc(MOVIE_TYPE, 0, _Action play_cb, "Play");
  setActionFunc(MOVIE_TYPE, 1, _Action stop_cb, "Stop");
  setActionFunc(MOVIE_TYPE, 2, _Action pause_cb, "Pause");	// side effect if pending
  setActionFunc(MOVIE_TYPE, 3, _Action loop_cb, "Loop");
  //setActionFunc(MOVIE_TYPE, 4, _Action rewind_cb, "Rewind");	// crash in mpeglib
  setActionFunc(MOVIE_TYPE, 4, _Action debug_cb, "Debug");
}
