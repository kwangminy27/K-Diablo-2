#include "stdafx.h"
#include "missile.h"

#include "math.h"

using namespace std;
using namespace TYPE;

void Missile::set_dir(Point _dir)
{
	dir_ = _dir;
}

void Missile::set_move_speed(float _speed)
{
	move_speed_ = _speed;
}

void Missile::set_move_range(float _range)
{
	move_range_ = _range;
}

void Missile::set_isometric_move_flag(bool _flag)
{
	isometric_move_flag_ = _flag;
}

Missile::Missile(Missile const& _other) : Object(_other)
{
	dir_ = _other.dir_;
	move_speed_ = _other.move_speed_;
	move_range_ = _other.move_range_;
	isometric_move_flag_ = _other.isometric_move_flag_;
}

Missile::Missile(Missile&& _other) noexcept : Object(move(_other))
{
	dir_ = move(_other.dir_);
	move_speed_ = move(_other.move_speed_);
	move_range_ = move(_other.move_range_);
	isometric_move_flag_ = move(_other.isometric_move_flag_);
}

void Missile::_Release()
{
}

bool Missile::_Initialize()
{
	type_ = OBJECT::MISSILE;
	
	set_color_key(RGB(1, 1, 1));

	move_speed_ = 400.f;
	move_range_ = 1000.f;

	return true;
}

void Missile::_Input(float _time)
{
	Object::_Input(_time);
}

void Missile::_Update(float _time)
{
	Object::_Update(_time);

	float angle = Math::GetAngle({ 0.f, 0.f }, dir_);
	float isometric_correction_factor = sqrtf(1.f - cos(Math::ConvertToRadians(angle)) * cos(Math::ConvertToRadians(angle)) * 0.25f) * 0.5f;
	float stride = move_speed_ * _time;

	if (isometric_move_flag_)
		position_ += {dir_.x * stride, dir_.y * stride * isometric_correction_factor};
	else
		position_ += dir_ * stride;

	move_range_ -= stride;

	if (move_range_ <= 0.f)
		set_activation(false);
}

void Missile::_LateUpdate(float _time)
{
	Object::_LateUpdate(_time);
}

void Missile::_Collision(float _time)
{
	Object::_Collision(_time);
}

void Missile::_Render(HDC _device_context, float _time)
{
	Object::_Render(_device_context, _time);
}

unique_ptr<Object, function<void(Object*)>> Missile::_Clone() const
{
	return unique_ptr<Object, function<void(Object*)>>{new Missile(*this), [](Object* _p) {
		dynamic_cast<Missile*>(_p)->_Release();
		delete dynamic_cast<Missile*>(_p);
	}};
}