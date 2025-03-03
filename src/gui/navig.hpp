//---------------------------------------------------------------------------
// VREng (Virtual Reality Engine)       http://vreng.enst.fr/
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
#ifndef NAVIG_HPP
#define NAVIG_HPP

#include "widgets.hpp"

class Widgets;


/**
 * Navig class
 */
class Navig {

public:
  Navig(Widgets*, class Scene&);
  UBox& manipulator();
  
  void selectObject(ObjInfo* objinfo);

  void userMotion(UMouseEvent& e, Motion* mvt_x, Motion* mvt_y);
  void objectMove(UMouseEvent& e, Motion* mvt_x, Motion* mvt_y);
  void doMotion(UMouseEvent& e);
  void stopMotion();
  void clearInfoBar();

private:
  friend class Widgets;

  Widgets& gw;
  float xref, yref;
  bool followMouse;		///< flag to follow mouse.
  int depthsel;
  UStr selected_object_url;
  UStr object_name, object_class;
  ULabel object_infos;		///< infos box
  UPopmenu object_menu;
  UCtlmenu navig_menu;
  uptr<UMenu> opened_menu;

  void initNavigMenu();
  void mouseRefCB(UMouseEvent& e);
  void mousePressB1orB3(UMouseEvent& e, int x, int y, int button_no);
  void mousePressB2(UMouseEvent& e, int x, int y);
  void mousePressCB(UMouseEvent& e);  
  void mouseReleaseCB(UMouseEvent& e);
  void mouseDragCB(UMouseEvent& e);
  void mouseMoveCB(UMouseEvent& e);
  void keyPressCB(UKeyEvent& e);
  void keyReleaseCB(UKeyEvent& e);
 };

#endif
