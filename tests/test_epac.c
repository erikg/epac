/*
 * $Id: test_epac.c,v 1.2 2004/04/11 15:06:24 erik Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <check.h>

#include "epac.h"
#include "test_epac.h"

START_TEST (test_epac1)
{
}
END_TEST

TCase *
test_epac (Suite *s)
{
    TCase *tc_epac = tcase_create ("epac");
    tcase_add_test (tc_epac, test_epac1);

    return tc_epac;
}

