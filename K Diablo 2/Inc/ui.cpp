#include "stdafx.h"
#include "ui.h"

#include "texture.h"
#include "animation.h"
#include "collider.h"

using namespace std;
using namespace TYPE;

void UI::set_offset_flag(bool _flag)
{
	offset_flag_ = _flag;
}

UI::UI(UI const& _other) : Object(_other)
{
	offset_ = _other.offset_;
}

UI::UI(UI&& _other) noexcept : Object(move(_other))
{
	offset_ = move(_other.offset_);
}

void UI::_Release()
{
}

bool UI::_Initialize()
{
	type_ = OBJECT::UI;

	return true;
}

void UI::_Input(float _time)
{
	Object::_Input(_time);
}

void UI::_Update(float _time)
{
	Object::_Update(_time);
}

void UI::_LateUpdate(float _time)
{
	Object::_LateUpdate(_time);
}

void UI::_Collision(float _time)
{
	Object::_Collision(_time);
}

void UI::_Render(HDC _device_context, float _time)
{
	if (tag_ == "left_skill_tap")
		int a = 3;

	int left = static_cast<int>(position_.x - size_.x * pivot_.x);
	int top = static_cast<int>(position_.y - size_.y * pivot_.y);
	int width = static_cast<int>(size_.x);
	int height = static_cast<int>(size_.y);

	if (is_color_key_)
	{
		if (animation_)
		{
			width = static_cast<int>(animation_->GetFrameWidth());
			height = static_cast<int>(animation_->GetFrameHeight());

			int frame_left = static_cast<int>(animation_->current_x_ * width + offset_.x);
			int frame_top = static_cast<int>(animation_->current_y_ * height + offset_.y);

			TransparentBlt(_device_context, left, top, width, height, texture_->memory_device_context(), frame_left, frame_top, width, height, color_key_);
		}
		else
			TransparentBlt(_device_context, left, top, width, height, texture_->memory_device_context(), static_cast<int>(offset_.x), static_cast<int>(offset_.y), width, height, color_key_);
	}
	else
		BitBlt(_device_context, left, top, width, height, texture_->memory_device_context(), static_cast<int>(offset_.x), static_cast<int>(offset_.y), SRCCOPY);

	for (auto const& collider : collider_list_)
	{
		if (collider->enablement())
			collider->_Render(_device_context, _time);
	}
}

unique_ptr<Object, function<void(Object*)>> UI::_Clone() const
{
	return unique_ptr<Object, function<void(Object*)>>{new UI{ *this }, [](Object* _p) {
		dynamic_cast<UI*>(_p)->_Release();
		delete dynamic_cast<UI*>(_p);
	}};
}