#include "stdafx.h"
#include "spin_ice_bolt.h"

#include "math.h"
#include "monster.h"
#include "point_collider.h"
#include "object_manager.h"
#include "audio_manager.h"
#include "effect.h"

using namespace std;
using namespace TYPE;

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

		angle += 60.f;
		set_dir({ cos(Math::ConvertToRadians(angle)), sin(Math::ConvertToRadians(angle)) });

		int dir_idx = static_cast<int>((static_cast<int>(angle + 281.25) % 360) / 22.5f);
		auto collider = dynamic_pointer_cast<PointCollider>(collider_list_.front());
		collider->SetCallBack([](shared_ptr<Collider> const& _src, shared_ptr<Collider> const& _dest, float _time) {
			random_device r;
			default_random_engine gen(r());
			uniform_int_distribution uniform_dist(1, 3);
			int number = uniform_dist(gen);

			if (_src->tag() == "MonsterBody")
			{
				auto const& src_object = dynamic_pointer_cast<Monster>(_src->object());
				auto const& dest_object = _dest->object();

				src_object->AddHp(-10.f);
				if (src_object->hp() > 0.f)
					src_object->set_state(CHARACTER_STATE::GET_HIT);
				else
					src_object->set_state(CHARACTER_STATE::DEATH);
			}
			else if (_dest->tag() == "MonsterBody")
			{
				auto const& src_object = _src->object();
				auto const& dest_object = dynamic_pointer_cast<Monster>(_dest->object());

				dest_object->AddHp(-10.f);
				if (dest_object->hp() > 0.f)
					dest_object->set_state(CHARACTER_STATE::GET_HIT);
				else
					dest_object->set_state(CHARACTER_STATE::DEATH);
			}

		}, COLLISION_CALLBACK::ENTER);

		switch (dir_idx)
		{
		case 0:
			collider->set_model_info({ 52.f, 10.f });
			break;
		case 1:
			collider->set_model_info({ 31.f, 9.f });
			break;
		case 2:
			collider->set_model_info({ 15.f, -4.f });
			break;
		case 3:
			collider->set_model_info({ 7.f, -17.f });
			break;
		case 4:
			collider->set_model_info({ 1.f, -21.f });
			break;
		case 5:
			collider->set_model_info({ -5.f, -28.f });
			break;
		case 6:
			collider->set_model_info({ 7.f, -35.f });
			break;
		case 7:
			collider->set_model_info({ 20.f, -40.f });
			break;
		case 8:
			collider->set_model_info({ 50.f, -54.f });
			break;
		case 9:
			collider->set_model_info({ 82.f, -44.f });
			break;
		case 10:
			collider->set_model_info({ 96.f, -37.f });
			break;
		case 11:
			collider->set_model_info({ 103.f, -28.f });
			break;
		case 12:
			collider->set_model_info({ 102.f, -20.f });
			break;
		case 13:
			collider->set_model_info({ 93.f, -17.f });
			break;
		case 14:
			collider->set_model_info({ 85.f, -4.f });
			break;
		case 15:
			collider->set_model_info({ 71.f, 7.f });
			break;
		}

		AddAnimationClip("ice_bolt_"s + to_string(dir_idx));
		ChangeAnimationClip("ice_bolt_"s + to_string(dir_idx));
	}
	else if (move_range_ <= 940.f && spin_point_2_)
	{
		spin_point_2_ = false;

		angle += 30.f;
		set_dir({ cos(Math::ConvertToRadians(angle)), sin(Math::ConvertToRadians(angle)) });

		int dir_idx = static_cast<int>((static_cast<int>(angle + 281.25) % 360) / 22.5f);
		auto collider = dynamic_pointer_cast<PointCollider>(collider_list_.front());
		collider->SetCallBack([](shared_ptr<Collider> const& _src, shared_ptr<Collider> const& _dest, float _time) {
			random_device r;
			default_random_engine gen(r());
			uniform_int_distribution uniform_dist(1, 3);
			int number = uniform_dist(gen);

			if (_src->tag() == "MonsterBody")
			{
				auto const& src_object = dynamic_pointer_cast<Monster>(_src->object());
				auto const& dest_object = _dest->object();

				src_object->AddHp(-10.f);
				if (src_object->hp() > 0.f)
					src_object->set_state(CHARACTER_STATE::GET_HIT);
				else
					src_object->set_state(CHARACTER_STATE::DEATH);
			}
			else if (_dest->tag() == "MonsterBody")
			{
				auto const& src_object = _src->object();
				auto const& dest_object = dynamic_pointer_cast<Monster>(_dest->object());

				dest_object->AddHp(-10.f);
				if (dest_object->hp() > 0.f)
					dest_object->set_state(CHARACTER_STATE::GET_HIT);
				else
					dest_object->set_state(CHARACTER_STATE::DEATH);
			}

		}, COLLISION_CALLBACK::ENTER);

		switch (dir_idx)
		{
		case 0:
			collider->set_model_info({ 52.f, 10.f });
			break;
		case 1:
			collider->set_model_info({ 31.f, 9.f });
			break;
		case 2:
			collider->set_model_info({ 15.f, -4.f });
			break;
		case 3:
			collider->set_model_info({ 7.f, -17.f });
			break;
		case 4:
			collider->set_model_info({ 1.f, -21.f });
			break;
		case 5:
			collider->set_model_info({ -5.f, -28.f });
			break;
		case 6:
			collider->set_model_info({ 7.f, -35.f });
			break;
		case 7:
			collider->set_model_info({ 20.f, -40.f });
			break;
		case 8:
			collider->set_model_info({ 50.f, -54.f });
			break;
		case 9:
			collider->set_model_info({ 82.f, -44.f });
			break;
		case 10:
			collider->set_model_info({ 96.f, -37.f });
			break;
		case 11:
			collider->set_model_info({ 103.f, -28.f });
			break;
		case 12:
			collider->set_model_info({ 102.f, -20.f });
			break;
		case 13:
			collider->set_model_info({ 93.f, -17.f });
			break;
		case 14:
			collider->set_model_info({ 85.f, -4.f });
			break;
		case 15:
			collider->set_model_info({ 71.f, 7.f });
			break;
		}

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