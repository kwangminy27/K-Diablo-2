#include "stdafx.h"
#include "bar.h"

#include "camera_manager.h"
#include "animation.h"
#include "texture.h"

using namespace std;
using namespace TYPE;

void Bar::set_range(pair<float, float> const& _range)
{
	range_ = _range;
}

void Bar::set_value(float _value)
{
	value_ = _value;
}

void Bar::set_cutting_direction(BAR_CUTTING_DIRECTION _direction)
{
	cutting_direction_ = _direction;
}

void Bar::set_ui_flag(bool _flag)
{
	ui_flag_ = _flag;
}

void Bar::AddValue(float _value)
{
	value_ += _value;
}

Bar::Bar(Bar const& _other) : Object(_other)
{
	range_ = _other.range_;
	value_ = _other.value_;
	cutting_direction_ = _other.cutting_direction_;
}

Bar::Bar(Bar&& _other) noexcept : Object(move(_other))
{
	range_ = move(_other.range_);
	value_ = move(_other.value_);
	cutting_direction_ = move(_other.cutting_direction_);
}

void Bar::_Release()
{
}

bool Bar::_Initialize()
{
	Object::_Initialize();

	return true;
}

void Bar::_Input(float _time)
{
	Object::_Input(_time);
}

void Bar::_Update(float _time)
{
	Object::_Update(_time);
}

void Bar::_LateUpdate(float _time)
{
	Object::_LateUpdate(_time);
}

void Bar::_Collision(float _time)
{
	Object::_Collision(_time);
}

void Bar::_Render(HDC _device_context, float _time)
{
	switch (cutting_direction_)
	{
	case BAR_CUTTING_DIRECTION::LEFT:
		pivot_ = { 0.f, 0.f };
		break;
	case BAR_CUTTING_DIRECTION::RIGHT:
		pivot_ = { 1.f, 0.f };
		break;
	case BAR_CUTTING_DIRECTION::UP:
		pivot_ = { 0.f, 0.f };
		break;
	case BAR_CUTTING_DIRECTION::DOWN:
		pivot_ = { 0.f, 1.f };
		break;
	}

	int left{};
	int top{};

	if (ui_flag_)
	{
		left = static_cast<int>(position_.x + size_.x * pivot_.x);
		top = static_cast<int>(position_.y + size_.y * pivot_.y);
	}
	else
	{
		auto camera_position = CameraManager::GetSingleton()->position();

		left = static_cast<int>(position_.x + size_.x * pivot_.x - camera_position.x);
		top = static_cast<int>(position_.y + size_.y * pivot_.y - camera_position.y);
	}

	int width = static_cast<int>(size_.x);
	int height = static_cast<int>(size_.y);

	float t = value_ / (range_.second - range_.first);

	switch (cutting_direction_)
	{
	case BAR_CUTTING_DIRECTION::LEFT:
		width = static_cast<int>(size_.x * t);
		break;
	case BAR_CUTTING_DIRECTION::RIGHT:
		width = static_cast<int>(size_.x * t);
		left -= width;
		break;
	case BAR_CUTTING_DIRECTION::UP:
		height = static_cast<int>(size_.y * t);
		break;
	case BAR_CUTTING_DIRECTION::DOWN:
		height = static_cast<int>(size_.y * t);
		top -= height;
		break;
	}

	if (is_color_key_)
	{
		if (animation_)
		{
			width = static_cast<int>(animation_->GetFrameWidth());
			height = static_cast<int>(animation_->GetFrameHeight());

			int frame_left = static_cast<int>(animation_->current_x() * width);
			int frame_top = static_cast<int>(animation_->current_y() * height);

			TransparentBlt(_device_context, left, top, width, height, texture_->memory_device_context(), frame_left, frame_top, width, height, color_key_);
		}
		else
			TransparentBlt(_device_context, left, top, width, height, texture_->memory_device_context(), 0, static_cast<int>(size_.y - height), width, height, color_key_);
	}
	else
		BitBlt(_device_context, left, top, width, height, texture_->memory_device_context(), 0, 0, SRCCOPY);
}

unique_ptr<Object, function<void(Object*)>> Bar::_Clone() const
{
	return unique_ptr<Object, function<void(Object*)>>{ new Bar{ *this }, [](Object* _p) {
		dynamic_cast<Bar*>(_p)->_Release();
		delete dynamic_cast<Bar*>(_p);
	} };
}