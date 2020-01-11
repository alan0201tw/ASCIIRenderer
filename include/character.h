#pragma once

#include "transform.h"

#include "framebuffer.h"

typedef struct
{
    transform_t m_transform;

} humanoid_t;

void render_humanoid(frame_buffer_t* const target, const humanoid_t* const humanoid);