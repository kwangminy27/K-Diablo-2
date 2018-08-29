#include "stdafx.h"
#include "frozen_orb.h"

#include "math.h"
#include "object_manager.h"
#include "spin_ice_bolt.h"

using namespace std;
using namespace TYPE;

FrozenOrb::FrozenOrb(FrozenOrb const& _other) : Missile(_other)
{
	life_time_ = _other.life_time_;
}

FrozenOrb::FrozenOrb(FrozenOrb&& _other) noexcept : Missile(move(_other))
{
	life_time_ = move(_other.life_time_);
}

void FrozenOrb::_Release()
{
}

bool FrozenOrb::_Initialize()
{
	Missile::_Initialize();

	life_time_ = 1.5f;

	return true;
}

void FrozenOrb::_Input(float _time)
{
	Missile::_Input(_time);
}

void FrozenOrb::_Update(float _time)
{
	Missile::_Update(_time);

	auto const& object_manager = ObjectManager::GetSingleton();

	static float const kIntervalTime = 0.1f;
	static float elapsed_time = 0.f;
	static float angle = 0.f;

	angle = static_cast<float>(static_cast<int>(angle) % 360);

	life_time_ -= _time;
	elapsed_time += _time;

	if (elapsed_time >= kIntervalTime)
	{
		elapsed_time -= kIntervalTime;

		angle += 130.f;

		int dir_idx = static_cast<int>((static_cast<int>(angle + 281.25) % 360) / 22.5f);

		auto ice_bolt_1 = dynamic_pointer_cast<Missile>(object_manager->CreateCloneObject("ice_bolt", layer()));
		ice_bolt_1->set_position(position_ - Point{ 20.f, 0.f });
		ice_bolt_1->AddAnimationClip("ice_bolt_"s + to_string(dir_idx));
		ice_bolt_1->set_dir({ cos(Math::ConvertToRadians(angle)), sin(Math::ConvertToRadians(angle)) });

		angle += 130.f;

		dir_idx = static_cast<int>((static_cast<int>(angle + 281.25) % 360) / 22.5f);

		auto ice_bolt_2 = dynamic_pointer_cast<Missile>(object_manager->CreateCloneObject("ice_bolt", layer()));
		ice_bolt_2->set_position(position_ - Point{ 20.f, 0.f });
		ice_bolt_2->AddAnimationClip("ice_bolt_"s + to_string(dir_idx));
		ice_bolt_2->set_dir({ cos(Math::ConvertToRadians(angle)), sin(Math::ConvertToRadians(angle)) });

		angle += 130.f;

		dir_idx = static_cast<int>((static_cast<int>(angle + 281.25) % 360) / 22.5f);

		auto ice_bolt_3 = dynamic_pointer_cast<Missile>(object_manager->CreateCloneObject("ice_bolt", layer()));
		ice_bolt_3->set_position(position_ - Point{ 20.f, 0.f });
		ice_bolt_3->AddAnimationClip("ice_bolt_"s + to_string(dir_idx));
		ice_bolt_3->set_dir({ cos(Math::ConvertToRadians(angle)), sin(Math::ConvertToRadians(angle)) });

		angle += 130.f;

		dir_idx = static_cast<int>((static_cast<int>(angle + 281.25) % 360) / 22.5f);

		auto ice_bolt_4 = dynamic_pointer_cast<Missile>(object_manager->CreateCloneObject("ice_bolt", layer()));
		ice_bolt_4->set_position(position_ - Point{ 20.f, 0.f });
		ice_bolt_4->AddAnimationClip("ice_bolt_"s + to_string(dir_idx));
		ice_bolt_4->set_dir({ cos(Math::ConvertToRadians(angle)), sin(Math::ConvertToRadians(angle)) });
	}

	if (life_time_ <= 0.f)
	{
		for (int i = 0; i < 16; ++i)
		{
			auto spin_ice_bolt = dynamic_pointer_cast<SpinIceBolt>(object_manager->CreateCloneObject("spin_ice_bolt", layer()));
			spin_ice_bolt->set_position(position_ - Point{ 20.f, 0.f });
			spin_ice_bolt->AddAnimationClip("ice_bolt_"s + to_string(i));

			float angle = static_cast<float>(static_cast<int>(i * 22.5f + 281.25) % 360);
			spin_ice_bolt->set_dir({ cos(Math::ConvertToRadians(angle)), sin(Math::ConvertToRadians(angle)) });
		}

		set_activation(false);
	}
}

void FrozenOrb::_LateUpdate(float _time)
{
	Missile::_LateUpdate(_time);
}

void FrozenOrb::_Collision(float _time)
{
	Missile::_Collision(_time);
}

void FrozenOrb::_Render(HDC _device_context, float _time)
{
	Missile::_Render(_device_context, _time);
}

unique_ptr<Object, function<void(Object*)>> FrozenOrb::_Clone() const
{
	return unique_ptr<Object, function<void(Object*)>>{ new FrozenOrb{ *this }, [](Object* _p) {
		dynamic_cast<FrozenOrb*>(_p)->_Release();
		delete dynamic_cast<FrozenOrb*>(_p);
	} };
}