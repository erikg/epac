
/*
 * $Id: list.c,v 1.1 2002/12/14 19:10:20 erik Exp $
 *
 * list.c: (singly) linked list functions 
 * Erik Greenwald <erik@smluc.org>
 */

#include <stdlib.h>
#include "list.h"

int
list_length (list_node * l)
{
    if (l == NULL)
    return 0;
    return 1 + list_length (l->cdr);
}

list_node *
list_cons (void *d, list_node * l)
{
    list_node *n;

    return n = (list_node *) malloc (sizeof (list_node)) ? n->car = d, n->cdr =
    l, n : n;
}

list_node *
list_last (list_node *l)
{
    return l->cdr == NULL ? l : list_last (l->cdr);
}

list_node *
list_append (list_node * a, list_node * b)
{
    return a && b ? list_last (a)->cdr = b, a : a ? a : b;
}

list_node *
list_insert_before (int (*func) (void *a, void *b), void *d, list_node *l)
{
//    return l ?  func (d, cdr (l)) ?  cons (d, l) : l->cdr = (struct list_node_s *)list_insert_before (func, d, l->cdr), l : NULL;
	if(l==NULL)
		return NULL;
	if(func(d,car(l)))
	{
		return cons(d, l);
	}
	l->cdr = list_insert_before(func,d,cdr(l));
	return l;
}

list_node *
list_append_after(int (*func) (void *a, void *b), void *d, list_node *l)
{
    if(l==NULL)return list_cons(d,NULL);
//    if(func(a,car(b))
//        return 
}

void
list_foreach (void (*func) (void *), list_node * l)
{
    l ? func (l->car), list_foreach (func, l->cdr) :0;
    return;
}

list_node *
list_map (list_node * (*func) (void *), list_node * l)
{
    return l == NULL ? NULL :
    list_cons (func (l->car), list_map (func, l->cdr));
}
