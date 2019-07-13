#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <iostream>

enum
    {
        TRACK_WIDTH = 12000, // width,in pixels, of the track 
        TRACK_HEIGHT = 140, //height,in pixels, of the track
        
        INITIAL_TIMELINE_WINDOW_WIDTH = 500, //width in pixels of timeline window
        INITIAL_TIMELINE_WINDOW_HEIGHT = 700, //height in pixels of timeline window
         
        
        TIME_START_VALUE = 0, //start value, in seconds, of ruler and track time
        TIME_END_VALUE = 600, //end value, in seconds, of ruler and track time
        TIME_RESOLUTION = 500,  //resolution, in milliseconds, of playback timer and track timer. i.e. if TIME_RESOLUTION=0.1s, only move forward by 0.1s in playback
        
        REWIND_SPEED = 5, // how fast to rewind, moves REWIND_SPEED*TIME_RESOLUTION during rewind 
        FAST_FORWARD_SPEED = 5, // how fast to fast forward, FAST_FORWARD_SPEED*TIME_RESOLUTION during fast forward
        
        TIME_TICK_NUM = 21, //number of ticks to display in ruler, make sure to include zero
        VERTICAL_LINE_HEIGHT_TIME = 400 //height,in pixels, of the vertical line showing current time position 
    };
    
#endif
