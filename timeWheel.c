#include<stdio.h>
#include<string.h>
#include "timeWheel.h"


/* --------------------------------------------------------------------------*/
/**
 * @Synopsis
 *      create a timewheel with num_slots and each successive slots being
 *      granularity milliseconds
 *
 * @Param num_slots
 *      number of slots/wheels
 * @Param granularity
 *      duration between slots (in milliseconds)
 *
 * @Returns   
 *      the timewheel instance
 */
/* ----------------------------------------------------------------------------*/
TW *TW_init(int num_slots, int granularity){
    TW *tw = malloc(sizeof(*tw));
    tw->num_slots = num_slots;
    tw->cur_index = 0;
    tw->rotation_cnt = 0;
    tw->granularity = granularity;
    tw->slots = calloc(num_slots, sizeof(void *));
    return tw;
}


/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  
 *      create a task
 *
 * @Param first
 *      first time the tsk should be fired
 * @Param interval
 *      interval the task happen
 * @Param ttype
 *      repeat/once
 * @Param f
 *      call back function
 * @Param f_arg
 *      call back function type
 *
 * @Returns   
 *      the task instance
 */
/* ----------------------------------------------------------------------------*/
task_t *task_init(int first, int interval, task_type_t ttype, task_fun_t f, void * f_arg ){
    task_t *task = malloc(sizeof(*task));
    task->func = f;
    task->func_args = f_arg;
    task->first = first;
    task->interval = interval;
    task->type = ttype;

    return task;
}

void task_free(void *task){
    task_t *tptr = (task_t *)task;
    free(task);
}

/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  
 *
 *      Add task to a timewheel
 *
 * @Param tw: the timewheel instance
 * @Param first: first time (in ms) the task should be fired 
 * @Param interval: successive times (in ms) the task should be fired 
 * @Param ttype: task type. can be REPEAT/ONCE
 * @Param f: the task call back function
 * @Param f_arg: the task call back function argument
 */
/* ----------------------------------------------------------------------------*/

void TW_add_task(TW *tw, int first, int interval, task_type_t ttype, task_fun_t f, void * f_arg ){
    task_t *task = NULL;
    int nidx = 0;
    int nround = 0;
    int nrround_offset = 0;

    task = task_init(first, interval, ttype, f, f_arg);
    nidx = (tw->cur_index + first/tw->granularity) % tw->num_slots;
    nrround_offset = (tw->cur_index + first/tw->granularity) / tw->num_slots;
    nround = tw->rotation_cnt + nrround_offset;
    task->rotation_cnt = nround;

    if(tw->slots[nidx] ==   NULL){
        tw->slots[nidx] = init_dlist( &task_free, NULL);
    }
    PREPEND_LIST(tw->slots[nidx], task);
    printf("add task %d to slo %d for roudn %d\n", *((int *) f_arg), nidx, nround);
    return ;
}


/* --------------------------------------------------------------------------*/
/**
 * @Synopsis
 *      convert milliseconds to timeval struct
 *
 * @Param ms
 *      milliseconds
 *
 * @Returns   
 *      timeval struct containing seconds and microseconds
 */
/* ----------------------------------------------------------------------------*/
struct timeval *to_tv(int ms){
    struct timeval *tv = NULL;

    tv = malloc(sizeof(*tv));
    tv->tv_sec = ms / 1000;
    tv->tv_usec = ms%1000 * 1000;
    return tv;
}

/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  
 *      sleep for msec (milliseconds)
 *
 * @Param msec
 *      sleep time
 */
/* ----------------------------------------------------------------------------*/
void select_sleep(int msec){
    struct timeval *tv = to_tv(msec);
    while(1){
        if( (tv->tv_sec == 0) && (tv->tv_usec == 0))
            break;
        select(0, NULL, NULL, NULL, tv);
    }
    free(tv);
}

/* --------------------------------------------------------------------------*/
/**
 * @Synopsis  
 *      fire up the task call back function
 *
 * @Param tw
 *      the timewheel
 * @Param task
 *      the task
 */
/* ----------------------------------------------------------------------------*/
void do_task(TW *tw, task_t *task){
    task->func(task->func_args);
}

/* --------------------------------------------------------------------------*/
/**
 * @Synopsis 
 *      Do tasks that expire. The tasks are contained in the linkedlist 
 *
 * @Param tw
 *      the timerwheel instance
 * @Param list
 *      the list containing expiry tasks that should be called
 */
/* ----------------------------------------------------------------------------*/
void do_tasks(TW *tw, dlist_t *list){
    dlist_itr_t *itr = get_itr_dlist(list, NORMAL);
    dlist_t *repeat_tasks = init_dlist(&task_free, NULL);
    node_t *curnode = NULL;
    while(has_next(itr)){
        curnode = get_next_node(itr);
        task_t *task =  (task_t *) curnode->val;
        if(task->rotation_cnt == tw->rotation_cnt){
            do_task(tw, task);
            if(task->type == REPEAT){
                task_t *ntask = malloc(sizeof(*ntask));
                memcpy(ntask, task, sizeof(*task));
                PREPEND_LIST(repeat_tasks, ntask);
            }
            delete_dlist_node(list, curnode);
        }
    }
    free(itr);

    dlist_itr_t *list_itr = get_itr_dlist(repeat_tasks, NORMAL);
    while(has_next(list_itr)){
        task_t *task = (task_t *)get_next(list_itr);
        TW_add_task(tw, task->interval, task->interval, task->type, task->func, task->func_args);
    }
    destroy_dlist(repeat_tasks);
    free(list_itr);
}


/* --------------------------------------------------------------------------*/
/**
 * @Synopsis
 *      Start the timerwheel
 *
 * @Param tw
 *      timerwheel instance
 */
/* ----------------------------------------------------------------------------*/
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


