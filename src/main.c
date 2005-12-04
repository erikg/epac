
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
 * $Id: main.c,v 1.7 2005/12/04 20:04:56 erik Exp $
 */

#include "config.h"

/* this should come out of the src and into configure.ac/config.h */
#ifdef __linux__
# ifndef __USE_BSD
#  define __USE_BSD
# endif
#include <getopt.h>
#include <sys/stat.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include "display.h"
#include "epac.h"

void 
winch(int signal) 
{
    display_set_width (atoi(getenv("COLUMNS")));
}

int
doversion (char *name)
{
    printf ("\
%s (%s) Copyright (C) 2002-2005 Erik Greenwald <erik@smluc.org>\n\
%s comes with ABSOLUTELY NO WARRANTY. Please read the GPL for details.\n\n", name, PACKAGE, VERSION);
    return 0;
}

int
dohelp (char *name)
{
    doversion (name);
    printf ("Usage\n\
\t%s [-hv] [-C [-W <cols>]] <dir>\n\
\n\
 -C                   Display completion information\n\
 -W NUM  --width=NUM  Output at most NUM (default 80) characters per line.\n\
 -h                   Display this help screen\n\
 -v                   Display the version\n\
\n", name);
    return 0;
}

int
main (int argc, char **argv)
{
    int c;

    signal(SIGWINCH, winch);

    while ((c = getopt (argc, argv, "Chv")) != -1)
	switch (c)
	{
	case 'C':
	    verbose = 1;
	    break;
	case 'W':
	    display_set_width (atoi (optarg));
	    signal(SIGWINCH, SIG_IGN);
	    break;
	case 'h':
	    dohelp (*argv);
	    return EXIT_SUCCESS;
	case 'v':
	    doversion (*argv);
	    return EXIT_SUCCESS;
	case 's':		/* not yet */
	    only_do_savings = 1;
	    break;
	case 'r':		/* not yet */
	    do_recursive = 1;
	    break;
	case ':':
	    printf ("Option \"%s\" missing parameter\n", optarg);
	    dohelp (*argv);
	    return 1;
	case '?':
	    dohelp (*argv);
	    return 1;
	default:
	    printf ("Unknown error (option: %c)\n", c);
	    dohelp (*argv);
	    return 2;
	}
    argc -= optind;
    argv += optind;

    if (argc <= 0)
    {
	dohelp (*argv);
	return 2;
    }

    return epac (argc, argv);
}
