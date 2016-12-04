#include<stdlib.h>
#include<stdio.h>
#include "dlist_generic.h"

/* --------------------------------------------------------------------------*/
/**
 * @Synopsis
 *      create dlist_t instance
 *
 * @Param freefunc
 *      function used to free value inside node
 * @Param pfunc
 *      function used to print value inside node
 *
 * @Returns   
 *      dlist_t instance
 */
/* ----------------------------------------------------------------------------*/
dlist_t *init_dlist(free_func_t freefunc, print_func_t pfunc){
    dlist_t *list = malloc(sizeof(*list));

    /*two dummy node*/
    list->head = malloc(sizeof( *(list->head)));
    list->tail = malloc(sizeof( *(list->tail)));
    list->head->next = list->tail;
    list->head->prev = NULL;
    list->tail->prev = list->head;
    list->tail->next = NULL;
    list->len = 0;
    list->freefun = freefunc;
    list->pfun = pfunc;

    return list;
}


void prepend_dlist(dlist_t *list, void *data){
    node_t *nnode = malloc(sizeof(*nnode));
    node_t *dummy_head = list->head;
    nnode->val = data;
    nnode->next = dummy_head->next;
    dummy_head->next->prev = nnode;
    dummy_head->next = nnode;
    nnode->prev = dummy_head;
    list->len += 1;
}

void append_dlist(dlist_t *list, void *data){
    node_t *nnode = malloc(sizeof(*nnode));
    node_t *dummy_tail = list->tail;

    nnode->val = data;
    nnode->next = dummy_tail;
    nnode->prev = dummy_tail->prev;

    dummy_tail->prev->next = nnode;
    dummy_tail->prev = nnode;

    list->len += 1;
}


void delete_dlist_node(dlist_t *list, node_t *node){
    node->prev->next = node->next;
    node->next->prev = node->prev;
    if(list->freefun)
        list->freefun(node->val);
    free(node);
}

/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  
 *      print the list
 *
 * @Param list
 * @Param dir
 *      forward/backward order
 */
/* ----------------------------------------------------------------------------*/
void show_dlist(dlist_t *list, direction_t dir){
    print_func_t pfun = list->pfun;
    dlist_itr_t *itr = get_itr_dlist(list, dir);
    while(has_next(itr)){
        void *data = get_next(itr);
        pfun(data);
        if(has_next(itr)) printf("-->");
    }
    printf("\n");
}

/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  
 *      free the list and each node in it with its value
 *
 * @Param list
 *      the list to be freed
 */
/* ----------------------------------------------------------------------------*/
void destroy_dlist(dlist_t *list){
    free_func_t freefun = list->freefun;
    dlist_itr_t *itr = get_itr_dlist(list, NORMAL );
    while(has_next(itr)){
        node_t *curnode = get_next_node(itr);
        if( freefun )
            freefun(curnode->val);
        free(curnode);
    }
    free(itr);
    free(list->head);
    free(list->tail);
    free(list);
}



/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  
 *      get iterator for list
 *
 * @Param list
 *      list
 * @Param dir
 *      forward/backward traversal
 *
 * @Returns   
 *      list iterator
 */
/* ----------------------------------------------------------------------------*/
dlist_itr_t *get_itr_dlist(dlist_t *list, direction_t dir){
    dlist_itr_t *itr = malloc(sizeof(*itr));
    if (dir == NORMAL)
        itr->next = list->head->next;
    else
        itr->next = list->tail->prev;
    itr->dir = dir;
    itr->end = (dir==NORMAL)? list->tail : list->head;
    return itr;
}

bool has_next(dlist_itr_t *itr){
    return itr->next != itr->end;
}

/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  
 *      get value of current iterator pointed node, and advance node to the
 *      next/prev node according to traversing direction
 *
 * @Param itr
 *      iterator
 *
 * @Returns   
 *      value of the node
 */
/* ----------------------------------------------------------------------------*/
void* get_next(dlist_itr_t *itr){
    void *data = itr->next->val;
    if (itr->dir == NORMAL)
        itr->next = itr->next->next;
    else
        itr->next = itr->next->prev;
    return data;
}


/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  
 *      get current iterator pointed node, and advance node to the
 *      next/prev node according to traversing direction
 *
 * @Param itr
 *
 * @Returns   
 *      list node
 */
/* ----------------------------------------------------------------------------*/
node_t* get_next_node(dlist_itr_t *itr){
    node_t *node = itr->next;
    if (itr->dir == NORMAL)
        itr->next = itr->next->next;
    else
        itr->next = itr->next->prev;
    return node;
}

