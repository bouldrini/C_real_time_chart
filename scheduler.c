#include "scheduler.h"

// ===================================================================================================================================================
// ===================================================================================================================================================
// CONFIGURATION
// ===================================================================================================================================================
// ===================================================================================================================================================

const uint64_t NANOS_PER_MSEC = 1000000ULL;
static const uint64_t NANOS_PER_USEC = 1000ULL;
static const uint64_t NANOS_PER_MILLISEC = 1000ULL * NANOS_PER_USEC;
static const uint64_t NANOS_PER_SEC = 1000ULL * NANOS_PER_MILLISEC;
int clockspersecond = CLOCKS_PER_SEC;

// ===================================================================================================================================================
// ===================================================================================================================================================
// FUNCTIONALITY
// ===================================================================================================================================================
// ===================================================================================================================================================

scheduler* scheduler_init(shiftable_window* _window, synthesizer* _synthi, analyser* _analyser, diagram* _diagram, bool _with_regeneration, bool _enable_simulation_module, bool _enable_analyser_module, bool _enable_analyser_tone_identification, bool _enable_plotting_module){
	scheduler* sched = (scheduler*) malloc(sizeof(scheduler));
	sched->m_window = _window;
	sched->m_synthi = _synthi;
	sched->m_analyser = _analyser;
	sched->m_diagram = _diagram;

	sched->m_with_regeneration = _with_regeneration;
	sched->m_enable_simulation_module = _enable_simulation_module;
	sched->m_enable_analyser_module = _enable_analyser_module;
	sched->m_enable_analyser_tone_identification = _enable_analyser_tone_identification;
	sched->m_enable_plotting_module = _enable_plotting_module;
	return sched;
};

void move_pthread_to_realtime_scheduling_class(pthread_t pthread)
{
    mach_timebase_info_data_t timebase_info;
    mach_timebase_info(&timebase_info);
 
    double clock2abs = ((double)timebase_info.denom / (double)timebase_info.numer) * NANOS_PER_MSEC;
 	clockspersecond = (uint32_t)(5 * clock2abs);

    thread_time_constraint_policy_data_t policy;
    policy.period      = 0;
    policy.computation = (uint32_t)(50 * clock2abs); // 10 ms of work
    policy.constraint  = (uint32_t)(90 * clock2abs);
    policy.preemptible = FALSE;
 
    int kr = thread_policy_set(pthread_mach_thread_np(pthread_self()), THREAD_TIME_CONSTRAINT_POLICY, (thread_policy_t)&policy, THREAD_TIME_CONSTRAINT_POLICY_COUNT);

    if (kr != KERN_SUCCESS){
        mach_error("thread_policy_set:", kr);
        exit(1);
    }
}

void scheduler_start(scheduler* _scheduler)
{
	if(_scheduler->m_enable_simulation_module)
	{
		pthread_t reader;
		pthread_create(&reader, NULL, synthesizer_simulate_by_csv, _scheduler->m_synthi);
		move_pthread_to_realtime_scheduling_class(reader);
		pthread_detach(reader);
	}

	if(_scheduler->m_enable_analyser_module)
	{
		pthread_t analyser_module;
		pthread_create(&analyser_module, NULL, analyser_analyse, _scheduler->m_analyser);
		move_pthread_to_realtime_scheduling_class(analyser_module);
		pthread_detach(analyser_module);
	}

	if(_scheduler->m_enable_plotting_module)
	{
		pthread_t plotting_module;
		pthread_create(&plotting_module, NULL, diagram_plot, _scheduler->m_diagram);
		move_pthread_to_realtime_scheduling_class(plotting_module);
		pthread_detach(plotting_module);
	}
}

// static uint64_t abs_to_nanos(uint64_t abs) {
//     return abs * timebase_info.numer  / timebase_info.denom;
// }
 
// static uint64_t nanos_to_abs(uint64_t nanos) {
//     return nanos * timebase_info.denom / timebase_info.numer;
// }

void messurement_test()
{
	// Messurement
	clock_t start_replot, end_replot, start_sec, end_sec;
	float cpu_time_used;
	
	struct timespec tim, tim2;
	tim.tv_sec = 0;
	tim.tv_nsec = 400000000L;

	mach_timespec_t start_regen, end_regen;

	clock_serv_t cclock;
	mach_timespec_t mts;

	host_get_clock_service(mach_host_self(), SYSTEM_CLOCK, &cclock);
	clock_get_time(cclock, &start_regen);
	nanosleep(&tim , &tim2);
	clock_get_time(cclock, &end_regen);

	printf("%d, %d\n", start_regen.tv_nsec, end_regen.tv_nsec);
	int used_time = end_regen.tv_nsec - start_regen.tv_nsec;
	double frequency = (clockspersecond/used_time);
	printf("\n %d ns -> max %f Hz\n", used_time, frequency);

	mach_port_deallocate(mach_task_self(), cclock);
}
