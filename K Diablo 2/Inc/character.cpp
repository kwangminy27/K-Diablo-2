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