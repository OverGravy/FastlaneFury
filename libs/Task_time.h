#ifndef __LIBS_TASKTIME_H__
#define __LIBS_TASKTIME_H__

#include <time.h>

// TIME MENAGMENT FUNCTION

// function that copy time from one struct to another
void time_copy(struct timespec *td, struct timespec ts);

// function that compares two times
int time_cmp(struct timespec td, struct timespec ts);

// function that adds time to a struct
void time_add_ms(struct timespec *t, int ms);

// funtion that gets the current time
long get_sys_time(int unit);



#endif