#ifndef _scheduler_H
	#define _scheduler_H
	// Standard Libraries
	#include <stdio.h>
	#include <math.h>
	#include <stdlib.h>
	#include <string.h>
	#include <stdbool.h>

	
	#include <pthread.h>
	#include <mach/mach_init.h>
	#include <mach/thread_policy.h>
	#include <mach/clock.h>
	#include <mach/mach.h>
	#include <mach/mach_time.h>
	#include <sched.h>

	// Shiftable Window
	#include "window.h"

	// Analyser
	#include "analyser.h"

	// Synthesizer
	#include "synthesizer.h"



	typedef struct scheduler{
		shiftable_window* m_window;
		synthesizer* m_synthi;
		analyser* m_analyser;
		diagram* m_diagram;

		bool m_with_regeneration;
		bool m_enable_simulation_module;
		bool m_enable_analyser_module;
		bool m_enable_analyser_tone_identification;
		bool m_enable_plotting_module;
	} scheduler;

	scheduler* scheduler_init(shiftable_window* _window, synthesizer* _synthi, analyser* _analyser, diagram* _diagram, bool _with_regeneration, bool _enable_simulation_module, bool _enable_analyser_module, bool _enable_analyser_tone_identification, bool _enable_plotting_module);
	void scheduler_start(scheduler* _schedule);
	
	void messurement_test();
	void move_pthread_to_realtime_scheduling_class(pthread_t pthread);
	
	// static uint64_t nanos_to_abs(uint64_t nanos);
	// static uint64_t abs_to_nanos(uint64_t abs);

#endif