/*
 * $Id: test_comp.c,v 1.2 2004/04/11 15:06:24 erik Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <check.h>

#include "comp.h"
#include "test_comp.h"

START_TEST (test_comp1)
{
}
END_TEST

TCase *
test_comp (Suite *s)
{
    TCase *tc_comp = tcase_create ("comp");
    tcase_add_test (tc_comp, test_comp1);

    return tc_comp;
}

