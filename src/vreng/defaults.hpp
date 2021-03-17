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
#ifndef DEFAULTS_HPP
#define DEFAULTS_HPP

/* default servers */
#if STANDALONE
#define IPMC_ENABLED		0	// ipmulticast disabled
#define DEF_HTTP_SERVER		"localhost"
#define DEF_VJS_SERVER		"localhost"
#define DEF_VAPS_SERVER		"localhost"
#define DEF_VACS_SERVER		"localhost"
#define DEF_VRUM_SERVER		"localhost"
#define DEF_MYSQL_SERVER	"localhost"
#else
#define IPMC_ENABLED		1	// ipmulticast enabled
#define DEF_HTTP_SERVER		"localhost"	// was "www.vreng.enst.fr" but recently unreachable
						// because it is no more maintained today
						// the httpd server and its datas associated
						// should be well known at a stable location (url)
						// and so reachable for everybody
						// allowing multi-users behavior
						// localhost is not the ideal solution (mono-user)
						// localhost allows to run simply autonomously.
						// see also DEF_URL_PFX witch can be changed
#define DEF_VJS_SERVER		"localhost"
#define DEF_VAPS_SERVER		"localhost"
#define DEF_VACS_SERVER		"localhost"
#define DEF_VRUM_SERVER		"localhost"
#define DEF_MYSQL_SERVER	"localhost"
#endif

#define VRE_VERSION	8			// current version of vre files

/* default URLs */
#define GEN_URL(h, u, p) 	("http://" h u p)
#if !defined(DEF_URL_PFX)
// default url prefix: can be changed to be adapted to your site.
#define DEF_URL_PFX	"~dax/"			// prefix to tune should be "" (old prefix)
#endif
#define DEF_URL_WORLD	"/vre/v8/Rendezvous.vre"	// old location: /vre/v8/Rendezvous.vre
#define DEF_URL_WORLD_BAK "/vre.v8/Rendezvous.vre" // old location: /vre/v8/Rendezvous.vre
#define DEF_URL_FRONT	"/gif/default.gif"
#define DEF_URL_BACK	"/gif/default.1.gif"
#define DEF_URL_BALL	"/gif/ball.gif"
#define DEF_URL_FONT	"/txf/arial-br-18.txf"
#define DEF_URL_PAPER	"/jpg/paper.jpg"
#define DEF_URL_NOISE	"/mp3/sheet.mp3"
#define DEF_URL_JAR	"/jar/vrengapp.jar"

/* default IPmc addresses & ports */
#define VRENG_MADDR_BASE	"224.255.0.0"
#define VRENG_MPORT_BASE	52656
#define DEF_VRE_MPORT		52666
#define DEF_VRE_TTL		127
#define DEF_VRE_MADDR		"224.255.0.1"
#define DEF_VRE_CHANNEL		"224.255.0.1/52666/127"
#define DEF_MANAGER_CHANNEL	"224.255.0.0/52656/127"

#define VACS_PORT		4150 + VRE_VERSION	// port of vacs server
#define SAP_ADDR		"224.2.127.254"
#define SAP_PORT		9875

#define DEF_RATE		50	// 50 frames/sec
#define DEF_RATIO3D		(16./9.)// screen ratio 16/9
#define DEF_WIDTH3D		960 	// screen width
#define DEF_HEIGHT3D		540 	// screen height
#define DEF_MAXSIMCON		8	// MAX simultaneous threaded connections
#define DEF_MAXFRAMES		255	// MAX frames
#define DEF_REFRESH_TIMEOUT	1.5	//orig: 1.5

#define PATH_LEN		128	// max lenght of path file
#define CHAN_LEN		32	// max length of a channel
#define GROUP_LEN		16	// max length of a group
#define URL_LEN			256	// max length of an url
#define USER_LEN		9	// max lenght of user name
#define OBJNAME_LEN		64	// max lenght of object name
#define HNAME_LEN		11	// max length of understandable name
#define ACTIONNAME_LEN		16	// max length of action name
#define ACTIONSNUMBER		64	// number max of actions per object
#define PROPSNUMBER		20	// number max of object's properties

#define NEEDLOOPBACK		0	// no loopback for ipmulticast

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif
#ifndef RAND_MAX
#define RAND_MAX	32767
#endif

#endif
