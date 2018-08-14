#include "stdafx.h"
#include "animation_clip.h"

using namespace std;

AnimationClip::AnimationClip(AnimationClip const& _other)
{
	*this = _other;
}

AnimationClip::AnimationClip(AnimationClip&& _other) noexcept
{
	*this = move(_other);
}

void AnimationClip::_Release()
{
}