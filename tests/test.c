/*
 * $Id: test.c,v 1.1 2004/04/11 14:34:59 erik Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <check.h>

#include "test_comp.h"
#include "test_display.h"
#include "test_epac.h"
#include "test_list.h"

Suite *
epac_suite (void)
{
    Suite *s = suite_create ("epac");

    suite_add_tcase (s, test_comp(s));
    suite_add_tcase (s, test_display(s));
    suite_add_tcase (s, test_epac(s));
    suite_add_tcase (s, test_list(s));
    return s;
}

int
main (void)
{
    int nf;
    Suite *s = epac_suite ();
    SRunner *sr = srunner_create (s);

    srunner_set_log (sr, "test.log");
    srunner_run_all (sr, CK_NORMAL);
    nf = srunner_ntests_failed (sr);
    srunner_free (sr);
    suite_free (s);
    return (nf == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
