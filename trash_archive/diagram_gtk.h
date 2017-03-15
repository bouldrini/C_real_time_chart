#ifndef _DIAGRAM_H
	#define _DIAGRAM_H
	
	// Standard
	#include <stdbool.h>
	#include <stdint.h>
	#include <stdio.h>
	#include <time.h>
	#include <stdlib.h>

	// GUI
	#include <gtk/gtk.h>

	typedef struct diagram {
		GtkWidget* m_window;
		GtkWidget* m_layout;
		GtkWidget* m_canvas;
		int m_step;
		int m_width;
	} diagram;

	diagram* diagram_init();
	void diagram_show(diagram* _dia);
	void diagram_buttons(diagram* _dia);
	void diagram_canvas(diagram* _dia);
	// void log(char* message);
#endif