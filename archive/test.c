// int main(){
	// shiftable_window* window;
	// analyser* analyser;
	// diagram* dia;
	// gnuplot_ctrl* h1;

	// char line[100];
	// char * tmp;
	// float value;
	// const char* real;

	// while(1 == 1){
	// 	// Reopen
	// 	FILE *stream = fopen("signal.csv", "r");
	// 	if(stream == NULL){
	// 		perror("Error opening file");
	// 		exit(0);
	// 	}

	//     while (fgets(line, 100, stream)){
	//     	tmp = strdup(line);

	//     	// Pushing to window
	//         real = getfield(tmp, 2);
	//         value = atof(real);
	//         window_push(&window, value);
	//     }
	//     fclose(stream);

	// 	int i = 0;
	// 	stream = fopen("signal.csv", "r");
	// 	if(stream == NULL){
	// 		perror("Error opening file");
	// 		exit(0);
	// 	}
		
	//     while (fgets(line, 100, stream)){
	//     	i++;
	//     	if(i<500){
	// 	    	tmp = strdup(line);

	// 	    	// Pushing to window
	// 	        real = getfield(tmp, 2);
	// 	        value = atof(real);
	// 	        window_push(&window, value);

	// 	        // RAW AND IN PARTS
	// 		    // window_print_in_parts(&window);

	//     	}
	//     }
	//     fclose(stream);

	//     diagram_plot(dia, 100);

	//     // IN ORDER
	// 	// window_print_for_resolution(&window, diagram_resolution);

	// 	// analyser_window_transform(&window, sampling_resolution, true, dia);
	//     // diagram_plot(dia, 20);

	// }

// }