#pragma once

#include "transform.h"

#include "framebuffer.h"

// forward declaration
struct ASCRanimationState;

typedef struct ASCRtextSprite
{
    // a 2D character array, as a flexible array member
    size_t rowCount;
    size_t* columnCountEachRow;

    char** content;

} ASCRtextSprite;

typedef struct ASCRtextSpriteEntity
{
    ASCRtransform transform;
    ASCRtextSprite* textSprite;

    struct ASCRanimationState* animatorState;
    
} ASCRtextSpriteEntity;

void ascrTextSpriteCreateFromFile(ASCRtextSprite* textSprite, const char* const file_name);
void ascrTextSpriteEntityRender(ASCRframeBuffer* const target, const ASCRtextSpriteEntity* const entity);

void ascrTextSpriteEntityUpdate(ASCRtextSpriteEntity* entity, float deltaTime);

void ascrFreeTextSprite(ASCRtextSprite* textSprite);
