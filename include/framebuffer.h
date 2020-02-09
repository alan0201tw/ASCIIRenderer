#pragma once

#include "common.h"

#include "linmath.h"

typedef struct ASCRframeBuffer
{
    char char_value[FRAME_BUFFER_WIDTH][FRAME_BUFFER_HEIGHT];
    // record a normalized [0, 1) range color buffer
    // using this instead of unsigned char allows for easier HDR extension
    vec3 color_value[FRAME_BUFFER_WIDTH][FRAME_BUFFER_HEIGHT];
    
    // use this to perform depth test
    float depth_value[FRAME_BUFFER_WIDTH][FRAME_BUFFER_HEIGHT];

    // combine camera information along in the frame_buffer_t type
    // for a 2D ortho camera that will not rotate, using a vec2 to represent
    // camera center is sufficient
    vec2 center;

} ASCRframeBuffer;


void ascrFrameBufferClear(ASCRframeBuffer* fb);
void ascrFrameBufferWriteChar(ASCRframeBuffer* fb, size_t x, size_t y, float depth, vec3 color, char pixel);