
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
 * $Id: epac.c,v 1.17 2003/02/20 14:19:47 erik Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "dir.h"
#include "tree.h"

int
doversion ()
{
	printf ("\
%s %s Copyright (C) 2002-2003 Erik Greenwald <erik@smluc.org>\n\
%s comes with ABSOLUTELY NO WARRANTY. Please read the GPL for details.\n\n", PACKAGE, PACKAGE, VERSION);
	return 0;
}

int
dohelp (char *name)
{
	doversion (name);
	printf ("Usage\n\
\t%s [-hv] [-s] <dir>\n\
\n\
 -s      Only do savings\n\
 -h      Display this help screen\n\
 -v      Display the version\n\
\n", name, name);
	return 0;
}

void
print (void *n)
{
	printf ("%s\n", (char *)n);
}

int
main (int argc, char **argv)
{
	int c, only_do_savings = 0, do_recursive = 0;
	tree_t *itree = NULL;
/*
	tree_t *dtree = NULL;
*/

	tree_t *basedirs = NULL;

	while ((c = getopt (argc, argv, "hvsr")) != -1)
		switch (c)
		{
		case 'h':
			dohelp (argv[0]);
			return EXIT_SUCCESS;
		case 'v':
			doversion (argv[0]);
			return EXIT_SUCCESS;
		case 's':
			only_do_savings = 1;
			break;
		case 'r':
			do_recursive = 1;
			break;
		case ':':
			printf ("Option \"%s\" missing parameter\n", optarg);
			dohelp (argv[0]);
			return 1;
		case '?':
			dohelp (argv[0]);
			return 1;
		default:
			printf ("Unknown error (option: %c)\n", c);
			dohelp (argv[0]);
			return 2;
		}
	argc -= optind;
	argv += optind;

	while (*argv)
	{
		itree = dirspew (itree, *argv, only_do_savings, do_recursive);
		++argv;
	}

	if (only_do_savings)
	{
		printf ("not implemented yet\n");
		return 0;
	}

	tree_traverse_213 (basedirs, print);

	return EXIT_SUCCESS;
}
