
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
 * $Id: list.c,v 1.5 2003/03/02 17:10:54 erik Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include "list.h"

list_t *
list_add_at_head (list_t * list, void *data)
{
  list_t *l;
  l = (list_t *) malloc (sizeof (list_t));
  l->data = data;
  l->prev = NULL;
  if (list)
    {
      list->prev = l;
      l->next = list;
      l->length = list->length + 1;
    }
  else
    {
      l->next = NULL;
      l->length = 1;
    }
  return l;
}

list_t *
list_add_sorted (list_t * list, void *data, int (*cmp) (void *a, void *b))
{
  if (list == NULL)
    {
      list_t *t = (list_t *) malloc (sizeof (list_t));

      t->data = data;
      t->prev = t->next = NULL;
      return t;
    }
#if 0
  switch (scrunch (cmp (list->data, data)))
    {
    case -1:
      list->left = list_add (list->left, data, cmp);
      break;
    case 0:
      /* ignore duplicates? */
      break;
    case 1:
      list->right = list_add (list->right, data, cmp);
      break;
    }
#endif
  return list;
}

list_t *
list_search (list_t * list, void *data, int (*cmp) (void *a, void *b))
{
  if (list == NULL)
    return NULL;
  if (cmp (list->data, data) == 0)
    return list;
  return list_search (list->next, data, cmp);
}

void
list_traverse (list_t * list, void (*func) (void *n))
{
  if (list == NULL)
    return;
  func (list->data);
  if (list->next)
    list_traverse (list->next, func);
  return;
}

int
list_length (list_t * list)
{
  return list ? list->length : 0;

}
