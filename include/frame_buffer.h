#pragma once

#include "linmath.h"

#include "common.h"

typedef struct
{
    char char_value[FRAME_BUFFER_WIDTH][FRAME_BUFFER_HEIGHT];
    // record a normalized [0, 1) range color buffer
    // using this instead of unsigned char allows for easier HDR extension
    vec3 color_value[FRAME_BUFFER_WIDTH][FRAME_BUFFER_HEIGHT];
    
    // use this to perform depth test
    float depth_value[FRAME_BUFFER_WIDTH][FRAME_BUFFER_HEIGHT];

} frame_buffer_t;

void fb_clear_frame_buffer(frame_buffer_t* fb)
{
    for(size_t i = 0; i < FRAME_BUFFER_WIDTH; ++i)
    {
        for(size_t j = 0; j < FRAME_BUFFER_HEIGHT; ++j)
        {
            fb->char_value[i][j] = ' ';
            // TODO : also clear color and depth
        }
    }
}