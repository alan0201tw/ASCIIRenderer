#pragma once

#include "linmath.h"

typedef struct
{
    // x, y : the position in the scene / world
    // z    : the depth value to perform depth test
    vec3 m_position;
    
} transform_t;
