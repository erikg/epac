/*
 * $Id: test_list.c,v 1.3 2004/04/11 14:43:32 erik Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <check.h>

#include "../list.h"
#include "test_list.h"

START_TEST (test_list_add)
{
    struct stat sb;

    filelist = NULL;
    fail_unless (stat ("test_list.c", &sb) == 0,
	"unable to stat \"test_list.c\"");
    addtolist ("test_list.c", &sb);
    fail_unless (filelist != NULL, "filelist is null");
    fail_unless (strcmp ("test_list.c", filelist->files->filename) == 0,
	"moo not added!");
}
END_TEST

START_TEST (test_list_length)
{
    struct stat sb;

    filelist = NULL;
    fail_unless (listlength (filelist) == 0, "length should be 0");
    fail_unless (stat ("test_list.c", &sb) == 0,
	"unable to stat \"test_list.c\"");
    addtolist ("test_list.c", &sb);
    fail_unless (stat ("test_list.h", &sb) == 0,
	"unable to stat \"test_list.h\"");
    addtolist ("test_list.h", &sb);
    fail_unless (stat ("test_epac", &sb) == 0, "unable to stat \"test_epac\"");
    addtolist ("test_epac", &sb);
    fail_unless (filelist != NULL, "filelist is null");
    fail_unless (listlength (filelist) == 3, "length should be 3");
    fail_unless (stat ("test_epac", &sb) == 0, "unable to stat \"test_epac\"");
    addtolist ("test_epac", &sb);
    fail_unless (listlength (filelist) == 3, "length should be 3");
}
END_TEST

START_TEST (test_list_search)
{
    struct stat sb[3];
    struct filegroup_s *d;
    char buf[BUFSIZ];

    filelist = NULL;
    fail_unless (stat ("test_list.c", sb) == 0,
	"unable to stat \"test_list.c\"");
    addtolist ("test_list.c", sb);

    fail_unless (stat ("test_list.h", sb + 1) == 0,
	"unable to stat \"test_list.h\"");
    addtolist ("test_list.h", sb + 1);

    fail_unless (stat ("test_epac", sb + 2) == 0,
	"unable to stat \"test_epac\"");
    addtolist ("test_epac", sb + 2);

    fail_unless (filelist != NULL, "filelist is null");

    d = searchlist (sb[0].st_ino, filelist);
    snprintf (buf, BUFSIZ, "expected \"test_list.c\", but got \"%s\"\n",
	d->files->filename);
    fail_unless (strcmp ("test_list.c", d->files->filename) == 0, buf);
    printf("\n%d\n", d->inode);

    d = searchlist (sb[1].st_ino, filelist);
    snprintf (buf, BUFSIZ, "expected \"test_list.h\", but got \"%s\"\n",
	d->files->filename);
    fail_unless (strcmp ("test_list.h", d->files->filename) == 0, buf);
    printf("\n%d\n", d->inode);

    d = searchlist (sb[2].st_ino, filelist);
    snprintf (buf, BUFSIZ, "expected \"test_epac\", but got \"%s\"\n",
	d->files->filename);
    fail_unless (strcmp ("test_epac", d->files->filename) == 0, buf);
    printf("\n%d\n", d->inode);
}
END_TEST

TCase *
test_list (Suite *s)
{
    TCase *tc_list = tcase_create ("list");
    tcase_add_test (tc_list, test_list_add);
    tcase_add_test (tc_list, test_list_length);
    tcase_add_test (tc_list, test_list_search);

    return tc_list;
}

