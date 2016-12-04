# timeWheel

A simple C implementation of timeWheel 

![alt tag](http://www.cubrid.org/files/attach/images/220547/531/683/operation_of_simple_timingwheel.png)

A timeWheel is a structure that holds multiple queues of tasks that should be fired when their specified time expire. It consists of slots, which are implemented as an array in a round-robin fashion, and queues in each slot, which hold tasks (call-back functions).

The timewheel advance its slot per granularity milliseconds, and will return to slot 0 when it goes beyond the maximal slot. 
At each slot, the timewheel fire up tasks stored in that slot, (i.e., call their respective callback function), remove them from the queue, and add repeated tasks to their specific slot for the next occurence (according to their interval). Tasks that repeat only for once are removed permanently. 

the queue is implemented as a doubly linked list in dlist_generic.h/c and the timewheel is implemented in timeWheel.c/h. 
A simple test program is provided in test_timeWheel.c

To compile the program.
     gcc -o test_timeWheel test_timeWheel.c timeWheel.c dlist_generic.c
to run the program
    ./test_timeWheel
