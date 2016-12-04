#include<stdio.h>
#include "timeWheel.h"

TW *TW_init(int num_slots, int granularity){
    TW *tw = malloc(sizeof(*tw));
    tw->num_slots = num_slots;
    tw->granularity = granularity;
    tw->slots = malloc(sizeof(void *) * num_slots);
    return tw;
}
task_t *task_init(int first, int interval, task_type_t ttype, task_fun_t f, void * f_arg ){
    task_t *task = malloc(sizeof(*task));
    task->func = f;
    task->func_args = f_arg;
    task->first = first;
    task->interval = interval;
    task->type = ttype;

    return task;
}

void TW_add_task(TW *tw, int first, int interval, task_type_t ttype, task_fun_t f, void * f_arg ){
    task_t *task = NULL;

    task = task_init(first, interval, ttype, f, f_arg);
    int nidx = (tw->cur_index + first/tw->granularity) % tw->num_slots;
    int nrround_offset = (tw->cur_index + first/tw->granularity) / tw->num_slots;
    int nround = tw->rotation_cnt + nrround_offset;
    task->rotation_cnt = nround;

    if(tw->slots[nidx] ==   NULL){
        tw->slots[nidx] = init_dlist(NULL, NULL);
    }
    PREPEND_LIST(tw->slots[nidx], task);
    printf("add task %d to slo %d for roudn %d\n", *((int *) f_arg), nidx, nround);
    return ;
}


struct timeval *to_tv(int ms){
    struct timeval *tv = NULL;

    tv = malloc(sizeof(*tv));
    tv->tv_sec = ms / 1000;
    tv->tv_usec = ms%1000 * 1000;
    return tv;
}

void select_sleep(int msec){
    struct timeval *tv = to_tv(msec);
    while(1){
        if( (tv->tv_sec == 0) && (tv->tv_usec == 0))
            break;
        select(0, NULL, NULL, NULL, tv);
    }
    free(tv);
}

void do_task(TW *tw, task_t *task){
    task->func(task->func_args);
}

void do_tasks(TW *tw, dlist_t *list){
    dlist_itr_t *itr = get_itr_dlist(list, NORMAL);
    dlist_t *repeat_tasks = init_dlist(NULL, NULL);
    node_t *curnode = NULL;
    while(has_next(itr)){
        curnode = get_next_node(itr);
        task_t *task =  (task_t *) curnode->val;
        if(task->rotation_cnt == tw->rotation_cnt){
            do_task(tw, task);
            delete_dlist_node(list, curnode);
            if(task->type == REPEAT){
                PREPEND_LIST(repeat_tasks, task);
            }
        }
    }

    dlist_itr_t *list_itr = get_itr_dlist(repeat_tasks, NORMAL);
    while(has_next(list_itr)){
        task_t *task = (task_t *)get_next(list_itr);
        TW_add_task(tw, task->interval, task->interval, task->type, task->func, task->func_args);
    }
    destroy_dlist(repeat_tasks);
    free(list_itr);
    free(itr);
}


void TW_start(TW *tw){
    while(1){
        dlist_t *list = (dlist_t *) tw->slots[tw->cur_index];
        if( list != NULL )
            do_tasks(tw, list);
        if(++tw->cur_index == tw->num_slots){
            tw->cur_index = 0;
            tw->rotation_cnt++;
        }
        select_sleep(tw->granularity);        
    }
}


