// standard
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include <GLUT/glut.h>

// messurement
#include <time.h>

// chart library
#include "gnuplot_i.h"

// gtk
#include <gtk/gtk.h>

// window
#include "window.h"

// diagram
#include "diagram.h"

// analyser
#include "analyser.h"

// scheduler
#include "scheduler.h"

#include <pthread.h>
#include <mach/mach_init.h>
#include <mach/thread_policy.h>
#include <mach/clock.h>
#include <mach/mach.h>
#include <mach/mach_time.h>
#include <sched.h>

// ===================================================================================================================================================
// ===================================================================================================================================================
// CONFIGURATION
// ===================================================================================================================================================
// ===================================================================================================================================================

// PROGRAM CONFIGURATION
bool with_regeneration = true;							// Regenerate Signal by Frequency
bool enable_simulation_module = true;
bool enable_analyser_module = true;
bool enable_analyser_tone_identification = true;
bool enable_plotting_module = true;

// DETAIL CONFIGURATION
const int kiloherz = 1000;
const int megaherz = kiloherz * 1000;
const int gigaherz = megaherz * 1000;

// WINDOW + ANALYSER CONFIGURATION
const int Fs = 65536 / 8;            			// MP3 Sampling frequency
// const int Fs = 100 * kiloherz;            			// MAX Sampling frequency
const long L = (long)Fs;	        					// MP3 Windowrange pow(2, 16)
// const long L = 131072;	        					// MAX Windowrange
const double T = 1.0/Fs;          						// Sampling period
const float sampling_resolution = Fs/L;					// Sampling Resolution

// SYNTHESIZER CONFIGURATION
const float synthesizer_start_frequency = 1;
const float synthesizer_phase = 0;
const float synthesizer_amplitude = 500;

const float diagram_seconds_per_frame = 1;
const int diagram_samples_per_frame = 2000;
const double diagram_resolution = diagram_seconds_per_frame / diagram_samples_per_frame;

// ===================================================================================================================================================
// ===================================================================================================================================================
// MAIN
// ===================================================================================================================================================
// ===================================================================================================================================================

int main(){
	// generator_generate_signal(30,0.0,5, Fs, T, L, sampling_resolution);
	shiftable_window* window;
	analyser* analyser;
	diagram* dia;
	gnuplot_ctrl* h1;
	scheduler* schedule;
	synthesizer* synthi;

	window = window_init(L, h1);
	dia = diagram_init(window, diagram_resolution, diagram_samples_per_frame, diagram_seconds_per_frame);
	analyser = analyser_init(window, sampling_resolution, enable_analyser_tone_identification);
	synthi = synthesizer_init(window, synthesizer_start_frequency, synthesizer_phase, synthesizer_amplitude, Fs, T, L, sampling_resolution);
	
	schedule = scheduler_init(window, synthi, analyser, dia, with_regeneration, enable_simulation_module, enable_analyser_module, enable_analyser_tone_identification, enable_plotting_module);
	scheduler_start(schedule);

	while(1==1){};
	return 1;
}
