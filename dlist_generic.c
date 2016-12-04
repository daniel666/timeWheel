#include<stdlib.h>
#include<stdio.h>
#include "dlist_generic.h"

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

void delete_head_dlist(dlist_t *list){
    ;
}
void delete_tail_dlist(dlist_t *list){
    ;
}
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

void destroy_dlist(dlist_t *list){
    free_func_t freefun = list->freefun;
    dlist_itr_t *itr = get_itr_dlist(list, NORMAL );
    while(has_next(itr)){
        node_t *curnode = itr->next;
        if( freefun )
            freefun(curnode->val);
        free(curnode);
        get_next(itr);
    }
    free(list->head);
    free(list->tail);
    free(list);
}


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

void* get_next(dlist_itr_t *itr){
    void *data = itr->next->val;
    if (itr->dir == NORMAL)
        itr->next = itr->next->next;
    else
        itr->next = itr->next->prev;
    return data;
}

node_t* get_next_node(dlist_itr_t *itr){
    node_t *node = itr->next;
    if (itr->dir == NORMAL)
        itr->next = itr->next->next;
    else
        itr->next = itr->next->prev;
    return node;
}

