/*
 * $Id: test_display.c,v 1.1 2004/02/22 18:23:31 erik Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <check.h>

#include "../display.h"
#include "test_display.h"

START_TEST (test_display1)
{
}
END_TEST

TCase *
test_display (Suite *s)
{
    TCase *tc_display = tcase_create ("display");
    tcase_add_test (tc_display, test_display1);

    return tc_display;
}

