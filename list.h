/*
 * $Id: list.h,v 1.1 2002/12/14 19:10:20 erik Exp $
 */

#ifndef __LIST_H_
#define __LIST_H_

typedef struct list_node_s
{
    void *car;
    struct list_node_s *cdr;
} list_node;

#define car(x) ((x)->car)
#define cdr(x) ((x)->cdr)

#define cddr(x) (cdr(cdr(x)))
#define cdddr(x) (cdr(cddr(x)))

#define cadr(x) (car(cdr(x)))
#define caddr(x) (car(cddr(x)))
#define cadddr(x) (car(cddddr(x)))
#define cadar(x) (cadr(car(x)))

list_node *cons (void *d, list_node * l);
int list_length (list_node * l);
list_node *list_last (list_node * l);
list_node *list_append (list_node * a, list_node * b);
void list_foreach (void (*func) (void *), list_node * l);
list_node *list_map (list_node * (*func) (void *), list_node * l);

#endif
