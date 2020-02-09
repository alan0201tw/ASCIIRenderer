#pragma once

#include "transform.h"

#include "framebuffer.h"

typedef struct ASCRtextSprite
{
    ASCRtransform m_transform;

    // a 2D character array, as a flexible array member
    size_t rowCount;
    size_t* columnCountEachRow;
    char** content;
    
} ASCRtextSprite;

void ascrTextSpriteCreateFromFile(ASCRtextSprite* text_sprite, const char* const file_name);
void ascrTextSpriteRender(ASCRframeBuffer* const target, const ASCRtextSprite* const text_sprite);