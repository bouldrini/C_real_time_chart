#include"diagram.h"

diagram* current_diagram = NULL;

static void replot_event(GtkWidget *_widget,gpointer _data){
	g_print ("replot event occurred\n");
}


diagram* diagram_init(){
	const GdkRGBA green = {0, 0x0000, 0xffff, 0x0000};
	GtkWidget *window;
	GtkWidget *layout;


	char ** args;
	gtk_init (0, &args);

	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_widget_override_background_color(window, GTK_STATE_NORMAL, &green);
	gtk_window_set_default_size(GTK_WINDOW(window), 1200, 370);
	gtk_window_set_title (GTK_WINDOW (window), "Diagram");

	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	layout = gtk_fixed_new();
	gtk_container_add(GTK_CONTAINER(window), layout);

	diagram* dia = (diagram*)malloc(sizeof(diagram));
	dia->m_window = window;
	dia->m_layout = layout;
	dia->m_steps = 0;
	dia->m_width = 1200;

	current_diagram = dia;
	return dia;
}

void diagram_show(diagram* _dia){
	gtk_widget_show_all(_dia->m_window);
	gtk_main();
}

void diagram_buttons(diagram* _dia){
	GtkWidget* button = gtk_button_new_with_label("Update Plot");
	gtk_fixed_put(GTK_FIXED(_dia->m_layout), button, 1095, 330);
	gtk_widget_set_size_request(button, 100, 30);
	
	// g_signal_connect(_window, "destroy", G_CALLBACK (gtk_main_quit), NULL);
	// gtk_container_set_border_width(GTK_CONTAINER(_window), 10);

	// button = gtk_button_new_with_label("Update Plot");
	g_signal_connect(button, "clicked", G_CALLBACK(replot_event), NULL);

	// gtk_container_add(GTK_CONTAINER(_window), button);
	// gtk_widget_set_usize(GTK_WIDGET(button), 30, 30);
}

void diagram_push(diagram* _dia, float _value){
	_dia->m_steps = _dia->m_steps - 1;
	_dia->m_width = _dia->m_width + 1;
	gtk_fixed_move(GTK_FIXED(_dia->m_layout), _dia->m_canvas, _dia->m_steps, 20);
	gtk_widget_set_size_request(_dia->m_canvas, _dia->m_width, 300);
}

gboolean draw_callback(GtkWidget *widget, cairo_t *cr, gpointer data){
	guint width,height;
	GdkRGBA color;
	GtkStyleContext *context;

	context=gtk_widget_get_style_context(widget);
	width=gtk_widget_get_allocated_width(widget);
	height=gtk_widget_get_allocated_height(widget);
	
	gtk_render_background(context, cr, 0, 0, width, height);
	cairo_move_to(cr, 1200, 150);
	cairo_line_to(cr, 0, 150);
	// cairo_arc(cr,width/2.0,height/2.0,MIN(width,height)/2.0,0,2*G_PI);
	gtk_style_context_get_color(context, gtk_style_context_get_state(context), &color);
	
	gdk_cairo_set_source_rgba(cr, &color);
	gdk_cairo_set_source_rgba(cr, &color);

	int i = 0;
	while(i < 1000){
		if(i % 100 == 0){
			printf("%d\n", i);
			diagram_push(current_diagram, 1.0);
			cairo_move_to(cr, current_diagram->m_width, i+150);
			cairo_line_to(cr, current_diagram->m_width-1, i+150);
			cairo_stroke(cr); 
		}
		i = i + 1; 
	}
	return FALSE;
}

void diagram_canvas(diagram* _dia){
	const GdkRGBA white = {1, 0xFFFF, 0xFFFF, 0xFFFF};
	GtkWidget *drawing_area = gtk_drawing_area_new ();
	gtk_widget_override_background_color(drawing_area, GTK_STATE_NORMAL, &white);

	gtk_fixed_put(GTK_FIXED(_dia->m_layout), drawing_area, 0, 20);
	gtk_widget_set_size_request(drawing_area,1200,300);
	g_signal_connect(G_OBJECT(drawing_area),"draw", G_CALLBACK(draw_callback),NULL);
	_dia->m_canvas = drawing_area;
}














