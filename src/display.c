
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
 * $Id: display.c,v 1.1 2004/04/11 15:06:24 erik Exp $
 */

#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include "display.h"

void
printfilenames (struct filename_s *f)
{
    if (f == NULL)
    {
	printf ("\n");
	return;
    }
    printf ("%s ", f->filename);
    printfilenames (f->next);
    return;
}

void
showstatus (float stat)
{
    static int dirty = -1;
    static float last = -1.0;
    static char buf[1024] =
	"\r  0.00% [                                                                   ] ";
    int flooble;

    if (fabs (stat - last) < .0001)
	return;

    last = stat;
    sprintf (buf + (stat >= 1.0 ? 1 : stat >= .10 ? 2 : 3), "%0.02f%%",
	100.0 * stat);
    flooble = (int)(67.0 * stat);
    if (flooble > dirty)
    {
	int i;

	dirty = flooble;
	for (i = 0; i < flooble; ++i)
	    buf[i + 10] = '=';
	write (STDOUT_FILENO, buf, 78);
    } else
	write (STDOUT_FILENO, buf, 7);
    fflush (stdout);
    return;
}
