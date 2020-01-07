#pragma once

#include "common.h"

#include "linmath.h"

typedef struct
{
    char char_value[FRAME_BUFFER_WIDTH][FRAME_BUFFER_HEIGHT];
    // record a normalized [0, 1) range color buffer
    // using this instead of unsigned char allows for easier HDR extension
    vec3 color_value[FRAME_BUFFER_WIDTH][FRAME_BUFFER_HEIGHT];
    
    // use this to perform depth test
    float depth_value[FRAME_BUFFER_WIDTH][FRAME_BUFFER_HEIGHT];

} frame_buffer_t;


void fb_clear_frame_buffer(frame_buffer_t* fb);
void fb_write_char(frame_buffer_t* fb, size_t x, size_t y, char pixel);