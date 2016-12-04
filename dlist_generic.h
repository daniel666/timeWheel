#ifndef _DLIST_GENERIC_H
#include<stdbool.h>
#define _DLIST_GENERIC_H

typedef enum {NORMAL, REVERSE} direction_t;

typedef void (*free_func_t)(void *) ;
typedef void (*print_func_t)(void *) ;

typedef struct _node_t{
  void *val;
  struct _node_t *prev;
  struct _node_t *next;
} node_t;


typedef struct{
  node_t *head;
  node_t *tail;
  int len;
  free_func_t freefun;
  print_func_t pfun;
} dlist_t;


typedef struct _dlist_itr{
  node_t *next;
  direction_t dir;
  node_t *end;
} dlist_itr_t;


dlist_t *init_dlist(free_func_t freefunc, print_func_t pfunc);
void prepend_dlist(dlist_t *list, void *data);
void append_dlist(dlist_t *list, void *data);
void delete_dlist_node(dlist_t *list, node_t *node);
void delete_head_dlist(dlist_t *list);
void delete_tail_dlist(dlist_t *list);
void show_dlist(dlist_t *list, direction_t dir);
void destroy_dlist(dlist_t *list);


dlist_itr_t* get_itr_dlist(dlist_t *list, direction_t dir);
bool has_next(dlist_itr_t *itr);
void *get_next(dlist_itr_t *itr);
node_t *get_next_node(dlist_itr_t *itr);
#endif
