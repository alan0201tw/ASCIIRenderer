#include "framebuffer.h"

#include "transform.h"

void ascrFrameBufferClear(ASCRframeBuffer* fb)
{
    for(size_t i = 0; i < FRAME_BUFFER_WIDTH; ++i)
    {
        for(size_t j = 0; j < FRAME_BUFFER_HEIGHT; ++j)
        {
            fb->char_value[i][j] = ' ';
            
            fb->color_value[i][j][0] = 0.0f;
            fb->color_value[i][j][1] = 0.0f;
            fb->color_value[i][j][2] = 0.0f;

            fb->depth_value[i][j] = 1e9;
        }
    }
}

void ascrFrameBufferWriteChar(ASCRframeBuffer* fb, size_t x, size_t y, float depth, vec3 color, char pixel)
{
    // Do transformation to camera space
    x = x - (size_t)(fb->center[0]) + FRAME_BUFFER_WIDTH / 2;
    y = y - (size_t)(fb->center[1]) + FRAME_BUFFER_HEIGHT / 2;

    // view culling
    if(x < 0 || x >= FRAME_BUFFER_WIDTH || y < 0 || y >= FRAME_BUFFER_HEIGHT)
        return;

    // depth test
    if( depth > fb->depth_value[x][y] )
        return;
    
    fb->char_value[x][y] = pixel;

    fb->color_value[x][y][0] = color[0];
    fb->color_value[x][y][1] = color[1];
    fb->color_value[x][y][2] = color[2];

    fb->depth_value[x][y] = depth;
}

void ascrFrameBufferWriteString(ASCRframeBuffer* fb, size_t x, size_t y, float depth, vec3 color, char* pixels)
{
    for(size_t idx = 0; idx < strlen(pixels); ++idx)
    {
        ascrFrameBufferWriteChar(
            fb, x + idx, y, depth, color, pixels[idx]
        );
    }
}

void ascrFrameBufferWriteCharScreenSpace(ASCRframeBuffer* fb, size_t x, size_t y, float depth, vec3 color, char pixel)
{
    ascrFrameBufferWriteChar(
        fb, 
        x + fb->center[0] - FRAME_BUFFER_WIDTH / 2,
        y + fb->center[1] - FRAME_BUFFER_HEIGHT / 2, 
        depth, color, pixel
    );
}

void ascrFrameBufferWriteStringScreenSpace(ASCRframeBuffer* fb, size_t x, size_t y, float depth, vec3 color, char* pixels)
{
    for(size_t idx = 0; idx < strlen(pixels); ++idx)
    {
        ascrFrameBufferWriteCharScreenSpace(
            fb, x + idx, y, depth, color, pixels[idx]
        );
    }
}