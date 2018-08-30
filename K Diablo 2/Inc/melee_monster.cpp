#include "stdafx.h"
#include "melee_monster.h"

#include "math.h"

using namespace std;
using namespace TYPE;

MeleeMonster::MeleeMonster(MeleeMonster const& _other) : Monster(_other)
{
}

MeleeMonster::MeleeMonster(MeleeMonster&& _other) noexcept : Monster(move(_other))
{
}

void MeleeMonster::_Release()
{
}

bool MeleeMonster::_Initialize()
{
	Monster::_Initialize();

	return true;
}

void MeleeMonster::_Input(float _time)
{
	Monster::_Input(_time);
}

void MeleeMonster::_Update(float _time)
{
	Monster::_Update(_time);
}

void MeleeMonster::_LateUpdate(float _time)
{
	Monster::_LateUpdate(_time);
}

void MeleeMonster::_Collision(float _time)
{
	Monster::_Collision(_time);
}

void MeleeMonster::_Render(HDC _device_context, float _time)
{
	Monster::_Render(_device_context, _time);
}

unique_ptr<Object, function<void(Object*)>> MeleeMonster::_Clone() const
{
	return unique_ptr<Object, function<void(Object*)>>{new MeleeMonster{ *this }, [](Object* _p) {
		dynamic_cast<MeleeMonster*>(_p)->_Release();
		delete dynamic_cast<MeleeMonster*>(_p);
	}};
}