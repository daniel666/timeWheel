#ifndef _TIME_WHEEL_T
#define _TIME_WHEEL_T

#include<stdlib.h>
#include<sys/time.h>
#include "dlist_generic.h"
typedef enum {REPEAT, ONCE} task_type_t;
typedef void (*task_fun_t)(void *);
typedef struct _time_wheel_t{
    int num_slots;
    int cur_index;
    int granularity; //home many usec
    int rotation_cnt;
    
    void ** slots;
} TW;

typedef struct _task_t{
    task_fun_t func;
    void *func_args;
    int first;
    int interval;
    int rotation_cnt;
    task_type_t type;
} task_t;


TW *TW_init(int num_slots, int granularity);
void TW_add_task(TW *tw, int first, int interval, task_type_t ttype, task_fun_t f, void * f_arg );
struct timeval *to_tv(int ms);
void select_sleep(int msec);
void do_task(TW *tw, task_t *task);
void do_tasks(TW *tw, dlist_t *list);
void TW_start(TW *tw);

task_t *task_init(int first, int interval, task_type_t ttype, task_fun_t f, void * f_arg );
#define PREPEND_LIST(lptr, tptr) (prepend_dlist( ((dlist_t *)(lptr)), (void *)(tptr)) )
#endif


