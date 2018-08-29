#include "stdafx.h"
#include "ranged_monster.h"

using namespace std;

RangedMonster::RangedMonster(RangedMonster const& _other) : Monster(_other)
{
}

RangedMonster::RangedMonster(RangedMonster&& _other) noexcept : Monster(move(_other))
{
}

void RangedMonster::_Release()
{
}

bool RangedMonster::_Initialize()
{
	Monster::_Initialize();

	return true;
}

void RangedMonster::_Input(float _time)
{
	Monster::_Input(_time);
}

void RangedMonster::_Update(float _time)
{
	Monster::_Update(_time);
}

void RangedMonster::_LateUpdate(float _time)
{
	Monster::_LateUpdate(_time);
}

void RangedMonster::_Collision(float _time)
{
	Monster::_Collision(_time);
}

void RangedMonster::_Render(HDC _device_context, float _time)
{
	Monster::_Render(_device_context, _time);
}

unique_ptr<Object, function<void(Object*)>> RangedMonster::_Clone() const
{
	return unique_ptr<Object, function<void(Object*)>>{new RangedMonster{ *this }, [](Object* _p) {
		dynamic_cast<RangedMonster*>(_p)->_Release();
		delete dynamic_cast<RangedMonster*>(_p);
	}};
}