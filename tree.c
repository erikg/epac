
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
 * $Id: tree.c,v 1.2 2003/02/09 01:40:13 erik Exp $
 */

#include <stdio.h>
#include "tree.h"

int scrunch(int x)
{
	if(x==0)return 0;
	if(x<0)return -1;
	if(x>0)return -1;
	printf("wtf???\n");
	exit(-1);
}

tree_t *tree_add(tree_t *tree, void *data, int (*cmp)(void *a,void *b))
{
	if(tree==NULL)
	{
		tree_t *t = (tree_t *)malloc(sizeof(tree_t));
		t->data = data;
		t->left=t->right=NULL;
		return t;
	}
	switch(scrunch(cmp(tree->data, data)))
	{
		case -1:
			tree->left=tree_add(tree->left, data, cmp);
			break;
		case 0:
			printf("Whoa, it exists alread!\n");
			break;
		case 1:
			tree->right=tree_add(tree->right, data, cmp);
			break;
	}
	return tree;
}

void *tree_search(tree_t *tree, void *data, int (*cmp)(void *a, void *b))
{
	if(tree==NULL)return NULL;
	switch(scrunch(cmp(tree->data, data)))
	{
		case -1: return tree_search(tree->left, data, cmp);
		case 0: return tree;
		case 1: return tree_search(tree->right, data, cmp);
	}
	printf("Should not have gotten here: %s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
	return NULL;
}

void
tree_traverse_213 (tree_t * tree, void (*func) (void * n))
{
    if (tree == NULL)
	return;
    if (tree->left)
	tree_traverse_213 (tree->left, func);
    func (tree->data);
    if (tree->right)
	tree_traverse_213 (tree->right, func);
    return;
}
