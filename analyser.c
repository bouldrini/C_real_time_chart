// ***************************************************************************************************************************************************
// 																			analyser 1.0
// ***************************************************************************************************************************************************
// - analyser to analyse the frequency spectrum of a signal
// ***************************************************************************************************************************************************

#include "analyser.h"




// ===================================================================================================================================================
// ===================================================================================================================================================
// CONFIGURATION
// ===================================================================================================================================================
// ===================================================================================================================================================

const float baseline_frequency = 28; // Lowest hearable tone 28 Hz A

const char C[2] = "C";
const char Csharp[4] = "C#";
const char D[2] = "D";
const char Dsharp[4] = "D#";
const char E[2] = "E";
const char F[2] = "F";
const char Fsharp[4] = "F#";
const char G[2] = "G";
const char Gsharp[4] = "G#";
const char A[2] = "A";
const char Asharp[4] = "A#";
const char H[2] = "H";

// ===================================================================================================================================================
// ===================================================================================================================================================
// FUNCTIONALITY
// ===================================================================================================================================================
// ===================================================================================================================================================

analyser* analyser_init(shiftable_window* _window, float _sampling_resolution, bool _tone_identification){

	analyser* _analyser = (analyser*) malloc(sizeof(analyser));
	_analyser->m_sampling_resolution;
	_analyser->m_window = _window;
	_analyser->m_tone_identification = _tone_identification;
	_analyser->m_current_tone = "A"; // since baseline is A
	return _analyser;
}

// HELPER
double analyser_round(double d){
  return floor(d + 0.5);
}

// ANALYSATION
const char* analyser_get_tone_for_frequency(double _frequency){
	int steps = analyser_get_semitone_steps_from_baseline_for_frequency(_frequency);
	const char* tone_name = analyser_match_tone(steps);
	return tone_name;
}

void analyser_print_tone_matching(analyser* analyser, float* _spectrum, float _resolution){
	for(int i = 0; i < analyser->m_window->m_capacity; i = i + 2){
		float frequency = i * _resolution;
		if(frequency > 28.0 && frequency < 8000.0){
			const char* tone = analyser_get_tone_for_frequency(frequency);
			printf("Frequency %4.2f Hz -> %s\n", frequency, tone);
		}
	}
}

double analyser_get_frequency_from_semitone_steps_from_baseline(int _steps){
	double tone_spectrum = (1.0/12.0);
	double tone_temperment = pow(2, tone_spectrum);
	double multiplier = pow(tone_temperment, _steps);
	double frequency = multiplier * baseline_frequency;
	return frequency;
}

int analyser_get_semitone_steps_from_baseline_for_frequency(double _frequency){
	double relational_from_baseline = _frequency / baseline_frequency;
	double tone_spectrum = (1.0/12.0);
	double tone_temperment = pow(2, tone_spectrum);

	int semitone_steps_from_baseline = (int)analyser_round(log(relational_from_baseline) / log(tone_temperment));
	return semitone_steps_from_baseline;
}

const char* analyser_match_tone(int _steps){
	if(_steps >= 12){
		return analyser_match_tone(_steps - 12);
	}
	const char* distances_from_a[] = { A, Asharp, H, C, Csharp, D, Dsharp, E, F, Fsharp, G, Gsharp };
	return distances_from_a[_steps];
}

void analyser_as_frequency_spectrum(analyser* analyser, float* _frequency_spectrum, COMPLEX* _complex_window){
	for(int i = 0; i < analyser->m_window->m_capacity; i++){
		_frequency_spectrum[i] = sqrt(pow(_complex_window[i].real,2) + pow(_complex_window[i].imag,2));
	}
}

static void activate(GtkApplication* app, gpointer user_data)
{
  GtkWidget *window;
  window = gtk_application_window_new(app);
  gtk_window_set_title (GTK_WINDOW (window), "Window");
  gtk_window_set_default_size (GTK_WINDOW (window), 200, 200);
  gtk_widget_show_all (window);
}

void analyser_transform(analyser* analyser, float _resolution){
	// printf("\nTransforming Signal...\n");
	COMPLEX* current_window = (COMPLEX*) malloc(analyser->m_window->m_capacity * analyser->m_window->m_element_size * 2);
	float* frequency_spectrum = (float*) malloc(analyser->m_window->m_capacity * analyser->m_window->m_element_size);

	window_get_as_complex(analyser->m_window, current_window);

	fft_real(current_window, analyser->m_window->m_capacity);
	analyser_as_frequency_spectrum(analyser, frequency_spectrum, current_window);

    gnuplot_setstyle(analyser->m_window->m_fft_gnuplot_ctrl, "lines");
	gnuplot_resetplot(analyser->m_window->m_fft_gnuplot_ctrl);
	gnuplot_plot_x(analyser->m_window->m_fft_gnuplot_ctrl, frequency_spectrum, analyser->m_window->m_capacity, "FFT");
	
	if(analyser->m_tone_identification){
		analyser_tone_identification(analyser, frequency_spectrum, _resolution);
	}

	// free(current_window);
}


const char* analyser_tone_identification(analyser* analyser, float* _spectrum, float _resolution){
	// analyser_print_tone_matching(_window, _spectrum, _resolution);
	for(int i=0; i< analyser->m_window->m_capacity; i++){
		float frequency = i * _resolution;
		if(_spectrum[i] > 100){
			if(frequency > baseline_frequency){
				printf("Lowest found frequency: %4.2f Hz...\n", frequency);
				int steps = analyser_get_semitone_steps_from_baseline_for_frequency(frequency);
				printf("Frequency is %d steps far from baseline tone ...\n", steps);

				const char* tone = analyser_get_tone_for_frequency(frequency);
				printf("Calculated tone is: %s ...\n", tone);
				analyser->m_current_tone = tone;
				return tone;
			}
		}
	}
	return "";
}

void* analyser_analyse(void* _analyser){
	analyser* anal = (analyser*) _analyser;
	shiftable_window* _window = anal->m_window;

	mach_timespec_t process_start, total_end, process_end;
	clock_serv_t cclock;

	long total_time = 5000000000L;
	
	int pure_process_time_used_sec, total_time_used_sec;
	long pure_process_time_used, time_to_wait, total_time_used;

	struct timespec wait, wait2;

	double frequency;

	host_get_clock_service(mach_host_self(), SYSTEM_CLOCK, &cclock);

	// MESSUREMENT
	while(1==1){
		// take time
		clock_get_time(cclock, &process_start);
		
		// ANALYSE SIGNAL....
	    analyser_transform(anal, anal->m_sampling_resolution);
	    // ....

	    // take time
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

		// total_time_used_sec = total_end.tv_sec - process_start.tv_sec;
		// total_time_used = total_end.tv_nsec - process_start.tv_nsec;

		// if(total_time_used < 0){
		// 	total_time_used = 1000000000L + total_time_used;
		// }
		// frequency = (1/(total_time_used/1000.0/1000.0/1000.0));

		// printf("FFT took %fs (nanosekunden genau) -> max %f times per sec\n", (total_time_used/1000.0/1000.0/1000.0), frequency);
	}
	
	return NULL;
}