#ifndef _analyser_H
	#define _analyser_H
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

	// gtk
	#include <gtk/gtk.h>

	// Shiftable Window
	#include "window.h"

	// Fourier Transformation
	#include "./extern/zx_fft.h" 

	typedef struct analyser{
		shiftable_window* m_window;

		bool m_tone_identification;
		char* m_current_tone;
		float m_sampling_resolution;

	} analyser;

	// init
	analyser* analyser_init(shiftable_window* _window, float _sampling_resolution, bool _tone_identification);

	// start
	void* analyser_analyse(void* _analyser);

	// internal
	void analyser_transform(analyser* analyser, float _resolution);
	void analyser_as_frequency_spectrum(analyser* analyser, float* _frequency_spectrum, COMPLEX* _complex_window);
	const char* analyser_match_tone(int _steps);

	// print
	void analyser_print_tone_matching(analyser* analyser, float* _spectrum, float _resolution);
	const char* analyser_tone_identification(analyser* analyser, float* _spectrum, float _resolution);
	
	// helpter
	int analyser_get_semitone_steps_from_baseline_for_frequency(double _frequecy);
	double analyser_get_frequency_from_semitone_steps_from_baseline(int _steps);

#endif