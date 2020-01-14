#pragma once

#include "transform.h"

#include "framebuffer.h"

typedef struct
{
    transform_t m_transform;

    // a 2D character array, as a flexible array member
    size_t rowCount;
    size_t* columnCountEachRow;
    char** content;
    
} text_sprite_t;

void create_text_sprite(text_sprite_t* text_sprite, const char* const file_name);
void render_text_sprite(frame_buffer_t* const target, const text_sprite_t* const text_sprite);