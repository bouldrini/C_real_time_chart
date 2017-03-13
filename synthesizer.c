#include "synthesizer.h"

// ===================================================================================================================================================
// ===================================================================================================================================================
// FUNCTIONALITY
// ===================================================================================================================================================
// ===================================================================================================================================================

synthesizer* synthesizer_init(shiftable_window* _window, float _f, float _phase, float _amplitude, const int _Fs, const double _T, const long _L, const float _resolution){
	synthesizer* synthi = (synthesizer*) malloc(sizeof(synthesizer));
	synthi->m_window = _window;
	synthi->m_Fs = _Fs;
	synthi->m_L = _L;
	synthi->m_T = _T;
	synthi->m_resolution = _resolution;
	synthi->m_f = _f;
	synthi->m_phase = _phase;
	synthi->m_phi = synthi->m_phase * M_PI/180.0;
	synthi->m_amplitude = _amplitude;

	synthesizer_synthesize_signal_to_csv(synthi);

	return synthi;
}

double synthesizer_synthesize_cosinus(double t, synthesizer* _synthi){
	return _synthi->m_amplitude * cos(2 * M_PI * _synthi->m_f * t + _synthi->m_phi); // + _synthi->m_amplitude / 2  * cos(2 * M_PI * _synthi->m_f * 2 * t + _synthi->m_phi)  + _synthi->m_amplitude * 2 * (t*10) * cos(2 * M_PI * 1 * t + _synthi->m_phi);
}

double synthesizer_synthesize_sinus(double t, synthesizer* _synthi){
	return _synthi->m_amplitude * sin(2 * M_PI * _synthi->m_f * t + _synthi->m_phi);
}

void synthesizer_synthesize_signal_to_csv(synthesizer* _synthi){
	// printf("\nRegenerating Signal...\n");
	FILE *signalfile = fopen("./signal.csv", "w");
	if (signalfile == NULL) {
		printf("Error opening file!\n");
	}

	fprintf(signalfile, "idx;real;\n");
	for(double i = 0; i < _synthi->m_L/_synthi->m_Fs; i=i+_synthi->m_T){
		fprintf(signalfile, "%d;%f;\n", (int)i, synthesizer_synthesize_cosinus(i, _synthi));
	}
	fclose(signalfile);
	
	// FILE *plotfile = fopen("./signal_for_plot.csv", "w");
	// if (plotfile == NULL) {
	// 	printf("Error opening file!\n");
	// }

	// fprintf(plotfile, "idx,real,\n");
	// for(double i = 0; i < _L/_Fs; i=i+_T){
	// 	fprintf(signalfile, "%d,%f,\n", (int)i, generate_cosinus(i));
	// }
	// fclose(plotfile);
}

void* synthesizer_simulate_by_csv(void* _synthi){
	// Declaration
	synthesizer* synthi = (synthesizer*)_synthi;
	shiftable_window* window = synthi->m_window;
	char line[100];
	char * tmp;
	float value;
	const char* real;
	int i = 0;
	int k = 50;
	int direction = 0;
	double frequency;
	mach_timespec_t process_start, total_end, process_end, read_start, read_end, read_short_start, read_short_end;
	clock_serv_t cclock;
	int total_time_used_sec;
	long total_time_used;
	long total_time = 1000000000L;
	long total_time_per_read = (long)(total_time / (synthi->m_Fs));

	printf("\n\n%ld\n\n", total_time_per_read);

	host_get_clock_service(mach_host_self(), SYSTEM_CLOCK, &cclock);

	// Reading
	long average;
	while(1 == 1){
		clock_get_time(cclock, &process_start);

		// synthi->m_f = synthi->m_f + k;
		// synthesizer_synthesize_signal_to_csv(synthi);

		// Reopen
		FILE *stream = fopen("signal.csv", "r");
		if(stream == NULL){
			perror("Error opening file");
			exit(0);
		}

		// Loop through file
	    while (fgets(line, 100, stream)){
	    	tmp = strdup(line);
	    	// Pushing to window
	        real = getfield(tmp, 2);
	        value = atof(real);
	        window_push_with_delay(window, value, total_time_per_read);
	    }
	    fclose(stream);    

	    if(direction == 0){
	    	k = k + 1000;
	    	if(k > window->m_capacity/2){
	    		direction = 1;
	    	}
	    } else {
		    k = k - 1000;
		    if(k < 20){
	    		direction = 0;
	    	}
	    }
	    
	    // wait
	    // clock_get_time(cclock, &process_end);
	    // nanowait(total_time, &process_start, &process_end);
		// clock_get_time(cclock, &total_end);

		// output calculations
		// total_time_used_sec = total_end.tv_sec - process_start.tv_sec;
		// total_time_used = total_end.tv_nsec - process_start.tv_nsec;
		// if(total_time_used < 0){
		// 	total_time_used = 1000000000L + total_time_used;
		// }
		// frequency = (1/(total_time_used/1000.0/1000.0/1000.0));

		// printf("READ took %d + %fs (nanosekunden genau) -> max %f times per sec\n", total_time_used_sec ,(total_time_used/1000.0/1000.0/1000.0), frequency);
	}
}