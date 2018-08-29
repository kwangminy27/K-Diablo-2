#include "stdafx.h"
#include "nova.h"

#include "math.h"
#include "object_manager.h"

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

		float angle = static_cast<float>(static_cast<int>(i * 11.25f + 281.25) % 360);
		frost_nova_piece->set_dir({ cos(Math::ConvertToRadians(angle)), sin(Math::ConvertToRadians(angle)) });
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