#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include "timeWheel.h"



void task_call(void *arg) {
    int id =  *((int *) arg);

    time_t rawtime;
    struct tm * timeinfo;

    time( &rawtime );
    timeinfo = localtime( &rawtime );

    printf("task id: %d,\t time:%s\n", id, asctime(timeinfo));
}

int main(){
    /*int n1arr[] = {1,2,3};*/
    /*int n2arr[] = {4,5};*/

    /*this is for singly linked list*/
    /*list_t *list = list_init( &free_arr_node, &print_arr_node);*/
    /*list_add(list, (void *) arr_node_init(n1arr, 3));*/
    /*list_add(list, (void *) arr_node_init(n2arr, 2));*/
    /*list_print(list);*/
    /*list_destroy(list);*/


    /*dlist_t *list = init_dlist( &free_arr_node, &print_arr_node);*/
    /*prepend_dlist(list, (void *) arr_node_init(n1arr, 3));*/
    /*prepend_dlist(list, (void *) arr_node_init(n2arr, 2));*/
    /*show_dlist(list, NORMAL);*/
    /*show_dlist(list, REVERSE);*/
    /*destroy_dlist(list);*/

    TW *tw = NULL;
    int *id = NULL;

    tw = TW_init(10, 1000);

    id = malloc(sizeof(*id));
    *id = 1;
    TW_add_task(tw, 1000, 1000, REPEAT, &task_call, id);


    id = malloc(sizeof(*id));
    *id = 2;
    TW_add_task(tw, 2000, 20000, REPEAT, &task_call, id);
    
    TW_start(tw);

    return 0;
}
