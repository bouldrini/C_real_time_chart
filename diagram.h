#ifndef _DIAGRAM_H
	#define _DIAGRAM_H
	
	// Standard
#include <stdbool.h>
	#include <stdint.h>
	#include <stdio.h>
	#include <time.h>
	#include <stdlib.h>
	#include "window.h"
	#include "gnuplot_i.h"

	// gtk
	#include <gtk/gtk.h>

	
	#include <pthread.h>
	#include <mach/mach_init.h>
	#include <mach/thread_policy.h>
	#include <mach/clock.h>
	#include <mach/mach.h>
	#include <mach/mach_time.h>
	#include <sched.h>

	struct shiftable_window;

	typedef struct diagram {
		struct shiftable_window* m_window;

		double m_resolution;
		int m_samples_per_frame;
		float m_seconds_per_frame;
		GtkWidget* m_display;
		char* m_current_tone;
	} diagram;

	diagram* diagram_init(struct shiftable_window* _window, double _resolution, int _samples_per_frame, float _seconds_per_frame);
	void diagram_plot(void* _dia);
	
	// void diagram_show(diagram* _dia);
	// void diagram_buttons(diagram* _dia);
	// void diagram_canvas(diagram* _dia);
	// void log(char* message);
#endif