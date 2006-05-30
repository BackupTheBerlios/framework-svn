/*
Implementacion de linked list
*/

#include <stdlib.h>
#include <string.h>
#include "list.h"

/*--------------------------------------------*/

void
list_init (List * list)
{
  list->size = 0;
  //list->destroy = destroy;
  list->head = NULL;
  list->tail = NULL;
  return;
}

/*----------------------------------------------*/

void
list_destroy (List * list)
{
  while (list->size > 0)
    {
      list_rem_next (list, NULL);
    } 
	memset (list, 0, sizeof (List));
	return;
}

int get_nth_table_item(List * list, int n)
{
	ListElmt *e = list->head;
	if (e == NULL)
		throw ("Invalid list element request!");
	for (int i = 0; i < n; i++)
	{
		e = e->next;
		if (e== NULL)
			throw ("Invalid list element request!");
	}
	return e->data->v2;
}
/*-------------------------------------------------*/

void list_ins_next (List * list, ListElmt * element, Edge *data)
{
  ListElmt *new_element;

  if ((new_element = (ListElmt *) malloc (sizeof (ListElmt))) == NULL)
    throw ("Could not allocate memory for list of links");
  
  new_element->data = data;
  if (element == NULL)
    {
      if (list_size(list) == 0)
		list->tail = new_element;
      new_element->next = list->head;
      list->head = new_element;
    }
  else
    {
      if (element->next == NULL)
		list->tail = new_element;
      new_element->next = element->next;
      element->next = new_element;
    }
  list->size++;
}


/*-------------------------------------------------*/

void list_rem_next (List * list, ListElmt * element)
{
  ListElmt *old_element;
  void *data;

  if (list_size (list) == 0)
    throw ("Element does not exists");
  if (element == NULL)
    {
      data = list->head->data;
      old_element = list->head;
      list->head = list->head->next;
      if (list_size (list) == 1)
		 list->tail = NULL;
    }
  else
    {
      if (element->next == NULL)
		throw ("Element does not exists");
      data = element->next->data;
      old_element = element->next;
      element->next = element->next->next;
      if (element->next == NULL)
	  list->tail = element;
    }
  free (data);
  free (old_element);
  list->size--;
}
