
/*****************************************************************************
 * Erik's Partial Archive Collator
 * Copyright (C) 2002 Erik Greenwald <erik@smluc.org>                        *
 *                                                                           *
 * This program takes a directory as an argument, then walks through the     *
 * directory looking for duplicate and partially duplicate files. If it      *
 * an exact duplicate, it deletes one and creates a hardlink to the other,   *
 * minimizing disk usage. If it finds a pair of files where they contain the *
 * same data up to the size of the smaller file, it will prompt if you want  *
 * to combine them. If you say yes, it will delete the smaller of the files  *
 * and hardlink to the larger. 
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
 * $Id: list.c,v 1.11 2004/02/22 20:17:55 erik Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#ifdef __linux__
# ifndef __USE_BSD
#  define __USE_BSD
# endif
#endif
#include <string.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>


#include "epac.h"
#include "list.h"

struct filegroup_s *filelist = NULL;

/**
 * Search the list for the first occurence of 'inode'. Note that the returned
 * list is not discrete; it is the list of which the head matches the inode. The
 * next param should probably be ignored, but cannot be set to null without
 * damage to the list. 
 * @param inode The key to search on.
 * @param fl The list to search;
 * @return The list starting with the matching element. The returned sublist is
 * NOT a copy, do NOT modify it.
 */
struct filegroup_s *
searchlist (ino_t inode, struct filegroup_s *fl)
{
    /*
     * if the compile doesn't short circuit the || like it should, this will cause a segfault or worse 
     */
    return fl == NULL
	|| fl->inode == inode ? fl : searchlist (inode, fl->next);
}

/**
 * Add a filename to an existing inode in the list.
 * @param fl The filegroup node to add the filename to.
 * @param filename The filename to add.
 * @return Void. Nil.
 */
void
addfilename (struct filegroup_s *fl, char *filename)
{
    struct filename_s *f;

    f = (struct filename_s *)malloc (sizeof (struct filename_s));
    memset (f, 0, sizeof (struct filename_s));
    f->filename = strdup (filename);
    f->next = fl->files;
    fl->files = f;
}

/**
 * Create a new node in the linked list. (found a new inode). The new node is
 * prepended to the list, and the list returned.
 * @param filelist The filelist to insert this element at.
 * @param sb The stat struct for the new element.
 * @return The new list.
 */
struct filegroup_s *
addnewnode (struct filegroup_s *filelist, char *filename, struct stat *sb)
{
    int f, size;
    struct filegroup_s *new;

    ++inodecount;

    size = MIN (sb->st_size, CMPSIZE);

    new = (struct filegroup_s *)malloc (sizeof (struct filegroup_s));
    memset (new, 0, sizeof (struct filegroup_s));
    new->inode = sb->st_ino;
    addfilename (new, filename);
    new->size = sb->st_size;
    if ((new->buf = (void *)malloc (size)) == 0)
    {
	printf ("Malloc error!!! %s:%d\n", __FILE__, __LINE__);
	exit (-1);
    }
    memset(new->buf, 0, size);
    f = open (filename, O_RDONLY);
    if (read (f, new->buf, size) != size)
    {
	printf ("Read error!!! %s:%d\n", __FILE__, __LINE__);
	exit (-1);
    }
    close (f);

    new->next = filelist;
    if (filelist)
	filelist->prev = new;
    return new;
}

/**
 * Add a filename/stat to the filelist global.
 * @todo Refactor to take a list and return the new list.
 * @param filename The name of the file (with path).
 * @param sb The stat block for this file. Used to determine if the file is a
 * dup or not.
 * @return Void. Nothing. Assumes success.
 */
void
addtolist (char *filename, struct stat *sb)
{
    struct filegroup_s *fl;

    ++filecount;

    if (sb->st_size == 0)
	return;
    fl = searchlist (sb->st_ino, filelist);
    if (fl == NULL)
	filelist = addnewnode (filelist, filename, sb);
    else			/* it's already in the list, just add this file name to the inode entry */
	addfilename (fl, filename);
    return;
}

/**
 * Query the length of a (filegroup_s) linked list. This is a simple recursive
 * function. If fed a NULL, it will return a 0. If fed a bad pointer, it will
 * crash, or worse.
 * @param f the list to query
 * @return the length of the list
 */
int
listlength (struct filegroup_s *f)
{
    return f == NULL ? 0 : 1 + listlength (f->next);
}
