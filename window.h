
#ifndef _WINDOW_H
	#define _WINDOW_H

	#include <stdbool.h>
	#include <stdint.h>
	#include <stdio.h>
	#include <time.h>
	#include <stdlib.h>
	#include <math.h>
	#include "gnuplot_i.h"

	#include "diagram.h"
	#include "./extern/zx_fft.h"

	typedef struct shiftable_window
	{
	    float* m_buffer;
	    float* m_buffer_end;

	    float* m_data_start;
	    float* m_data_end;

	    int m_count;

	    clock_t m_start_time;
	    clock_t m_current_time;
	    int m_seconds;

	    long m_capacity;
	    int m_element_size;
	    bool m_dirty;

	    float m_currently_highest_value;

	    gnuplot_ctrl* m_gnuplot_ctrl;
	    gnuplot_ctrl* m_fft_gnuplot_ctrl;

	} shiftable_window;

	struct diagram;

	// init
	shiftable_window* window_init(long _capacity, gnuplot_ctrl* _h1);
	void window_free(shiftable_window* _window);
	
	// push
	void window_push(shiftable_window* _window, float _data);
	void window_push_with_delay(shiftable_window* _window, float _data, long _execution_time_in_ns);

	// get
	float* window_get_for_diagram(shiftable_window* _window, float* _result_window, struct diagram* _diagram);
	bool window_get_for_resolution(shiftable_window* _window, float* _result_window, float _resolution);
	float* window_get(shiftable_window* _window);
	float* window_get_mem(shiftable_window* _window);
	COMPLEX* window_get_as_complex(shiftable_window* _window, COMPLEX* _result_window);

	// PRINT
	// in order
	void window_print(shiftable_window* _window);
	void window_print_for_resolution(shiftable_window* _window, float _resolution);

	// raw
	void window_print_full(shiftable_window* _window);
	void window_print_in_parts(shiftable_window* _window);
	void window_print_representation(shiftable_window* _window);
	void window_print_memory_extension(shiftable_window* _window);

	// configuration
	void window_print_infos(shiftable_window* _window);
#endif