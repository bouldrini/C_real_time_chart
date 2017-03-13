#! /usr/bin/env python

top = '.'
out = 'build'

def options(opt):
    opt.load ('compiler_c')

def configure(conf):
    conf.load ('compiler_c')
    conf.check_cfg (package='gtk+-3.0', uselib_store='GTKLIB', args='--cflags --libs')

def build(bld):

    bld(
        features='c cshlib', 
        source='./extern/zx_fft.c', 
        target='fft',
        use='math'
    )

    bld(
        features='c cshlib', 
        source='./extern/zx_math.c', 
        target='math'
    )

    bld(
        features='c cshlib', 
        source='./helper.c', 
        target='helper'
    )

    bld(
        features='c cshlib', 
        uselib='GTKLIB',
        source='./window.c', 
        target='window', 
        use='gnuplot helper'
    )
    
    bld(
        features='c cshlib', 
        source='./gnuplot_i.c', 
        target='gnuplot'
    )

    bld(
        features='c cshlib', 
        uselib='GTKLIB',
        source='./diagram.c', 
        target='diagram',
        use='gnuplot window'
    )

    bld(
        features='c cshlib', 
        uselib='GTKLIB',
        source='./analyser.c', 
        target='analysor',
        use='fft gnuplot window'
    )

    bld(
        features='c cshlib', 
        source='./synthesizer.c', 
        target='synthesizer',
        use='window helper'
    )

    bld(
        features='c cshlib', 
        source='./scheduler.c', 
        target='scheduler',
        use='analysor diagram window synthesizer'
    )

    app=bld(
        features= 'c cprogram',
        uselib='GTKLIB',
        source='main.c',
        target='real_time_chart',
        framework = ["GLUT", "OpenGL"], 
        use='analysor diagram synthesizer scheduler'
    )