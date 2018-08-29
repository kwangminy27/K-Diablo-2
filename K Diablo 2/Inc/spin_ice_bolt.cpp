#include "stdafx.h"
#include "spin_ice_bolt.h"

#include "math.h"

using namespace std;

SpinIceBolt::SpinIceBolt(SpinIceBolt const& _other) : Missile(_other)
{
	spin_point_1_ = _other.spin_point_1_;
	spin_point_2_ = _other.spin_point_2_;
}

SpinIceBolt::SpinIceBolt(SpinIceBolt&& _other) noexcept : Missile(move(_other))
{
	spin_point_1_ = move(_other.spin_point_1_);
	spin_point_2_ = move(_other.spin_point_2_);
}

void SpinIceBolt::_Release()
{
}

bool SpinIceBolt::_Initialize()
{
	Missile::_Initialize();

	move_speed_ = 600.f;

	return true;
}

void SpinIceBolt::_Input(float _time)
{
	Missile::_Input(_time);
}

void SpinIceBolt::_Update(float _time)
{
	Object::_Update(_time);

	float angle = Math::GetAngle({ 0.f, 0.f }, dir_);
	float stride = move_speed_ * _time;
	float isometric_correction_factor = sqrtf(1.f - cos(Math::ConvertToRadians(angle)) * cos(Math::ConvertToRadians(angle)) * 0.25f) * 0.5f;

	if (move_range_ <= 970.f && spin_point_1_)
	{
		spin_point_1_ = false;

		angle += 45.f;
		set_dir({ cos(Math::ConvertToRadians(angle)), sin(Math::ConvertToRadians(angle)) });

		int dir_idx = static_cast<int>((static_cast<int>(angle + 281.25) % 360) / 22.5f);

		AddAnimationClip("ice_bolt_"s + to_string(dir_idx));
		ChangeAnimationClip("ice_bolt_"s + to_string(dir_idx));
	}
	else if (move_range_ <= 940.f && spin_point_2_)
	{
		spin_point_2_ = false;

		angle += 22.5f;
		set_dir({ cos(Math::ConvertToRadians(angle)), sin(Math::ConvertToRadians(angle)) });

		int dir_idx = static_cast<int>((static_cast<int>(angle + 281.25) % 360) / 22.5f);

		AddAnimationClip("ice_bolt_"s + to_string(dir_idx));
		ChangeAnimationClip("ice_bolt_"s + to_string(dir_idx));
	}

	position_ += {dir_.x * stride, dir_.y * stride * isometric_correction_factor};
	move_range_ -= stride;

	if (move_range_ <= 0.f)
		set_activation(false);
}

void SpinIceBolt::_LateUpdate(float _time)
{
	Missile::_LateUpdate(_time);
}

void SpinIceBolt::_Collision(float _time)
{
	Missile::_Collision(_time);
}

void SpinIceBolt::_Render(HDC _device_context, float _time)
{
	Missile::_Render(_device_context, _time);
}

unique_ptr<Object, function<void(Object*)>> SpinIceBolt::_Clone() const
{
	return unique_ptr<Object, function<void(Object*)>>{ new SpinIceBolt{ *this }, [](Object* _p) {
		dynamic_cast<SpinIceBolt*>(_p)->_Release();
		delete dynamic_cast<SpinIceBolt*>(_p);
	} };
}