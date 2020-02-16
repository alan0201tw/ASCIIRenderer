#include "animation.h"

void ascrAnimationClipInit(ASCRanimationClip* clip)
{
    vec_init(&clip->sprites);
}

void ascrAnimationStateInit(ASCRanimationState* state)
{
    vec_init(&state->transitions);
}

ASCRanimationState* ascrAnimationStateTransitionUpdate(
    ASCRanimationState* state,
    ASCRanimationStateTransition* transition)
{
    if(transition->predicate() == true)
    {
        return transition->targetState;
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
