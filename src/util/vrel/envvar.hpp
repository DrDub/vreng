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

#ifndef ENVVAR_H
#define ENVVAR_H

#include "data.hpp"

/**
 * class Env_var
 */
class Env_var
{
 public:

  char nom[256][30];  ///< Tableau des noms de variables.
  Data** donnee;      ///< Tableau des data associ�es.
  int count;          ///< (count-1) = nombre de variables d�clar�es.

  Env_var(); ///< initialisation des tableaux et du compteur.

  int parcours(const char * str);
  // Parcours la liste chain�e des variables existantes et renvoie l'index
  // sur cette variable si la variable existe d�j�. Sinon, renvoie 0.

  void declaration(const char* str);
  // Ajouter une variable dans le tableau de nom.

  void declaration(const char * str, float val);
  // Ajouter une variable dans le tableau de nom et l'affecter � un nombre.
  
  void declaration(const char * var, const char *str);
  // Ajouter une variable dans le tableau de nom et l'affecter � une chaine.

  int affectation(const char *, Data*);
  // M�thode d'affectation; la data sera d'abord �valu�e (m�thode get_data).

  int affectation2(const char *, Data*);
  // M�thode d'affectation sans evaluation (ie la data peut etre un pointeur 
  // sur un objet de type Variable).
  
  Data* eval(const char *);
  // Fonction qui renvoie la valeur d'une variable.

  virtual ~Env_var();		///< Destructeur

};

#endif
