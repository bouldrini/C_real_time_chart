#ifndef _synthesizer_H
	#define _synthesizer_H
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

	#include "window.h"
	#include "helper.c"

	typedef struct synthesizer{
		shiftable_window* m_window;
		int m_Fs;
		long m_L;
		double m_T;
		float m_resolution;
		float m_f;
		float m_phase;
		float m_amplitude;
		float m_phi;
	} synthesizer;

	// synthetisation
	synthesizer* synthesizer_init(shiftable_window* _window, float _f, float _phase, float _amplitude, const int _Fs, const double _T, const long _L, const float _resolution);
	double synthesizer_synthesize_cosinus(double t, synthesizer* _synthi);
	double synthesizer_synthesize_sinus(double t, synthesizer* _synthi);
	void synthesizer_synthesize_signal_to_csv(synthesizer* _synthi);

	// simulation
	void* synthesizer_simulate_by_csv(void* _window);
#endif