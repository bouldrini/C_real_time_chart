// ***************************************************************************************************************************************************
// 																			WINDOW 1.0
// ***************************************************************************************************************************************************
// - window for representing a shifting signal in memory
// ***************************************************************************************************************************************************

#include "window.h"
#include "helper.c"





// ===================================================================================================================================================
// ===================================================================================================================================================
// CONFIGURATION
// ===================================================================================================================================================
// ===================================================================================================================================================

pthread_mutex_t lock;


// ===================================================================================================================================================
// ===================================================================================================================================================
// FUNCTIONALITY
// ===================================================================================================================================================
// ===================================================================================================================================================

shiftable_window* window_init(long _capacity, gnuplot_ctrl* _h1){
	shiftable_window* _window = (shiftable_window*) malloc(sizeof(shiftable_window));

	_window->m_element_size = (int) sizeof(float);
	_window->m_gnuplot_ctrl = _h1;
	_window->m_capacity = _capacity;

	_window->m_buffer = (float*) malloc(_window->m_element_size * _window->m_capacity * 2);
	_window->m_buffer_end = _window->m_buffer + _window->m_capacity;

	for(int i = 0; i < _window->m_capacity * 2; i++){
		_window->m_buffer[i] = 0.0f;
	}

	_window->m_data_start = _window->m_buffer;
	_window->m_data_end = _window->m_buffer;
	_window->m_currently_highest_value = 0;
	_window->m_count = _window->m_capacity;
	_window->m_dirty = true;

	_window->m_gnuplot_ctrl = gnuplot_init();
	_window->m_fft_gnuplot_ctrl = gnuplot_init();

    gnuplot_setstyle(_window->m_gnuplot_ctrl, "lines");
    gnuplot_setstyle(_window->m_fft_gnuplot_ctrl, "lines");

    return _window;
}

void window_push_with_delay(shiftable_window* _window, float data, long _execution_time_in_ns){
	_execution_time_in_ns = 365000 / 8;
	mach_timespec_t push_start, push_end, total_end;
	clock_serv_t cclock;

	int total_time_used_sec;
	long total_time_used;

	host_get_clock_service(mach_host_self(), SYSTEM_CLOCK, &cclock);

	// printf("WINDOW PUSH\n");
	clock_get_time(cclock, &push_start);
	window_push(_window, data);
	clock_get_time(cclock, &push_end);
    nanowait(_execution_time_in_ns, &push_start, &push_end);

	// take total end time
	clock_get_time(cclock, &total_end);
}

void window_push(shiftable_window* _window, float data){
		// printf("WINDOW PUSH\n");
		*_window->m_data_end = data;

		if(data > _window->m_currently_highest_value){
			_window->m_currently_highest_value = data;
		}

		_window->m_data_end = _window->m_data_end + 1;
		_window->m_data_start = _window->m_data_start + 1;

		if(_window->m_data_end == _window->m_buffer_end){
			_window->m_data_end = _window->m_buffer;
		}

		if(_window->m_data_start == _window->m_buffer_end){
			_window->m_data_start = _window->m_buffer;
		}
		_window->m_dirty = true;
}

void window_free(shiftable_window* _window){
	free(_window->m_buffer);
}

// GETTING

float* window_get_mem(shiftable_window* _window){
	pthread_mutex_lock(&lock);

	// printf("WINDOW GET MEM\n");
	if(_window->m_dirty == true){
		memcpy(_window->m_buffer_end, _window->m_buffer, (_window->m_data_end - _window->m_buffer + 1) * _window->m_element_size);
		_window->m_dirty = false;
	}
	// printf("LEAVING GET MEM\n");

	pthread_mutex_unlock(&lock);
	return _window->m_data_start;
}

bool window_get_for_resolution(shiftable_window* _window, float* _result_window, float _resolution){
	float* start_address = window_get(_window);

	float float_interval = _window->m_capacity * _resolution;
	int interval = floor(float_interval);
	int i = 0;
	if(interval > 0){
		for(float* cur = start_address; cur < start_address + _window->m_capacity; cur=cur+interval){
			_result_window[i] = *cur;
			i++;
		}
		return true;
	} else {
		printf("ERROR (window_get_for_resolution): Invalid Resolution for window: should be calculated as 1 / _samples_per_frame\n");
		return false;
	}
}

float* window_get(shiftable_window* _window){
	// printf("WINDOW GET\n");
	float* start_address = window_get_mem(_window);
	return start_address;
}

float* window_get_for_diagram(shiftable_window* _window, float* _result_window, struct diagram* _diagram){
	// printf("WINDOW GET FOR DIAGRAM\n");
	return window_get_for_resolution(_window, _result_window, _diagram->m_resolution);
}

COMPLEX* window_get_as_complex(shiftable_window* _window, COMPLEX* _result_window){
	// printf("WINDOW GET AS COMPLEX\n");
	float* start_address = window_get(_window);
	int i = 0;
	for(float* cur = start_address; cur <= start_address + _window->m_capacity; cur++){
		COMPLEX val;
		val.real = *cur;
		val.imag = 0.0f;
		_result_window[i] = val;
		i++;
	}
	return _result_window;
}

// PRINTING

void window_print_infos(shiftable_window* _window)
{
	printf("WINDOW PRINT INFOS\n");
	printf("Window capacity: %d\nWindow count: %d\n", _window->m_capacity, _window->m_count);
}

void window_print(shiftable_window* _window)
{
	// printf("WINDOW PRINT\n");
	float * start_address = window_get(_window);
	printf("\n\nWINDOW IN ORDER: [\t");
	for(float* cur = start_address; cur < start_address + _window->m_capacity; cur++){
		printf("%f\t", *cur);
	}
	printf("]\n");
}

void window_print_for_resolution(shiftable_window* _window, float _resolution)
{	

	int interval = floor(_window->m_capacity * _resolution);
	if(interval > 0){
		int _samples_per_frame = _window->m_capacity / interval;

		float * signal_representation = (float*)malloc(_samples_per_frame * _window->m_element_size);
		bool result = window_get_for_resolution(_window, signal_representation, _resolution);
		// printf("WINDOW PRINT\n");
		if(result){
			printf("\n\nWINDOW FOR RESOLUTION %f: [\t", _resolution);
			for(int i = 0; i < _samples_per_frame; i++){
				printf("%f\t", signal_representation[i]);
			}
			printf("]\n");
		}
	} else {
		printf("ERROR (window_get_for_resolution): Invalid Resolution for window: should be calculated as 1 / _samples_per_frame\n");
	}
}

void window_print_full(shiftable_window* _window)
{
	printf("\n\nFULL WINDOW UNORDERED: [\t");
	for(int i = 0; i < _window->m_capacity * 2; i++){
		printf("%f\t", _window->m_buffer[i]);
	}
	printf("]\n\n");
}

void window_print_in_parts(shiftable_window* _window)
{
	window_get_mem(_window);
	printf("\n\nREPRESENTATIONAL PART: [\t");
	for(int i = 0; i < _window->m_capacity; i++){
		printf("%f\t", _window->m_buffer[i]);
	}
	printf("]\n\n");
	printf("\n\nEXTENSION PART: [\t");
	for(int i = _window->m_capacity; i < _window->m_capacity*2; i++){
		printf("%f\t", _window->m_buffer[i]);
	}
	printf("]\n\n");
}

void window_print_representation(shiftable_window* _window)
{
	printf("\n\nREPRESENTATIONAL PART: [\t");
	for(int i = 0; i < _window->m_capacity; i++){
		printf("%f\t", _window->m_buffer[i]);
	}
	printf("]\n\n");
}

void window_print_memory_extension(shiftable_window* _window)
{
	window_get_mem(_window);
	printf("\n\nEXTENSION PART: [\t");
	for(int i = _window->m_capacity; i < _window->m_capacity*2; i++){
		printf("%f\t", _window->m_buffer[i]);
	}
	printf("]\n\n");
}