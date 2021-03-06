#include "stdafx.h"
#include "frozen_orb.h"

#include "math.h"
#include "object_manager.h"
#include "effect.h"
#include "spin_ice_bolt.h"
#include "point_collider.h"
#include "monster.h"
#include "audio_manager.h"

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
		ice_bolt_1->set_position(position_ - Point{ 20.f, -50.f });
		ice_bolt_1->AddAnimationClip("ice_bolt_"s + to_string(dir_idx));
		ice_bolt_1->set_dir({ cos(Math::ConvertToRadians(angle)), sin(Math::ConvertToRadians(angle)) });

		auto ice_bolt_1_collider = dynamic_pointer_cast<PointCollider>(ice_bolt_1->AddCollider<PointCollider>("ice_bolt_1_collider"));
		ice_bolt_1_collider->SetCallBack([](shared_ptr<Collider> const& _src, shared_ptr<Collider> const& _dest, float _time) {
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
			ice_bolt_1_collider->set_model_info({ 52.f, 10.f });
			break;
		case 1:
			ice_bolt_1_collider->set_model_info({ 31.f, 9.f });
			break;
		case 2:
			ice_bolt_1_collider->set_model_info({ 15.f, -4.f });
			break;
		case 3:
			ice_bolt_1_collider->set_model_info({ 7.f, -17.f });
			break;
		case 4:
			ice_bolt_1_collider->set_model_info({ 1.f, -21.f });
			break;
		case 5:
			ice_bolt_1_collider->set_model_info({ -5.f, -28.f });
			break;
		case 6:
			ice_bolt_1_collider->set_model_info({ 7.f, -35.f });
			break;
		case 7:
			ice_bolt_1_collider->set_model_info({ 20.f, -40.f });
			break;
		case 8:
			ice_bolt_1_collider->set_model_info({ 50.f, -54.f });
			break;
		case 9:
			ice_bolt_1_collider->set_model_info({ 82.f, -44.f });
			break;
		case 10:
			ice_bolt_1_collider->set_model_info({ 96.f, -37.f });
			break;
		case 11:
			ice_bolt_1_collider->set_model_info({ 103.f, -28.f });
			break;
		case 12:
			ice_bolt_1_collider->set_model_info({ 102.f, -20.f });
			break;
		case 13:
			ice_bolt_1_collider->set_model_info({ 93.f, -17.f });
			break;
		case 14:
			ice_bolt_1_collider->set_model_info({ 85.f, -4.f });
			break;
		case 15:
			ice_bolt_1_collider->set_model_info({ 71.f, 7.f });
			break;
		}

		angle += 130.f;

		dir_idx = static_cast<int>((static_cast<int>(angle + 281.25) % 360) / 22.5f);

		auto ice_bolt_2 = dynamic_pointer_cast<Missile>(object_manager->CreateCloneObject("ice_bolt", layer()));
		ice_bolt_2->set_position(position_ - Point{ 20.f, -50.f });
		ice_bolt_2->AddAnimationClip("ice_bolt_"s + to_string(dir_idx));
		ice_bolt_2->set_dir({ cos(Math::ConvertToRadians(angle)), sin(Math::ConvertToRadians(angle)) });

		auto ice_bolt_2_collider = dynamic_pointer_cast<PointCollider>(ice_bolt_2->AddCollider<PointCollider>("ice_bolt_2_collider"));
		ice_bolt_2_collider->SetCallBack([](shared_ptr<Collider> const& _src, shared_ptr<Collider> const& _dest, float _time) {
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
			ice_bolt_2_collider->set_model_info({ 52.f, 10.f });
			break;
		case 1:
			ice_bolt_2_collider->set_model_info({ 31.f, 9.f });
			break;
		case 2:
			ice_bolt_2_collider->set_model_info({ 15.f, -4.f });
			break;
		case 3:
			ice_bolt_2_collider->set_model_info({ 7.f, -17.f });
			break;
		case 4:
			ice_bolt_2_collider->set_model_info({ 1.f, -21.f });
			break;
		case 5:
			ice_bolt_2_collider->set_model_info({ -5.f, -28.f });
			break;
		case 6:
			ice_bolt_2_collider->set_model_info({ 7.f, -35.f });
			break;
		case 7:
			ice_bolt_2_collider->set_model_info({ 20.f, -40.f });
			break;
		case 8:
			ice_bolt_2_collider->set_model_info({ 50.f, -54.f });
			break;
		case 9:
			ice_bolt_2_collider->set_model_info({ 82.f, -44.f });
			break;
		case 10:
			ice_bolt_2_collider->set_model_info({ 96.f, -37.f });
			break;
		case 11:
			ice_bolt_2_collider->set_model_info({ 103.f, -28.f });
			break;
		case 12:
			ice_bolt_2_collider->set_model_info({ 102.f, -20.f });
			break;
		case 13:
			ice_bolt_2_collider->set_model_info({ 93.f, -17.f });
			break;
		case 14:
			ice_bolt_2_collider->set_model_info({ 85.f, -4.f });
			break;
		case 15:
			ice_bolt_2_collider->set_model_info({ 71.f, 7.f });
			break;
		}

		angle += 130.f;

		dir_idx = static_cast<int>((static_cast<int>(angle + 281.25) % 360) / 22.5f);

		auto ice_bolt_3 = dynamic_pointer_cast<Missile>(object_manager->CreateCloneObject("ice_bolt", layer()));
		ice_bolt_3->set_position(position_ - Point{ 20.f, -50.f });
		ice_bolt_3->AddAnimationClip("ice_bolt_"s + to_string(dir_idx));
		ice_bolt_3->set_dir({ cos(Math::ConvertToRadians(angle)), sin(Math::ConvertToRadians(angle)) });

		auto ice_bolt_3_collider = dynamic_pointer_cast<PointCollider>(ice_bolt_3->AddCollider<PointCollider>("ice_bolt_3_collider"));
		ice_bolt_3_collider->SetCallBack([](shared_ptr<Collider> const& _src, shared_ptr<Collider> const& _dest, float _time) {
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
			ice_bolt_3_collider->set_model_info({ 52.f, 10.f });
			break;
		case 1:
			ice_bolt_3_collider->set_model_info({ 31.f, 9.f });
			break;
		case 2:
			ice_bolt_3_collider->set_model_info({ 15.f, -4.f });
			break;
		case 3:
			ice_bolt_3_collider->set_model_info({ 7.f, -17.f });
			break;
		case 4:
			ice_bolt_3_collider->set_model_info({ 1.f, -21.f });
			break;
		case 5:
			ice_bolt_3_collider->set_model_info({ -5.f, -28.f });
			break;
		case 6:
			ice_bolt_3_collider->set_model_info({ 7.f, -35.f });
			break;
		case 7:
			ice_bolt_3_collider->set_model_info({ 20.f, -40.f });
			break;
		case 8:
			ice_bolt_3_collider->set_model_info({ 50.f, -54.f });
			break;
		case 9:
			ice_bolt_3_collider->set_model_info({ 82.f, -44.f });
			break;
		case 10:
			ice_bolt_3_collider->set_model_info({ 96.f, -37.f });
			break;
		case 11:
			ice_bolt_3_collider->set_model_info({ 103.f, -28.f });
			break;
		case 12:
			ice_bolt_3_collider->set_model_info({ 102.f, -20.f });
			break;
		case 13:
			ice_bolt_3_collider->set_model_info({ 93.f, -17.f });
			break;
		case 14:
			ice_bolt_3_collider->set_model_info({ 85.f, -4.f });
			break;
		case 15:
			ice_bolt_3_collider->set_model_info({ 71.f, 7.f });
			break;
		}

		angle += 130.f;

		dir_idx = static_cast<int>((static_cast<int>(angle + 281.25) % 360) / 22.5f);

		auto ice_bolt_4 = dynamic_pointer_cast<Missile>(object_manager->CreateCloneObject("ice_bolt", layer()));
		ice_bolt_4->set_position(position_ - Point{ 20.f, -50.f });
		ice_bolt_4->AddAnimationClip("ice_bolt_"s + to_string(dir_idx));
		ice_bolt_4->set_dir({ cos(Math::ConvertToRadians(angle)), sin(Math::ConvertToRadians(angle)) });

		auto ice_bolt_4_collider = dynamic_pointer_cast<PointCollider>(ice_bolt_4->AddCollider<PointCollider>("ice_bolt_4_collider"));
		ice_bolt_4_collider->SetCallBack([](shared_ptr<Collider> const& _src, shared_ptr<Collider> const& _dest, float _time) {
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
			ice_bolt_4_collider->set_model_info({ 52.f, 10.f });
			break;
		case 1:
			ice_bolt_4_collider->set_model_info({ 31.f, 9.f });
			break;
		case 2:
			ice_bolt_4_collider->set_model_info({ 15.f, -4.f });
			break;
		case 3:
			ice_bolt_4_collider->set_model_info({ 7.f, -17.f });
			break;
		case 4:
			ice_bolt_4_collider->set_model_info({ 1.f, -21.f });
			break;
		case 5:
			ice_bolt_4_collider->set_model_info({ -5.f, -28.f });
			break;
		case 6:
			ice_bolt_4_collider->set_model_info({ 7.f, -35.f });
			break;
		case 7:
			ice_bolt_4_collider->set_model_info({ 20.f, -40.f });
			break;
		case 8:
			ice_bolt_4_collider->set_model_info({ 50.f, -54.f });
			break;
		case 9:
			ice_bolt_4_collider->set_model_info({ 82.f, -44.f });
			break;
		case 10:
			ice_bolt_4_collider->set_model_info({ 96.f, -37.f });
			break;
		case 11:
			ice_bolt_4_collider->set_model_info({ 103.f, -28.f });
			break;
		case 12:
			ice_bolt_4_collider->set_model_info({ 102.f, -20.f });
			break;
		case 13:
			ice_bolt_4_collider->set_model_info({ 93.f, -17.f });
			break;
		case 14:
			ice_bolt_4_collider->set_model_info({ 85.f, -4.f });
			break;
		case 15:
			ice_bolt_4_collider->set_model_info({ 71.f, 7.f });
			break;
		}
	}

	if (life_time_ <= 0.f)
	{
		for (int i = 0; i < 16; ++i)
		{
			auto spin_ice_bolt = dynamic_pointer_cast<SpinIceBolt>(object_manager->CreateCloneObject("spin_ice_bolt", layer()));
			spin_ice_bolt->set_position(position_ - Point{ 20.f, -50.f });
			spin_ice_bolt->AddAnimationClip("ice_bolt_"s + to_string(i));

			float angle = static_cast<float>(static_cast<int>(i * 22.5f + 281.25) % 360);
			spin_ice_bolt->set_dir({ cos(Math::ConvertToRadians(angle)), sin(Math::ConvertToRadians(angle)) });

			auto spin_ice_bolt_collider = dynamic_pointer_cast<PointCollider>(spin_ice_bolt->AddCollider<PointCollider>("spin_ice_bolt_collider"));
			spin_ice_bolt_collider->SetCallBack([](shared_ptr<Collider> const& _src, shared_ptr<Collider> const& _dest, float _time) {
				random_device r;
				default_random_engine gen(r());
				uniform_int_distribution uniform_dist(1, 3);
				int number = uniform_dist(gen);

				if (_src->tag() == "MonsterBody")
				{
					auto const& src_object = dynamic_pointer_cast<Monster>(_src->object());
					auto const& dest_object = _dest->object();

					src_object->AddHp(-50.f);
					if (src_object->hp() > 0.f)
						src_object->set_state(CHARACTER_STATE::GET_HIT);
					else
						src_object->set_state(CHARACTER_STATE::DEATH);
				}
				else if (_dest->tag() == "MonsterBody")
				{
					auto const& src_object = _src->object();
					auto const& dest_object = dynamic_pointer_cast<Monster>(_dest->object());

					dest_object->AddHp(-50.f);
					if (dest_object->hp() > 0.f)
						dest_object->set_state(CHARACTER_STATE::GET_HIT);
					else
						dest_object->set_state(CHARACTER_STATE::DEATH);
				}

			}, COLLISION_CALLBACK::ENTER);

			switch (i)
			{
			case 0:
				spin_ice_bolt_collider->set_model_info({ 52.f, 10.f });
				break;
			case 1:
				spin_ice_bolt_collider->set_model_info({ 31.f, 9.f });
				break;
			case 2:
				spin_ice_bolt_collider->set_model_info({ 15.f, -4.f });
				break;
			case 3:
				spin_ice_bolt_collider->set_model_info({ 7.f, -17.f });
				break;
			case 4:
				spin_ice_bolt_collider->set_model_info({ 1.f, -21.f });
				break;
			case 5:
				spin_ice_bolt_collider->set_model_info({ -5.f, -28.f });
				break;
			case 6:
				spin_ice_bolt_collider->set_model_info({ 7.f, -35.f });
				break;
			case 7:
				spin_ice_bolt_collider->set_model_info({ 20.f, -40.f });
				break;
			case 8:
				spin_ice_bolt_collider->set_model_info({ 50.f, -54.f });
				break;
			case 9:
				spin_ice_bolt_collider->set_model_info({ 82.f, -44.f });
				break;
			case 10:
				spin_ice_bolt_collider->set_model_info({ 96.f, -37.f });
				break;
			case 11:
				spin_ice_bolt_collider->set_model_info({ 103.f, -28.f });
				break;
			case 12:
				spin_ice_bolt_collider->set_model_info({ 102.f, -20.f });
				break;
			case 13:
				spin_ice_bolt_collider->set_model_info({ 93.f, -17.f });
				break;
			case 14:
				spin_ice_bolt_collider->set_model_info({ 85.f, -4.f });
				break;
			case 15:
				spin_ice_bolt_collider->set_model_info({ 71.f, 7.f });
				break;
			}
		}

		auto ice_orb_explode = dynamic_pointer_cast<Effect>(object_manager->CreateObject<Effect>("ice_orb_explode", layer()));
		ice_orb_explode->set_position(position_ - Point{ 85.f, 35.f });
		ice_orb_explode->AddAnimationClip("ice_orb_explode");
		ice_orb_explode->set_color_key(RGB(1, 1, 1));

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