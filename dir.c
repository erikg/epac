
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
 * $Id: dir.c,v 1.9 2003/03/01 19:15:01 erik Exp $
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#include "dir.h"
#include "list.h"
#include "funcs.h"
#include "node.h"

static long long int count=0, read;

list_t *
dirspew (list_t * ilist, char *dir, int only_do_savings, int do_recursive)
{
	DIR *d;
	struct dirent *de;
	char buf[BUFSIZ];

	d = opendir (dir);
	if (d == NULL)
		return ilist;

	while ((de = readdir (d)) != NULL)
	{
		static struct stat sb;

		snprintf (buf, BUFSIZ, "%s/%s", dir, de->d_name);

		if (de->d_type == DT_DIR)
		{
				/* dont' care bout "." or "..", that'd be bad recursion */
			if (de->d_name[0] == '.' && (de->d_name[1] == 0
				|| (de->d_name[1] == '.' && de->d_name[2] == 0)))
				continue;
			if (do_recursive)
				ilist =
				    dirspew (ilist, buf, only_do_savings,
				    do_recursive);
		} else if (!stat (buf, &sb) && sb.st_mode&S_IFREG)
		{
			list_t *node = NULL;
			node_t *n = node_new(buf, &sb);
			ilist = list_add_at_head (ilist, n);
			count++;
			printf("%d\r", count);
		}
	}

	closedir (d);
	return ilist;
}

