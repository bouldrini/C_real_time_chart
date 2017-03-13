#include "diagram.h"

// ===================================================================================================================================================
// ===================================================================================================================================================
// FUNCTIONALITY
// ===================================================================================================================================================
// ===================================================================================================================================================

diagram* diagram_init(struct shiftable_window* _window, double _resolution, int _samples_per_frame, float _seconds_per_frame){
	diagram* dia = (diagram*)malloc(sizeof(diagram));
	dia->m_window = _window;
	dia->m_resolution = _resolution;
	dia->m_samples_per_frame = _samples_per_frame;
	dia->m_seconds_per_frame = _seconds_per_frame;
	dia->m_current_tone = "C";
	return dia;
};

void diagram_plot(void* _dia){
	diagram* dia = (diagram*) _dia;
	shiftable_window* window = dia->m_window;
    gnuplot_setstyle(window->m_gnuplot_ctrl, "lines");
    gnuplot_cmd(window->m_gnuplot_ctrl, "set yrange[-1000:1000]");
    // gnuplot_cmd(window->m_gnuplot_ctrl, "set size 3,1");

	float* signal_representation = (float*) malloc(dia->m_samples_per_frame * window->m_element_size);

	mach_timespec_t process_start, total_end, process_end;
	clock_serv_t cclock;

	long total_time = 250000000L;
	// long total_time = 31250000L;

	int pure_process_time_used_sec, total_time_used_sec;
	long pure_process_time_used, time_to_wait, total_time_used;

	struct timespec wait, wait2;

	double frequency;

	host_get_clock_service(mach_host_self(), SYSTEM_CLOCK, &cclock);


	while(1==1){
		clock_get_time(cclock, &process_start);
		
		window_get_for_resolution(window, signal_representation, dia->m_resolution);
		gnuplot_resetplot(window->m_gnuplot_ctrl);
		gnuplot_plot_x(window->m_gnuplot_ctrl, signal_representation, dia->m_samples_per_frame, "Signal");

		clock_get_time(cclock, &process_end);

		// time to wait to stay in total_time
		pure_process_time_used_sec = process_end.tv_sec - process_start.tv_sec;
		
		if(pure_process_time_used_sec < 0){
			pure_process_time_used = 1000000000L + (process_end.tv_nsec - process_start.tv_nsec);
		} else {
			pure_process_time_used = process_end.tv_nsec - process_start.tv_nsec;
		}

		time_to_wait = total_time - pure_process_time_used;
	   
		wait.tv_sec = 0;
		wait.tv_nsec = time_to_wait;

		// sleep
		nanosleep(&wait, &wait2);

		// take total end time
		clock_get_time(cclock, &total_end);

		total_time_used_sec = total_end.tv_sec - process_start.tv_sec;
		total_time_used = total_end.tv_nsec - process_start.tv_nsec;

		if(total_time_used < 0){
			total_time_used = 1000000000L + total_time_used;
		}
		frequency = (1/(total_time_used/1000.0/1000.0/1000.0));

		// printf("Plotting took %fs (nanosekunden genau) -> max %f times per sec\n\n", (total_time_used/1000.0/1000.0/1000.0), frequency);
	}
	
};