#include "animation.h"

void ascrAnimationClipInit(ASCRanimationClip* clip)
{
    vec_init(&clip->sprites);
    clip->timeToNextSprite = 0.5f;
}

void ascrAnimationStateInit(ASCRanimationState* state)
{
    vec_init(&state->transitions);
    state->accumulationTimer = 0.0f;
}

static ASCRanimationState* ascrAnimationStateTransitionUpdate(
    ASCRanimationState* state,
    ASCRanimationStateTransition* transition)
{
    if(transition->predicate() == true)
    {
        return transition->targetState;
    }
    return state;
}

ASCRanimationState* ascrAnimationStateUpdate(ASCRanimationState* state)
{
    // look for any possible transitions before returning the original state
    for(size_t idx = 0; idx < state->transitions.length; idx++)
    {
        ASCRanimationState* nextState = 
            ascrAnimationStateTransitionUpdate(
                state, 
                &(state->transitions.data[idx]));
        
        if(nextState != state)
        {
            return nextState;
        }
    }

    return state;
}

void ascrFreeASCRanimationClip(ASCRanimationClip* clip)
{
    vec_deinit(&clip->sprites);
}

void ascrFreeASCRanimationState(ASCRanimationState* state)
{
    vec_deinit(&state->transitions);
}
