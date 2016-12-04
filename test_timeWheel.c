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
