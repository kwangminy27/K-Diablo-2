#pragma once

#include "tag.h"

class Texture;

struct AnimationClipInfo
{
	int frame_width{};
	int frame_height{};
	int offset_x{};
	int offset_y{};
	int start_x{};
	int start_y{};
	int end_x{};
	int count_x{};
	int count_y{};
	int total_count{};
};

class AnimationClip : public Tag
{
	friend class AnimationManager;
	friend class Animation;
private:
	AnimationClip() = default;
	AnimationClip(AnimationClip const& _other);
	AnimationClip(AnimationClip&& _other) noexcept;
	AnimationClip& operator=(AnimationClip const&) = default;
	AnimationClip& operator=(AnimationClip&&) noexcept = default;

	virtual void _Release() override;

	ANIMATION_CLIP type_{};
	ANIMATION_OPTION option_{};
	AnimationClipInfo animation_clip_info_{};
	float completion_time_{};
	std::shared_ptr<Texture> texture_{};
};