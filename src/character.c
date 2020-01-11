#include "character.h"

void render_humanoid(frame_buffer_t* const target, const humanoid_t* const humanoid)
{
    vec2 position;
    position[0] = humanoid->m_transform.m_position[0];
    position[1] = humanoid->m_transform.m_position[1];
    
    float depth = humanoid->m_transform.m_position[2];

    vec3 color = {1.0f, 1.0f, 1.0f};

    fb_write_char(target, (size_t)position[0]    , (size_t)position[1] + 1, depth, color, 'o');
    fb_write_char(target, (size_t)position[0]    , (size_t)position[1]    , depth, color, 'l');
    fb_write_char(target, (size_t)position[0] - 1, (size_t)position[1]    , depth, color, '/');
    fb_write_char(target, (size_t)position[0] + 1, (size_t)position[1]    , depth, color, '\\');
    fb_write_char(target, (size_t)position[0]    , (size_t)position[1] - 1, depth, color, 'l');
    fb_write_char(target, (size_t)position[0] - 1, (size_t)position[1] - 2, depth, color, '/');
    fb_write_char(target, (size_t)position[0] + 1, (size_t)position[1] - 2, depth, color, '\\');
}