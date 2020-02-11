#pragma once

#include <stdbool.h>

// include dynamic array library
#include "vec/vec.h"

#include "textsprite.h"

struct ASCRanimationStateTransition;
struct ASCRanimationState;

typedef bool (*predicateFunction) ();
typedef vec_t(ASCRtextSprite) ASCRtextSpriteVector;

typedef struct ASCRanimationClip
{
	ASCRtextSpriteVector sprites;

} ASCRanimationClip;


typedef struct ASCRanimationStateTransition
{
	predicateFunction predicate;
	struct ASCRanimationState* targetState;

} ASCRanimationStateTransition;

typedef vec_t(ASCRanimationStateTransition) ASCRanimationStateTransitionVector;

typedef struct ASCRanimationState
{
	ASCRanimationClip clip;
	ASCRanimationStateTransitionVector transitions;

} ASCRanimationState;

void ascrAnimationClipInit(ASCRanimationClip* clip);
void ascrAnimationStateInit(ASCRanimationState* state);

ASCRanimationState* ascrAnimationStateTransitionUpdate(
    ASCRanimationState* state,
    ASCRanimationStateTransition* transition);
	
/**
 * 	Target Usage :
 * 	
 * 	ASCRanimationState characterIdle, characterWalking;
 * 	ascrAnimationStateInit(&characterIdle);
 * 	ascrAnimationStateInit(&characterWalking);
 * 	
 * 	characterIdle.clip = idleClip;
 * 	characterWalking.clip = walkingClip;
 * 
 * 	ASCRanimationStateTransition transition;
 * 	transition.predicate = someFunction;
 * 	transition.targetState = &characterWalking;
 * 
 * 	vec_push(
 * 		&characterIdle.transitions,
 * 		transition
 * 	)
 */