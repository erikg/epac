
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

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>

	/*
	 * how many bytes to store for 'fast' comparison. If this number
	 * is too small, it will cause a lot of hard file comparisons. If
	 * it's too large, it'll burn up memory. I suggest something between
	 * 256 and 4096. This is the 'safe' default... 
	 */
#define CMPSIZE 256

#define MIN(a,b) (a)<(b)?(a):(b)
#define MAX(a,b) (a)>(b)?(a):(b)

int count = 0, inodecount = 0, filecount = 0, possiblematchcount = 0, at =
    0, reclaimed = 0;

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

struct filegroup_s *filelist = NULL;

struct filegroup_s *
searchlist (ino_t inode, struct filegroup_s *fl)
{
    if (fl == NULL)
	return NULL;
    if (fl->inode == inode)
	return fl;
    return searchlist (inode, fl->next);
}

void
addtolist (char *filename, struct stat *sb)
{
    struct filegroup_s *fl, *new;

    ++filecount;

    if (sb->st_size == 0)
	return;
    fl = searchlist (sb->st_ino, filelist);
    if (fl == NULL)
    {
	int f, size;

	++inodecount;

	size = MIN (sb->st_size, CMPSIZE);

	new = (struct filegroup_s *)malloc (sizeof (struct filegroup_s));
	new->inode = sb->st_ino;
	new->files = (struct filename_s *)malloc (sizeof (struct filename_s));
	new->files->filename = strdup (filename);
	new->files->next = NULL;
	new->size = sb->st_size;
	f = open (filename, O_RDONLY);
	if ((new->buf = (void *)malloc (size)) == 0)
	{
	    printf ("Malloc error!!! %s:%d\n", __FILE__, __LINE__);
	    exit (-1);
	}
	if (read (f, new->buf, size) != size)
	{
	    printf ("Read error!!! %s:%d\n", __FILE__, __LINE__);
	    exit (-1);
	}
	close (f);

	if (!filelist)
	{
	    new->next = new->prev = NULL;
	    filelist = new;
	} else if (filelist->size < new->size)
	{
	    new->next = filelist;
	    new->prev = NULL;
	    filelist->prev = new;
	    filelist = new;
	} else
	{
	    struct filegroup_s *f;

	    f = filelist;
	    while (f->next && f->size > new->size)
		f = f->next;
	    if (f->next == NULL)
	    {
		f->next = new;
		new->prev = f;
		new->next = NULL;
	    } else
	    {
		new->prev = f->prev;
		new->next = f;
		if (f->prev)
		    f->prev->next = new;
		f->prev = new;
	    }
	}

    } else
    {
	struct filename_s *f;

	f = (struct filename_s *)malloc (sizeof (struct filename_s));
	f->filename = strdup (filename);
	f->next = fl->files;
	fl->files = f;
    }
    return;
}

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
    static int dirty = 0;
    static float last = -1.0;
    static char buf[1024] =
	"\r   .  % [                                                                   ] ";
    int flooble;

    if (fabs (stat - last) < .0001)
	return;

    last = stat;
    sprintf (buf + (stat >= 1.0 ? 1 : stat >= .10 ? 2 : 3), "%.02f%%",
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
	write (STDOUT_FILENO, buf, 10);
    fflush (stdout);
    return;
}


struct filegroup_s *
combine (struct filegroup_s *a, struct filegroup_s *b)
{
    struct filegroup_s *fg;
    struct filename_s *fn;
    struct filename_s *blah;

    printf ("\nCollapsing %s into %s\n", b->files->filename,
	a->files->filename);

    fn = b->files;
    fg = b->next;
    while (fn)
    {
	blah = fn;
	unlink (fn->filename);
	link (a->files->filename, fn->filename);
	printf ("\n%s -> %s\n", fn->filename, a->files->filename);
	free (fn->filename);
	fn = fn->next;
	free (blah);
	blah = NULL;
    }
    if (b->prev)
	b->prev->next = b->next;
    if (b->next)
	b->next->prev = b->prev;
    reclaimed += b->size;
    printf ("Reclaimed %d bytes (%d bytes total)\n", b->size, reclaimed);
    free (b);
    return fg;
}

struct filegroup_s *
possiblematch (struct filegroup_s *a, struct filegroup_s *b)
{
    static int size, fa, fb;
    static void *ba, *bb;

    ++possiblematchcount;
    size = MIN (a->size, b->size);
    fa = open (a->files->filename, O_RDONLY);
    fb = open (b->files->filename, O_RDONLY);
    ba = mmap (0, size, PROT_READ, MAP_SHARED, fa, 0);
    if (ba == MAP_FAILED)
	perror ("ba failed");
    bb = mmap (0, size, PROT_READ, MAP_SHARED, fb, 0);
    if (bb == MAP_FAILED)
	perror ("bb failed");
    if (memcmp (ba, bb, size) == 0)
    {
	printf ("\nA match!\n");
	printf ("%d ", a->size);
	printfilenames (a->files);
	printf ("%d ", b->size);
	printfilenames (b->files);
	if (a->size == b->size)
	    b = combine (a, b);
	else
	{
	    char c;

	    while (1)
	    {
		c = 0;
		printf ("\aCombine? y/n > ");
		scanf ("%c", &c);
		if (c == 'y' || c == 'Y')
		{
		    b = combine (a, b);
		    break;
		} else if (c == 'n' || c == 'N')
		    break;
	    }
	}
    }
    munmap (ba, size);
    munmap (bb, size);
    close (fa);
    close (fb);
    return b;
}

void
compagainst (struct filegroup_s *a)
{
    struct filegroup_s *b;

    while (a->next)
    {
	b = a->next;
	while (b)
	{
	    int size;

	    ++at;
	    showstatus ((float)at / (float)count);

	    size = MIN (MIN (a->size, b->size), CMPSIZE);
	    if (memcmp (a->buf, b->buf, size) == 0)
	    {
		struct filegroup_s *fg;

		fg = b;
		b = b->next;
		possiblematch (a, fg);
	    } else
		b = b->next;
	}
	a = a->next;
    }
    return;
}

int
main (int argc, char **argv)
{
    DIR *d;
    struct dirent *de;
    int i = 0;
    char buf[BUFSIZ];

    if (argc != 2 || (d = opendir (argv[1])) == NULL)
    {
	printf ("Usage:\n\t %s <dir>\n", argv[0]);
	return -1;
    }

    while (de = readdir (d))
    {
	static struct stat sb;

	sprintf (buf, "%s/%s", argv[1], de->d_name);
	if (stat (buf, &sb) == -1)
	    perror ("stat issue? :");
	else if (sb.st_mode & S_IFREG && sb.st_size)
	    addtolist (buf, &sb);
	if (i % 128)
	    printf ("\rFiles: %d\tInodes: %d", filecount, inodecount);
	++i;
    }
    printf ("\n");
    closedir (d);
    count = inodecount;
    printf ("Dir read completed, %d inodes, ", count);
    count = (int)((float)count * (float)count / 2.0);
    printf ("estimating %d scans\n", count);

    if (filelist && filelist->next)
	compagainst (filelist);
    else
	printf ("uh?\n");
    showstatus (1.0);
    printf ("\n");
    printf ("%d possiblematch calls, %.2f%% scans\n", possiblematchcount,
	100.0 * possiblematchcount / count);
    printf ("%d bytes (%0.2f k, %02.f m) recovered\n", reclaimed,
	(float)reclaimed / 1024.0, (float)reclaimed / (1024.0 * 1024.0));
    return 0;
}
