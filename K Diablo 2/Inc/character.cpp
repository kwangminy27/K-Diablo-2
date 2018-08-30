#include "stdafx.h"
#include "character.h"

using namespace std;

Character::Character(Character const& _other) : Object(_other)
{
}

Character::Character(Character&& _other) noexcept : Object(move(_other))
{
}

void Character::_Release()
{
}

bool Character::_Initialize()
{
	type_ = OBJECT::CHARACTER;

	return true;
}

void Character::_Input(float _time)
{
	Object::_Input(_time);
}

void Character::_Update(float _time)
{
	Object::_Update(_time);
}

void Character::_LateUpdate(float _time)
{
	Object::_LateUpdate(_time);
}

void Character::_Collision(float _time)
{
	Object::_Collision(_time);
}

void Character::_Render(HDC _device_context, float _time)
{
	Object::_Render(_device_context, _time);
}

unique_ptr<Object, function<void(Object*)>> Character::_Clone() const
{
	return unique_ptr<Object, function<void(Object*)>>{new Character{ *this }, [](Object* _p) {
		dynamic_cast<Character*>(_p)->_Release();
		delete dynamic_cast<Character*>(_p);
	}};
}

float Character::hp() const
{
	return hp_;
}

float Character::max_hp() const
{
	return max_hp_;
}

float Character::mp() const
{
	return mp_;
}

float Character::max_mp() const
{
	return max_mp_;
}

void Character::set_hp(float _hp)
{
	hp_ = _hp;
}

void Character::set_max_hp(float _hp)
{
	max_hp_ = _hp;
}

void Character::set_mp(float _mp)
{
	mp_ = _mp;
}

void Character::set_max_mp(float _mp)
{
	max_mp_ = _mp;
}

void Character::AddHp(float _amount)
{
	hp_ += _amount;
}

void Character::AddMp(float _amount)
{
	mp_ += _amount;
}
