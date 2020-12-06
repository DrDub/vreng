/****************************************************************************
 *                             VREL COMPONENTS                              *
 *                                                                          *
 *                           Copyright (C) 2000                             *
 *     Yanneck Chevalier, Pascal Belin, Alexis Jeannerod, Julien Dauphin    *
 *                                                                          *
 *   This program is free software; you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation; either version 2 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   This program is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 ****************************************************************************/

/**** classe Decl_var_liste ****/

#include "vrel.hpp"
#include "decllist.hpp"
#include "declvar.hpp"
#include "envvar.hpp"


// Constructeur de la classe.
Decl_var_liste::Decl_var_liste ()
{
  varloc = new Env_var();
}

// Destructeur de la classe.
Decl_var_liste::~Decl_var_liste ()
{
  delete varloc;
}

// Ajout d'une variable a la liste varlocal.
void Decl_var_liste::adddeclarvar (Decl_var* declarvar)
{
  varloc->declaration (declarvar->nomvar);
  if (declarvar->donnee != NULL)
    varloc->affectation (declarvar->nomvar, declarvar->donnee);
}
