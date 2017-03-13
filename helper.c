// standard
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include <pthread.h>
#include <mach/mach_init.h>
#include <mach/thread_policy.h>
#include <mach/clock.h>
#include <mach/mach.h>
#include <mach/mach_time.h>
#include <sched.h>


// ===================================================================================================================================================
// ===================================================================================================================================================
// FUNCTIONALITY
// ===================================================================================================================================================
// ===================================================================================================================================================

const char* getfield(char* line, int num)
{
    const char* tok;
    for (tok = strtok(line, ";");
            tok && *tok;
            tok = strtok(NULL, ";\n"))
    {
        if (!--num)
            return tok;
    }
    return NULL;
}

unsigned int randr(unsigned int min, unsigned int max)
{
	double scaled = (double)rand()/RAND_MAX;
	return (max - min +1)*scaled + min;
}

void nanowait(long total_time, mach_timespec_t* _start, mach_timespec_t* _end){
	int pure_process_time_used_sec;
	int pure_process_time_used_nsec;
	long pure_process_time_used, time_to_wait;
	struct timespec wait, wait2;

	// time to wait to stay in total_time
	pure_process_time_used_nsec = _end->tv_nsec - _start->tv_nsec;
	
	if(pure_process_time_used_nsec < 0){
		pure_process_time_used = 1000000000L + (_end->tv_nsec - _start->tv_nsec);
	} else {
		pure_process_time_used = _end->tv_nsec - _start->tv_nsec;
	}

	time_to_wait = total_time - pure_process_time_used;
	wait.tv_sec = 0;
	wait.tv_nsec = time_to_wait;

	// sleep
	nanosleep(&wait, &wait2);
}