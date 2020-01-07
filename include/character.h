#pragma once

#include "framebuffer.h"

typedef struct
{
    vec2 m_position;

} humanoid_t;

void render_humanoid(frame_buffer_t* const target, const humanoid_t* const humanoid);