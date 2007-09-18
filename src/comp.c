
/*****************************************************************************
 * Erik's Partial Archive Collator                                           *
 * Copyright (C) 2002-2007 Erik Greenwald <erik@smluc.org>                   *
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
 * $Id: comp.c,v 1.11 2007/09/18 22:54:47 erik Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/mman.h>

#include "comp.h"
#include "display.h"
#include "epac.h"

#define BSDBAD(x) ((size_t)(x) == 0x5a5a5a5a)

static int
compare (char *ba, char *bb, int size, int shortsize)
{
    if (size > shortsize && (memcmp (ba, bb, shortsize)
	    || memcmp (ba + size - shortsize, bb + size - shortsize,
		shortsize)))
	return -1;
    return memcmp (ba, bb, size);
}

static struct filegroup_s *
possiblematch (struct filegroup_s *a, struct filegroup_s *b)
{
    int size, fa, fb, shortsize;
    void *ba, *bb;

    if (BSDBAD(a->files))
    {
	printf ("%s:%d A is blown!\n", __FILE__, __LINE__);
    }

    ++possiblematchcount;
    if (a->size < b->size)
	list_swap(a,b);
    size = b->size;
    shortsize = MIN (size, CMPSIZE);

    fa = open (a->files->filename, O_RDONLY);
    ba = mmap (0, size, PROT_READ, MAP_SHARED, fa, 0);
    if (ba == MAP_FAILED)
	perror ("ba failed");

    fb = open (b->files->filename, O_RDONLY);
    bb = mmap (0, size, PROT_READ, MAP_SHARED, fb, 0);
    if (bb == MAP_FAILED)
	perror ("bb failed");

    if (compare (ba, bb, size, shortsize) == 0)
	epac_handle_match (a, b);

    munmap (ba, size);
    munmap (bb, size);
    close (fa);
    close (fb);
    return b;
}

/********************************************************************/

/** 
 * Walk the list and look for possible matches 
 */
void
compagainst (struct filegroup_s *a)
{
    struct filegroup_s *b;

    while (a && a->next)
    {
	printf(" <<< \n");
	list_print(a);
	printf(" >>> \n");
	b = a->next;
	while (b)
	{
	    int size;

	    ++at;
	    if (verbose)
		showstatus ((float)at / (float)count);

	    if (BSDBAD(a))
		printf ("%s:%d a is blown\n", __FILE__, __LINE__);
	    if (BSDBAD(a->files))
		printf ("%s:%d a->files is blown\n", __FILE__, __LINE__);
	    if (BSDBAD(a->buf))
		printf ("%s:%d a->buf is blown\n", __FILE__, __LINE__);

	    size = MIN (MIN (a->size, b->size), CMPSIZE);
	    if (memcmp (a->buf, b->buf, size) == 0)	/* seeing a crash here... bus fault... a is all forked up */
	    {
		struct filegroup_s *fg[2];
		int rez = 0;

		fg[0] = a;
		fg[1] = b;
		rez = possiblematch (a, b);
		switch(rez) {
		    case 0:
			break;
		    case 1:
			break;
		    case 2:
			break;
		}
	    } else
		b = b->next;
	}
	if (BSDBAD(a->next->buf))
	    printf ("About to walk to a bad a... from %s\n",
		a->files->filename);
	a = a->next;
    }
    return;
}

/** 
 * This is called when a possible match is found. It executes a careful check,
 * and either automatically combines or prompts for combination instructions.
 */
struct filegroup_s *
combine (struct filegroup_s *a, struct filegroup_s *b)
{
    struct filegroup_s *fg;
    struct filename_s *fn;
    struct filename_s *blah;

    /*
     * make a the larger of the two, so b must be reduced
     */
    if (b->size > a->size)
    {
	struct filegroup_s *tmp;

	tmp = a;
	a = b;
	b = tmp;
    }

    printf ("\nCollapsing \"%s\" into \"%s\"\n", b->files->filename,
	a->files->filename);

    fn = b->files;
    printf ("\nb: ");
    while (fn)
    {
	printf ("%s ", fn->filename);
	fn = fn->next;
    }

    fn = a->files;
    printf ("\na: ");
    while (fn)
    {
	printf ("%s ", fn->filename);
	fn = fn->next;
    }
    printf ("\n\n");

    fn = b->files;
    fg = b->next;
    while (fn)
    {
	/* re-link */
	unlink (fn->filename);
	link (a->files->filename, fn->filename);
	printf ("\n%s -> %s\n", fn->filename, a->files->filename);

	/* update the lists */
	addfilename(a, fn->filename);
	free (fn->filename);
	fn->filename = NULL;

	/* free the garbage */
	blah = fn;
	fn = fn->next;
	free (blah);
	blah = NULL;
    }
    fn=NULL;
    reclaimed += (double)b->size;
    printf ("Reclaimed %d bytes (%.0f bytes total (%.0fk %.0fm))\n", b->size,
	reclaimed, reclaimed / 1024.0, reclaimed / (1024.0 * 1024.0));
    if (b->prev)
	b->prev->next = b->next;
    if (b->next)
	b->next->prev = b->prev;
    free (b);
    b = NULL;
    if (BSDBAD(a->files))
	printf ("%s:%d Whoa, a is blown...\n", __FILE__, __LINE__);
    return fg;
}
