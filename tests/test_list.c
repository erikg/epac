/*
 * $Id: test_list.c,v 1.1 2004/02/22 18:23:31 erik Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <check.h>

#include "../list.h"
#include "test_list.h"

START_TEST (test_list1)
{
}
END_TEST

TCase *
test_list (Suite *s)
{
    TCase *tc_list = tcase_create ("list");
    tcase_add_test (tc_list, test_list1);

    return tc_list;
}

