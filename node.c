
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
 * $Id: node.c,v 1.3 2003/02/20 14:19:47 erik Exp $
 */

#include <stdlib.h>

#include "node.h"

int ino_cmp(void *_a, void *_b)
{
	node_t *a = (node_t *)_a;
	node_t *b = (node_t *)_b;
	return a->ino == b->ino ? 0 : a->ino < b->ino ? -1 : 1;
}

node_t *node_new(char *name, struct stat *sb)
{
	node_t *n = (node_t *)malloc(sizeof(node_t));
	memset(n, 0, sizeof(node_t));

	n->nametree = tree_add(NULL, name, ino_cmp);
	n->size = sb->st_size;
	n->ino = sb->st_ino;
	/*
		n->dev = sb->st_dev;
	*/
	return n;
}
