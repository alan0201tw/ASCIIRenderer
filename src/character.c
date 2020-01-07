#include "character.h"

void render_humanoid(frame_buffer_t* const target, const humanoid_t* const humanoid)
{
    vec2 position;
    position[0] = humanoid->m_position[0];
    position[1] = humanoid->m_position[1];
    
    fb_write_char(target, (size_t)position[0]    , (size_t)position[1] + 1, 'o');
    fb_write_char(target, (size_t)position[0]    , (size_t)position[1]    , 'l');
    fb_write_char(target, (size_t)position[0] - 1, (size_t)position[1]    , '/');
    fb_write_char(target, (size_t)position[0] + 1, (size_t)position[1]    , '\\');
    fb_write_char(target, (size_t)position[0]    , (size_t)position[1] - 1, 'l');
    fb_write_char(target, (size_t)position[0] - 1, (size_t)position[1] - 2, '/');
    fb_write_char(target, (size_t)position[0] + 1, (size_t)position[1] - 2, '\\');
}