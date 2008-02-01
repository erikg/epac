
/*****************************************************************************
 * Erik's Partial Archive Collator                                           *
 * Copyright (C) 2002-2008 Erik Greenwald <erik@smluc.org>                   *
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
 * $Id: list.h,v 1.8 2008/02/01 20:08:53 erik Exp $
 */

#ifndef LIST_H
#define LIST_H

#include <sys/types.h>
#include <sys/stat.h>

struct filename_s {
    char *filename;
    struct filename_s *next;
};

struct filegroup_s {
    unsigned int inode;
    struct filename_s *files;
    void *buf;
    unsigned int size;
    struct filegroup_s *prev;
    struct filegroup_s *next;
};

extern struct filegroup_s *filelist;

void list_addfilename (struct filegroup_s *fl, char *filename);
struct filegroup_s *list_search (ino_t inode, struct filegroup_s *fl);
void list_add (char *filename, struct stat *sb);
int list_length (struct filegroup_s *filelist);
struct filegroup_s *list_deletenode (struct filegroup_s *node);
void list_print (struct filegroup_s *);
void list_swap (struct filegroup_s *, struct filegroup_s *);

#endif
