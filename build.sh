# GTK ATTEMB
eval "../waf configure"
eval "../waf -v"
eval "./build/real_time_chart"

# C
# eval "gcc -c -std=c99 -o window_c.o window.c"
# eval "gcc -c -std=c99 -o window_h.o window.h"
# eval "gcc -c -std=c99 -o gnuplot_c.o gnuplot_i.c"
# eval "gcc -c -std=c99 -o gnuplot_h.o gnuplot_i.h"
# eval "gcc -c -std=c99 -o generator_c.o generator.c"
# eval "gcc -c -std=c99 -o generator_h.o generator.h"
# eval "gcc -c -std=c99 -o diagram_c.o diagram.c"
# eval "gcc -c -std=c99 -o diagram_h.o diagram.h"
# eval "gcc -c -std=c99 -o analysor_c.o analysor.c"
# eval "gcc -c -std=c99 -o analysor_h.o analysor.h"
# eval "gcc -c -std=c99 -o zx_fft_c.o ./extern/zx_fft.c"
# eval "gcc -c -std=c99 -o zx_fft_head.o ./extern/zx_fft.h"
# eval "gcc -c -std=c99 -o zx_math_c.o ./extern/zx_math.c"
# eval "gcc -c -std=c99 -o zx_math_head.o ./extern/zx_math.h"
# eval "gcc -c -std=c99 -o main.o main.c"

# eval "gcc /System/Library/Frameworks/GLUT.framework/GLUT /System/Library/Frameworks/OpenGL.framework/OpenGL main.c -o myGlutApp"
# eval "./myGlutApp"

# eval "gcc -o real_time_chart zx_fft_c.o zx_fft_head.o zx_math_c.o zx_math_head.o analysor_c.o analysor_h.o generator_c.o generator_h.o gnuplot_c.o gnuplot_h.o window_c.o window_h.o diagram_c.o diagram_h.o main.o"

# eval "g++ -c test.cpp -o test.o"
# eval "g++ -c window.cpp -o window_c.o"
# eval "g++ -c gnuplot_i.cpp -o gnuplot_c.o"
# eval "g++ -c main.c -o main.o"
# eval "gcc gnuplot_c.o window_c.o test.o -o real_time_chart"

# eval "chmod 777 real_time_chart"
# eval "./real_time_chart"