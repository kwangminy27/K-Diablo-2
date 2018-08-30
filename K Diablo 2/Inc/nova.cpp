#include "stdafx.h"
#include "nova.h"

#include "math.h"
#include "object_manager.h"
#include "point_collider.h"
#include "monster.h"
#include "audio_manager.h"
#include "effect.h"

using namespace std;
using namespace TYPE;

Nova::Nova(Nova const& _other) : Missile(_other)
{
}

Nova::Nova(Nova&& _other) noexcept : Missile(move(_other))
{
}

void Nova::_Release()
{
}

bool Nova::_Initialize()
{
	return true;
}

void Nova::_Input(float _time)
{
}

void Nova::_Update(float _time)
{
	for (int i = 0; i < 32; ++i)
	{
		auto frost_nova_piece = dynamic_pointer_cast<Missile>(ObjectManager::GetSingleton()->CreateCloneObject("frost_nova_piece", layer()));
		frost_nova_piece->set_position(position_);
		frost_nova_piece->AddAnimationClip("frost_nova_"s + to_string(i / 2));
		frost_nova_piece->set_move_range(300.f);
		frost_nova_piece->set_move_speed(600.f);
		frost_nova_piece->set_isometric_move_flag(true);

		float angle = static_cast<float>(static_cast<int>(i * 11.25f + 281.25) % 360);
		frost_nova_piece->set_dir({ cos(Math::ConvertToRadians(angle)), sin(Math::ConvertToRadians(angle)) });

		auto frost_nova_piece_collider = dynamic_pointer_cast<PointCollider>(frost_nova_piece->AddCollider<PointCollider>("frost_nova_piece_collider"));
		frost_nova_piece_collider->set_model_info({ 60.f, 30.f });
		frost_nova_piece_collider->SetCallBack([](shared_ptr<Collider> const& _src, shared_ptr<Collider> const& _dest, float _time) {
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
					src_object->set_state(MONSTER_STATE::GET_HIT);
				else
					src_object->set_state(MONSTER_STATE::DEATH);
			}
			else if (_dest->tag() == "MonsterBody")
			{
				auto const& src_object = _src->object();
				auto const& dest_object = dynamic_pointer_cast<Monster>(_dest->object());

				dest_object->AddHp(-50.f);
				if (dest_object->hp() > 0.f)
					dest_object->set_state(MONSTER_STATE::GET_HIT);
				else
					dest_object->set_state(MONSTER_STATE::DEATH);
			}

		}, COLLISION_CALLBACK::ENTER);
	}

	set_activation(false);
}

void Nova::_LateUpdate(float _time)
{
}

void Nova::_Collision(float _time)
{
}

void Nova::_Render(HDC _device_context, float _time)
{
}

unique_ptr<Object, function<void(Object*)>> Nova::_Clone() const
{
	return unique_ptr<Object, function<void(Object*)>>{new Nova{ *this }, [](Object* _p) {
		dynamic_cast<Nova*>(_p)->_Release();
		delete dynamic_cast<Nova*>(_p);
	}};
}