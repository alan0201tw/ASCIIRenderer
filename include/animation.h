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
	// the time interval between sprites is determined by the size 
	// of the sprites and frameRate, currently all sprites are neighbor frames
	// Keyframe utility should be added for a more sophisticated animation system
	float timeToNextSprite;

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

	float accumulationTimer;

} ASCRanimationState;

void ascrAnimationClipInit(ASCRanimationClip* clip);
void ascrAnimationStateInit(ASCRanimationState* state);

// ASCRanimationState* ascrAnimationStateTransitionUpdate(
//     ASCRanimationState* state,
//     ASCRanimationStateTransition* transition);

ASCRanimationState* ascrAnimationStateUpdate(ASCRanimationState* state);

void ascrFreeASCRanimationClip(ASCRanimationClip* clip);
void ascrFreeASCRanimationState(ASCRanimationState* state);


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