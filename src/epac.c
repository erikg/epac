
/*****************************************************************************
 * Erik's Partial Archive Collator                                           *
 * Copyright (C) 2002-2005 Erik Greenwald <erik@smluc.org>                   *
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
 * $Id: epac.c,v 1.12 2007/09/05 15:36:09 erik Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <dirent.h>


#include "config.h"

#include "comp.h"
#include "display.h"
#include "epac.h"
#include "list.h"

unsigned int count = 0, inodecount = 0, filecount = 0, possiblematchcount =
    0, at = 0;
unsigned int only_do_savings = 0, do_recursive = 0, verbose = 0;

double reclaimed = 0.0;

int
epac (int argc, char **argv)
{
    DIR *d;
    struct dirent *de;
    int i = 0;
    char buf[BUFSIZ];

    /* for each toplevel dir... */
    while (argc--)
    {
	d = opendir (*argv);
	
	if (d == NULL)
	    continue;

	while ((de = readdir (d)) != NULL)
	{
	    static struct stat sb;

	    snprintf (buf, BUFSIZ, "%s/%s", *argv, de->d_name);

	    if (stat (buf, &sb) == -1)
		perror ("stat issue? :");
	    else if (sb.st_mode & S_IFREG && sb.st_size)
		addtolist (buf, &sb);

	    if (verbose && !(i % 8))
		printf ("\rFiles: %d\tInodes: %d", filecount, inodecount),
		    fflush (stdout);
	    ++i;
	}
	closedir (d);
	argv++;
    }
    count = inodecount;
    count = (int)((float)count * (float)count / 2.0);
    if (verbose)
	printf ("\nDir read completed, %d inodes, estimating %d scans\n",
	    inodecount, count);

    printlist(filelist);
    if (filelist && filelist->next)
	compagainst (filelist);
    else
	printf ("uh?\n");

    if (verbose)
    {
	showstatus (1.0);
	printf ("\n%d possiblematch calls, %.2f%% scans\n", possiblematchcount,
	    100.0 * possiblematchcount / count);
	printf ("%.0f bytes (%0.2f k, %02.f m) recovered\n", reclaimed,
	    reclaimed / 1024.0, reclaimed / (1024.0 * 1024.0));
    }

    return 0;
}


/**
 */
void
epac_handle_match (struct filegroup_s *a, struct filegroup_s *b)
{
    if (a->size == b->size)
	b = combine (a, b);
    else
    {
	char c[BUFSIZ];

	printf
	    ("\n====================================================================\
	      \n(%d)\t\"%s\"\n(%d)\t\"%s\"\
	      \n====================================================================\n",
	    a->size, a->files->filename, b->size, b->files->filename);
	while (1)
	{
	    printf ("\aCombine? y/n > ");
	    scanf ("%1s", c);
	    switch(tolower(*c)) {
		case 'y':
		    b = combine (a, b);
		    break;
		case 'n':
		    break;
		default:
		    printf ("Unknown option \"%c\"\n", *c);
	    }
	}
    }
}
