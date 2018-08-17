#include "stdafx.h"
#include "animation.h"

#include "object.h"
#include "animation_manager.h"
#include "animation_clip.h"

using namespace std;

int Animation::current_x() const
{
	return current_x_;
}

int Animation::current_y() const
{
	return current_y_;
}

int Animation::GetFrameWidth() const
{
	return current_clip_->animation_clip_info_.frame_width;
}

int Animation::GetFrameHeight() const
{
	return current_clip_->animation_clip_info_.frame_height;
}

void Animation::set_default_clip(string const& _tag)
{
	default_clip_ = _FindAnimationClip(_tag);
}

void Animation::set_current_clip(string const& _tag)
{
	if (object_.expired())
		return;

	current_clip_ = _FindAnimationClip(_tag);

	if (!current_clip_)
		return;

	current_x_ = current_clip_->animation_clip_info_.start_x;
	current_y_ = current_clip_->animation_clip_info_.start_y;
	elapsed_time_ = 0.f;

	object_.lock()->set_texture(current_clip_->texture_);
}

void Animation::set_object(shared_ptr<Object> const& _object)
{
	object_ = _object;
}

Animation::Animation(Animation const& _other)
{
	*this = _other;
}

Animation::Animation(Animation&& _other)
{
	*this = move(_other);
}

void Animation::_Release()
{
}

bool Animation::_Initialize()
{
	return true;
}

void Animation::_Update(float _time)
{
	if (!current_clip_)
		return;

	if (object_.expired())
		return;

	auto const& animation_clip_info = current_clip_->animation_clip_info_;
	float interval_time = current_clip_->completion_time_ / animation_clip_info.total_count;

	elapsed_time_ += _time;

	while (elapsed_time_ >= interval_time)
	{
		elapsed_time_ -= interval_time;

		if (current_y_ == animation_clip_info.start_y + animation_clip_info.count_y)
		{
			if (current_x_ == animation_clip_info.end_x)
			{
				current_x_ = animation_clip_info.start_x;
				current_y_ = animation_clip_info.start_y;

				switch (current_clip_->option_)
				{
				case ANIMATION_OPTION::RETURN:
					_ChangeClip(default_clip_->tag_);
					break;
				case ANIMATION_OPTION::DESTROY:
					object_.lock()->set_activation(false);
					break;
				case ANIMATION_OPTION::ONCE:
					object_.lock()->set_enablement(false);
					break;
				}

				continue;
			}
		}

		if (current_x_ >= animation_clip_info.start_x + animation_clip_info.count_x)
		{
			++current_y_;

			current_x_ = animation_clip_info.start_x;

			continue;
		}

		++current_x_;
	}
}

void Animation::_ChangeClip(string const& _tag)
{
	if (current_clip_->tag() == _tag)
		return;

	set_current_clip(_tag);
}

unique_ptr<Animation, function<void(Animation*)>> Animation::_Clone() const
{
	return unique_ptr<Animation, function<void(Animation*)>>{ new Animation(*this), [](Animation* _p) {
		_p->_Release();
		delete _p;
	} };
}

bool Animation::_AddAnimationClip(string const& _tag)
{
	auto const& animation_manager = AnimationManager::GetSingleton();

	auto animation_clip = animation_manager->FindAnimationClip(_tag);

	if (!animation_clip)
		return false;

	animation_clip_map_.insert(make_pair(_tag, move(animation_clip)));

	if (animation_clip_map_.size() == 1)
	{
		set_default_clip(_tag);
		set_current_clip(_tag);
	}

	return true;
}

shared_ptr<AnimationClip> const& Animation::_FindAnimationClip(string const& _tag) const
{
	auto iter = animation_clip_map_.find(_tag);

	if (iter == animation_clip_map_.end())
		return animation_clip_nullptr_;

	return iter->second;
}