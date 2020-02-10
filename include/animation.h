#pragma once

#include <stdbool.h>

// include dynamic array library
#include "vec/vec.h"

#include "textsprite.h"


typedef bool (*predicateFunction) ();

typedef vec_t(ASCRtextSprite) ASCRtextSpriteVector;

typedef struct ASCRanimationClip
{
	ASCRtextSpriteVector sprites;

} ASCRanimationClip;


typedef struct ASCRanimationStateTransition
{
	predicateFunction* predicate;

} ASCRanimationStateTransition;

typedef vec_t(ASCRanimationStateTransition) ASCRanimationStateTransitionVector;

typedef struct ASCRanimationState
{
	ASCRanimationClip clip;
	ASCRanimationStateTransitionVector transitions;

} ASCRanimationState;
