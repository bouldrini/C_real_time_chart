static gboolean
cb_timeout( gpointer data )
{
    /* Timeouts also don't automatically obtain main lock, but
       since we added this one using gdk_threads_add_timeout,
       lock has been obtained for us. */
    gtk_label_set_text( GTK_LABEL( data ), current_diagram->m_current_tone);

    return( TRUE );
}

// int main(int _argc, char *_argv[]){

// 	// Declarations and Initializations
// 	shiftable_window window;
// 	gnuplot_ctrl* h1;
// 	window_init(&window, L, h1);
// 	diagram* dia = diagram_init(&window, diagram_resolution, diagram_samples_per_frame, diagram_seconds_per_frame);
// 	current_diagram = dia;


// 	//  Tone Analysation
// 	const GdkRGBA green = {0, 0x0000, 0xffff, 0x0000};
// 	GtkWidget *win;
// 	GtkWidget *layout;
// 	char ** args;
	
// 	if( ! g_thread_supported() )
//         g_thread_init( NULL );

//     /* Secure gtk */
//     gdk_threads_init();

//     /* Obtain gtk's global lock */
//     gdk_threads_enter();

// 	gtk_init(0, &args);
// 	win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
// 	// gtk_widget_override_background_color(window, GTK_STATE_NORMAL, &green);
// 	gtk_window_set_default_size(GTK_WINDOW(win), 200, 200);
// 	gtk_window_set_title(GTK_WINDOW(win), "Tone");
// 	gtk_window_set_position(GTK_WINDOW(win), GTK_WIN_POS_CENTER);
// 	// layout = gtk_fixed_new();
// 	// gtk_container_add(GTK_CONTAINER(win), layout);
// 	GtkWidget* box = gtk_box_new(TRUE, 25);
//     gtk_container_add(GTK_CONTAINER(win), box);
//     gtk_widget_show(box);
// 	GtkWidget* label = gtk_label_new("C");
//     gtk_widget_show(label);
//     gtk_box_pack_start(GTK_BOX(box), label, FALSE, TRUE, 1);

//     dia->m_display = label;


//     // signal
// 	pthread_t updater;
// 	pthread_create (&updater, NULL, parsing_unit, dia);
// 	pthread_detach(updater);

// 	gtk_widget_show_all(win);

// 	// g_idle_add( cb_idle, (gpointer)label );
//     gdk_threads_add_timeout( 100, cb_timeout, (gpointer)label );

// 	gtk_main();
// 	gdk_threads_leave();

// 	int leave = 0;
// 	while(leave == 0){
// 		printf("Programm verlassen [y=1|n=0]: ");
// 		scanf("%d", &leave);
// 	}

// 	gnuplot_close(window.m_gnuplot_ctrl);
// 	return 1;
// }