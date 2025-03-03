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
#include "vreng.hpp"
#include "humanoid.hpp"
#include "world.hpp"	// current
#include "body.hpp"	// Body
#include "v3d.hpp"	// V3d
#include "channel.hpp"	// getGroup
#include "socket.hpp"	// openStream
#include "nsl.hpp"	// my_gethostbyname
#include "pref.hpp"	// reflector
#include "timer.hpp"	// rate
#include "user.hpp"	// localuser
#include "bap.hpp"	// setMask, setBap


const OClass Humanoid::oclass(HUMANOID_TYPE, "Humanoid", Humanoid::creator);

//local
static const char DEF_BODY_URL[] = "/avatar/man.body";
static float skin[] = {1, .75, .7}; // skin color
static uint8_t vaps_offset_port = Humanoid::VAPS_OFFSET_PORT;


/** create from a fileline */
WO * Humanoid::creator(char *l)
{
  return new Humanoid(l);
}

void Humanoid::defaults()
{
  head_url = new char[URL_LEN];
  memset(head_url, 0, URL_LEN);
  strcpy(vaps, DEF_VAPS_SERVER);   // default server
  strcpy(names.url, DEF_BODY_URL); // default body

  for (int i=0; i<3; i++) cloth[i] = skin[i];
}

void Humanoid::parser(char *l)
{
  defaults();
  l = tokenize(l);
  begin_while_parse(l) {
    l = parseAttributes(l);
    if (!l) break;
    if      (! stringcmp(l, "body="))   l = parseString(l, names.url, "body");	//body
    else if (! stringcmp(l, "head="))   l = parseString(l, head_url, "head");	//head
    else if (! stringcmp(l, "face="))   l = parseString(l, head_url, "face");	//head
    else if (! stringcmp(l, "color="))  l = parseVector3f(l, cloth, "color");	//color
    else if (! stringcmp(l, "server=")) l = parseString(l, vaps, "server");	//server
  }
  end_while_parse(l);
}

void Humanoid::geometry()
{
  char s[128];

  sprintf(s, "solid shape=\"bbox\" dim=\"%f %f %f\" />", .24, .14, B_HEIGHT);
  parseSolid(s);
}

void Humanoid::behaviors()
{
  enableBehavior(NO_ELEMENTARY_MOVE);
  enableBehavior(COLLIDE_NEVER);
  enableBehavior(PERMANENT_MOVEMENT);	// bap/fap frames
  enableBehavior(SPECIFIC_RENDER);
}

void Humanoid::inits()
{
  initMobileObject(0);

  sdudp = -1;
  sdtcp = -1;
  bapfile = NULL;

  body = new Body();
  body->wobject = this;

  bap = new Bap();
  body->bap = bap;
  body->setColors(skin, cloth);
  body->load(names.url);
  body->draw();

  if (*head_url) {
    body->v3d = new V3d();
    body->v3d->load(head_url);
  }
  if (head_url) delete[] head_url;

  vaps_offset_port += 2;
  vaps_port = Channel::getPort(World::current()->getChan()) + vaps_offset_port;
  if (! ::g.pref.reflector) reset();
  state = INACTIVE;
}

/* Comes from file */
Humanoid::Humanoid(char *l)
{
  parser(l);
  geometry();
  behaviors();
  inits();
  usercontrol = false;
}

/* Called from User */
Humanoid::Humanoid()
{
  defaults();
  geometry();
  behaviors();
  inits();
  usercontrol = true;
}

void Humanoid::render()
{
  if (localuser->humanoid && ! localuser->isVisible()) return;

  body->render(pos);
}

/** init receiver in UDP mode : returns 1 if OK else 0 if fails */
int Humanoid::initReceiver()
{
  if (sdudp > 0) disconnectFromBapServer();

  if ((sdudp = Socket::openDatagram()) < 0) {
    //error("initReceiver: open UDP failed");
    return 0;
  }
  Socket::bindSocket(sdudp, INADDR_ANY, vaps_port);
#if 0 //dax
  if (ipmode == MULTICAST) {
    char group[GROUP_LEN];
    Channel::getGroup(World::current()->getChan(), group);
    inet4_pton(group, &(udpsa.sin_addr.s_addr));
    memset(&mreq, 0, sizeof(mreq));
    inet4_pton(group, &(mreq.imr_multiaddr.s_addr));
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    if (Socket::addMembership(sdudp, &mreq) < 0) {
      error("initReceiver: cannot join multicast group %s", group); return 0;
    }
    //echo("initReceiver: waiting for Multicast on %x:%d", udpsa.sin_addr.s_addr, udpsa.sin_port);
  }
  else {  // Unicast
    //echo("initReceiver: waiting for Unicast on port %d", vaps_port);
  }
#endif
  //echo("initReceiver: waiting on port %d, sdudp=%d", vaps_port, sdudp);
  return 1;
}

/** Establishes a TCP connection and send the setup packet */
int Humanoid::connectToBapServer(int _ipmode)
{
  char setup_cmd[128];
  struct hostent *hp;

  if ((sdtcp = Socket::openStream()) < 0) {
    error("socket vaps failed");
    return 0;
  }
  if ((hp = my_gethostbyname(vaps, AF_INET)) == NULL) {
    error("can't resolve vaps");
    return 0;
  }
  memset(&tcpsa, 0, sizeof(struct sockaddr_in));
  tcpsa.sin_family = AF_INET;
  tcpsa.sin_port = htons(VAPS_PORT);
  memcpy(&tcpsa.sin_addr, hp->h_addr_list[0], hp->h_length);
  my_free_hostent(hp);

  struct timeval timeout;
  timeout.tv_sec = 10;
  timeout.tv_usec = 0;

  if (setsockopt(sdtcp, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0)
    error("setsockopt failed");
  if (connect(sdtcp, (const struct sockaddr *) &tcpsa, sizeof(tcpsa)) < 0) {
    //error("connection failed with the vaps server: %s (%s)", vaps, inet4_ntop(&tcpsa.sin_addr));
    sdtcp = -1;
    return 0;
  }
  //echo("connection established with vaps server: %s(%s)", vaps, inet4_ntop(&tcpsa.sin_addr));
#if 0 //dax
  ipmode = _ipmode;
  if (ipmode == MULTICAST) {
    char group[GROUP_LEN];
    Channel::getGroup(World::current()->getChan(), group);
    sprintf(setup_cmd, "setup a=%s p=%d t=%d r=%.2f ",
            group, vaps_port, Channel::getTtl(World::current()->getChan()), ::g.timer.rate());
  }
  else
#endif
  sprintf(setup_cmd, "setup p=%d r=%.2f ", vaps_port, ::g.timer.rate());
  write(sdtcp, setup_cmd, strlen(setup_cmd));  // setup packet
  return sdtcp;
}

void Humanoid::disconnectFromBapServer()
{
  if (sdtcp > 0) {
    Socket::closeStream(sdtcp);
    if (ipmode == MULTICAST)
      Socket::dropMembership(sdudp, &mreq);
  }
  if (sdudp > 0) Socket::closeDatagram(sdudp);
  //echo("connection closed with the vaps server");
  sdtcp = sdudp = -1;
  state = INACTIVE;
}

/** get a frame from the vaps server */
int Humanoid::readBapFrame()
{
  fd_set set;
  FD_ZERO(&set);
  FD_SET(sdudp, &set);
  struct timeval delay;
  delay.tv_sec = delay.tv_usec = 0;  // set select passing

  if (select(FD_SETSIZE, &set, NULL, NULL, &delay) == 0) {
    //echo("select=0 sdudp=%d", sdudp);
    return 0;  // nothing to read
  }

  memset(bapline, 0, VAPS_BUFSIZ);
  socklen_t slen = sizeof(struct sockaddr_in);
  int len;
  // receive bap line
  if ((len = recvfrom(sdudp, bapline, VAPS_BUFSIZ, 0, (struct sockaddr *)&udpsa, &slen)) < 0) {
    error("readBapFrame: error recvfrom");
    return 0;
  }
  if (! len) {
    // end of stream
    //disconnectFromBapServer();
    //state = INACTIVE;
    return 0; // eof
  }
  return 1; // bap is present
}

/** system of equations handling permanent motion */
void Humanoid::changePermanent(float lasting)
{
  static float angle = 0;

  if (state != PLAYING) return;  // not playing

  if (usercontrol && localuser) {
    pos.x = localuser->pos.x;
    pos.y = localuser->pos.y;
    pos.z = localuser->pos.z - localuser->height / 2;
    pos.az = localuser->pos.az - M_PI_2;
    pos.ax = localuser->pos.ax;
    pos.ay = localuser->pos.ay;
    updatePosition();
  }

  if ((sdtcp > 0) && readBapFrame()) {		// from vaps server
    // bap is ready to be played
    //echo("bapline: %s", bapline);
    uint8_t baptype = bap->parse(bapline);	// parse it

    switch (baptype) {
    case TYPE_BAP_V31: case TYPE_BAP_V32: 
      for (int i=1; i <= NUM_BAPS_V32; i++) {
        if (! bap->isMask(i)) continue;
        //echo("play: %d (%.2f)", i, bap->getBap(i));
      }
      body->animate();		// play bap frame
      break;
    case TYPE_FAP_V20: case TYPE_FAP_V21:
      for (int i=1; i <= NUM_FAPS; i++) {
        if (bap->isMask(i) && body->v3d) {
          body->v3d->animate(i, bap->getFap(i)); // play fap frame
        }
      }
      break;
    case 0:	// end of frames
      // for tests only !!!
      //body->animHead(+30, 0);		// head flexion front : OK
      //body->animHead(+30, 1);		// head abduct
      //body->animHead(+30, 2);		// head torsion right : OK
      //body->animNeck(+30, 0);		// neck flexion front : OK
      //body->animNeck(-30, 1);		// neck abduct right : OK
      //body->animNeck(-30, 2);		// neck torsion : NO
      //body->animChest(-30, 0);	// chest flexion back : OK
      //body->animChest(+30, 0);	// chest flexion front : OK
      //body->animChest(-30, 1);	// chest abduct right : OK
      //body->animChest(-30, 2);	// chest torsion left : OK
      //for (int i=0; i<30; i++) {
      //body->animArm(-i, 0, 0);		// arm left flexion : OK
      //body->animArm(+i, 1, 0);		// arm right flexion : OK
      //body->animForearm(-2*i, 0, 0);	// forearm left flexion front : OK
      //body->animForearm(+2*i, 1, 0);	// forearm right flexion front : OK
      //}
      //body->animLeg(-30, 0, 0);	// leg left front flexion : OK
      //body->animLeg(+30, 0, 1);	// leg left abduct back : OK
      //body->animLeg(-30, 1, 2);	// leg right torsion ext : OK
      //body->animFoot(-30, 1, 2);	// foot right torsion ext : NO
      render();
       //disconnectFromBapServer();
       state = INACTIVE;
       angle = 0;
       //echo("disconnect");
      break;
    default:
      break;
    //TODO: predictive interpollation
    }
  }
  else if ((sdtcp > 0) && body->v3d) {
    body->v3d->animate();	// local animation
  }
  //angle = 10;
  //body->animArm(-angle, 0, 0);	// arm left flexion : OK
  //body->animArm(+angle, 1, 0);	// arm right flexion : OK
  //body->animForearm(-2*angle, 0, 0);	// forearm left flexion front : OK
  //body->animForearm(+2*angle, 1, 0);	// forearm right flexion front : OK

#if 0 //dax testing
  else if (sdtcp <= 0) {
    // get frame from local string bapfile (see gestures.hpp)
    echo("get local frame");
    static bool hdr_frame = true;
    char ch;
    char *p = NULL;
    int c = 0;
    uint8_t baptype = 0;
    int nbr_frames = 0;
    int num_frame = 0;
    int num_params = 0;
    //echo("bapfile: %s", bapfile);

    if (hdr_frame) {
      //baphdr
      memset(bapline, 0, sizeof(bapline));
      for (c = 0; (ch = bapfile[c]) != '\n'; c++) { bapline[c] = ch; }
      bapfile += (c + 1);	// + eol
      echo("hdr_frame: %s (%d)", bapline, c);
      p = strrchr(bapline, ' ');
      if (p)
        nbr_frames = atoi(++p);
      baptype = bap->parse(bapline);	// parse hdr_frame
      switch (baptype) {
      case TYPE_BAP_V31:
        num_params = NUM_BAPS_V31; break;
      case TYPE_BAP_V32: 
        num_params = NUM_BAPS_V32; break;
      case TYPE_FAP_V20: 
      case TYPE_FAP_V21: 
        num_params = NUM_FAPS; break;
      }
      //echo("baptype: %d", baptype);
      //echo("nbr_frames: %d", nbr_frames);
      hdr_frame = false;
    }

    // data
    do {
      //bapmask
      memset(bapline, 0, sizeof(bapline));
      for (c = 0; (ch = bapfile[c]) != '\n'; c++) { bapline[c] = ch; }
      bapfile += (c + 1);
      //echo("mask: %s (%d)", bapline, c);
      // masks
      p = bapline;
      for (int i=1; i <= num_params; i++) {
        if (p) {
          switch (*p) {
          case '0': bap->setMask(i, 0); break;
          case '1': bap->setMask(i, 1); break;
          }
          p = strchr(p, ' ');
          if (! p) break;	// no mask
          p++;	// skip space
        } 
        else
          break;
      } 

      //bapvalues
      memset(bapline, 0, sizeof(bapline));
      for (c = 0; (ch = bapfile[c]) != '\n'; c++) { bapline[c] = ch; }
      bapfile += (c + 1);

      // num_frame
      p = bapline;
      num_frame = atoi(p);
      echo("num_frame: %d", num_frame);

      // values
      p = strchr(bapline, ' ');
      if (! p) break;	// no values
      p++;		// first value
      echo("values: %s", p);
      for (int i=1; i <= num_params; i++) {
        if (! bap->isMask(i)) continue;
        if (i >= TR_VERTICAL && i <= TR_FRONTAL) {	// 170..172 translations
          bap->setBap(i, (float) atof(p)); // / TR_DIV));	// magic formula (300)
        }
        else {  // angles
          switch (baptype) {
          case TYPE_BAP_V31:
            bap->setBap(i, (float) (atof(p) / BAPV31_DIV)); // magic formula (1745)
            break;
          case TYPE_BAP_V32:
            bap->setBap(i, (float) (atof(p) / BAPV32_DIV)); // magic formula (555)
            break;
          }
          trace(DBG_MAN, "bap: p=%s ba[%d]=%.2f", p, i, bap->getBap(i));
        }
        echo("mask: %d (%.2f)", i, bap->getBap(i));
        p = strchr(p, ' ');	// skip space
        if (! p) break;		// end of frame
        p++;			// next value
      }

      // play frame
      switch (baptype) {
      case TYPE_BAP_V31: case TYPE_BAP_V32: 
        for (int i=1; i <= num_params; i++) {
          if (! bap->isMask(i)) continue;
          echo("play bap: %d (%.2f)", i, bap->getBap(i));
        }
        body->animate();	// play bap frame
        break;
      case TYPE_FAP_V20: case TYPE_FAP_V21:
        for (int i=1; i <= NUM_FAPS; i++) {
          if (! bap->isMask(i)) continue;
          echo("play fap: %d (%.2f)", i, bap->getFap(i));
          if (body->v3d) {
            body->v3d->animate(i, bap->getFap(i)); // play fap frame
          }
        }
        break;
      default:
        echo("baptype: %d", baptype);
        break;
      }
      //usleep(200000);	// 2/10 sec
      struct timeval to;
      to.tv_sec = 0;
      to.tv_usec = 200000; //::g.pref.frame_delay;     // 20ms -> 50 fps
      select(0, 0, 0, 0, &to);
    } while ((num_frame + 1) < nbr_frames) ;

    hdr_frame = true;
    state = INACTIVE;
    echo("end frames");
  }
#endif //dax
}

/** send a play command to the vaps server */
bool Humanoid::sendPlayToBapServer(const char *bap_name)
{
  char play_cmd[128];

  if (sdtcp <= 0) {
    if (! initReceiver()) { // UDP reception
      error("sendPlayToBapServer: receiver fails");
      return false;
    }
    if (! connectToBapServer(UNICAST)) { // opens a TCP connection
      //error("sendPlayToBapServer: connect fails");
      state = PLAYING;	// local playing
      return false;
    }
  }
  sprintf(play_cmd, "play f=%s", bap_name);
  write(sdtcp, play_cmd, strlen(play_cmd));  // TCP play packet
  state = PLAYING;
  return true;
}

void Humanoid::quit()
{
  disconnectFromBapServer();
  if (bap) delete bap;
  bap = NULL;
  if (body) delete body;
  body = NULL;
}

#if 0 //dax wrong!!!
char * Humanoid::toPlay(const char *str)
{
  if (connectToBapServer(UNICAST)) { // opens a TCP connection
    sendPlayToBapServer(str);
    return NULL;
  }
  else {
    char *newstr = strdup(str);
    char *p = newstr;
    p = strchr(newstr, '.');
    if (p)
      *p = '_';
    strcpy(bapline, newstr);
    free(newstr);
    state = PLAYING;
    return bapline;
  }
}
#endif

void Humanoid::reset()
{
  disconnectFromBapServer();  // first disconnect
  if (! initReceiver()) return;
  state = LISTENING;
  if (! connectToBapServer(MULTICAST)) return;
}

void Humanoid::pause()
{
  sendPlayToBapServer("pause.bap");
  bapfile = (char *) pause_bap;
}

void Humanoid::hi()
{
  sendPlayToBapServer("hi.bap");
  bapfile = (char *) hi_bap;
}

void Humanoid::bye()
{
  sendPlayToBapServer("bye.bap");
  bapfile = (char *) bye_bap;
}

void Humanoid::ask()
{
  sendPlayToBapServer("ask.bap");
  bapfile = (char *) ask_bap;
}

void Humanoid::sit()
{
  sendPlayToBapServer("sit.bap");
  bapfile = (char *) sit_bap;
}

void Humanoid::show()
{
  sendPlayToBapServer("show.bap");
  bapfile = (char *) show_bap;
}

void Humanoid::clap()
{
  sendPlayToBapServer("clap.bap");
  bapfile = (char *) clap_bap;
}

void Humanoid::nak()
{
  sendPlayToBapServer("nak.bap");
  bapfile = (char *) nak_bap;
}

void Humanoid::test()
{
  sendPlayToBapServer("test.bap");
  bapfile = (char *) test_bap;
}

void Humanoid::eyes()
{
  sendPlayToBapServer("eyes.fap");
  bapfile = (char *) eyes_fap;
}

void Humanoid::joy()
{
  sendPlayToBapServer("joy.fap");
  bapfile = (char *) joy_fap;
}

void Humanoid::sad()
{
  sendPlayToBapServer("sad.fap");
  bapfile = (char *) sad_fap;
}

void Humanoid::surp()
{
  sendPlayToBapServer("surp.fap");
  bapfile = (char *) surp_fap;
}

void Humanoid::jag()
{
  sendPlayToBapServer("jag.fap");
}

void Humanoid::pause_cb(Humanoid *humanoid, void *d, time_t s, time_t u)
{
  humanoid->pause();
}

void Humanoid::hi_cb(Humanoid *humanoid, void *d, time_t s, time_t u)
{
  humanoid->hi();
}

void Humanoid::bye_cb(Humanoid *humanoid, void *d, time_t s, time_t u)
{
  humanoid->bye();
}

void Humanoid::ask_cb(Humanoid *humanoid, void *d, time_t s, time_t u)
{
  humanoid->ask();
}

void Humanoid::sit_cb(Humanoid *humanoid, void *d, time_t s, time_t u)
{
  humanoid->sit();
}

void Humanoid::show_cb(Humanoid *humanoid, void *d, time_t s, time_t u)
{
  humanoid->show();
}

void Humanoid::clap_cb(Humanoid *humanoid, void *d, time_t s, time_t u)
{
  humanoid->clap();
}

void Humanoid::nak_cb(Humanoid *humanoid, void *d, time_t s, time_t u)
{
  humanoid->nak();
}

void Humanoid::test_cb(Humanoid *humanoid, void *d, time_t s, time_t u)
{
  humanoid->test();
}

void Humanoid::reset_cb(Humanoid *humanoid, void *d, time_t s, time_t u)
{
  humanoid->reset();
}

void Humanoid::eyes_cb(Humanoid *humanoid, void *d, time_t s, time_t u)
{
  humanoid->eyes();
}

void Humanoid::joy_cb(Humanoid *humanoid, void *d, time_t s, time_t u)
{
  humanoid->joy();
}

void Humanoid::sad_cb(Humanoid *humanoid, void *d, time_t s, time_t u)
{
  humanoid->sad();
}

void Humanoid::surp_cb(Humanoid *humanoid, void *d, time_t s, time_t u)
{
  humanoid->surp();
}

void Humanoid::jag_cb(Humanoid *humanoid, void *d, time_t s, time_t u)
{
  humanoid->jag();
}

/** functions initialization */
void Humanoid::funcs()
{
  setActionFunc(HUMANOID_TYPE, 0, _Action pause_cb, "Pause");
  setActionFunc(HUMANOID_TYPE, 1, _Action hi_cb, "Hi");
  setActionFunc(HUMANOID_TYPE, 2, _Action bye_cb, "Bye");
  setActionFunc(HUMANOID_TYPE, 3, _Action ask_cb, "Ask");
  setActionFunc(HUMANOID_TYPE, 4, _Action sit_cb, "Sit");
  setActionFunc(HUMANOID_TYPE, 5, _Action show_cb, "Show");
  setActionFunc(HUMANOID_TYPE, 6, _Action clap_cb, "Clap");
  setActionFunc(HUMANOID_TYPE, 7, _Action nak_cb, "Nak");
  setActionFunc(HUMANOID_TYPE, 8, _Action test_cb, "Test");
  setActionFunc(HUMANOID_TYPE, 9, _Action eyes_cb, "Eyes");
  setActionFunc(HUMANOID_TYPE, 10, _Action joy_cb, "Joy");
  setActionFunc(HUMANOID_TYPE, 11, _Action sad_cb, "Sad");
  setActionFunc(HUMANOID_TYPE, 12, _Action surp_cb, "Surp");
  setActionFunc(HUMANOID_TYPE, 13, _Action jag_cb, "Jag");
  setActionFunc(HUMANOID_TYPE, 14, _Action reset_cb, "Reset");
  setActionFunc(HUMANOID_TYPE, 15, _Action V3d::changeFace, "New");
  setActionFunc(HUMANOID_TYPE, 16, _Action V3d::changeMoveYes, "Yes");
  setActionFunc(HUMANOID_TYPE, 17, _Action V3d::changeMoveNo, "No");
  setActionFunc(HUMANOID_TYPE, 18, _Action V3d::changeMoveEyeL, "EyeL");
  setActionFunc(HUMANOID_TYPE, 19, _Action V3d::changeMoveMouth, "Mouth");
}
