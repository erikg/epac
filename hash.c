
/*****************************************************************************
 * Erik's Partial Archive Collator                                           *
 * Copyright (C) 2002-2003 Erik Greenwald <erik@smluc.org>                   *
 *                                                                           *
 * This program takes a directory as an argument, then walks through the     *
 * directory looking for duplicate and partially duplicate files. If it      *
 * an exact duplicate, it deletes one and creates a hardlink to the other,   *
 * minimizing disk usage. If it finds a pair of files where they contain the *
 * same data up to the size of the smaller file, it will prompt if you want  *
 * to combine them. If you say yes, it will delete the smaller of the files  *
 * and hardlink to the larger.                                               *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      * 
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 2 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this program; if not, write to the Free Software               *
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA *
 ****************************************************************************/

/*
 * $Id: hash.c,v 1.3 2003/03/02 17:11:14 erik Exp $
 */

#include <stdio.h>
#include <stdlib.h>


#include "hash.h"

hash_t *
hash_spawn (int width, int (*func) (void *))
{
  hash_t *h = (hash_t *) malloc (sizeof (hash_t));
  h->width = width;
  h->func = func;
  h->table = malloc (width * sizeof (void *));
  memset (h->table, 0, width * sizeof (void *));
  return h;
}

void *
hash_lookup (hash_t * hash, void *data)
{
  int i;
  i = hash->func (data);
  if (i > hash->width || i < 0)
    printf ("Bad hash!\n");
  else
    return &hash->table[i];
  return NULL;
}
