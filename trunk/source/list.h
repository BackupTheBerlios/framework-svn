/* Interface de linked list*/

#ifndef LIST_H
#define LIST_H

#include <stdlib.h>

typedef struct Edge_ {
//int		v1;
int 		v2;
float		weight;
} Edge;

typedef struct ListElmt_ {
	Edge			   *data;
	struct ListElmt_   *next;
} ListElmt;

typedef struct List_ {
int                size;
int                (*match)(const void *key1, const void *key2);
ListElmt           *head;
ListElmt           *tail;
} List;

//void list_init(List *list, void (*destroy)(void *data));
void list_init(List *list);

void list_destroy(List *list);

int get_nth_table_item(List * list, int n);

void list_ins_next(List *list, ListElmt *element, Edge *data);

void list_rem_next(List *list, ListElmt *element);

#define list_size(list) ((list)->size)

#define list_head(list) ((list)->head)

#define list_tail(list) ((list)->tail)

#define list_is_head(list, element) ((element) == (list)->head ? 1 : 0)

#define list_is_tail(element) ((element)->next == NULL ? 1 : 0)

#define list_data(element) ((element)->data)

#define list_next(element) ((element)->next)

#endif
