#include "stdafx.h"
#include "effect.h"

using namespace std;

Effect::Effect(Effect const& _other) : Object(_other)
{
}

Effect::Effect(Effect&& _other) noexcept : Object(move(_other))
{
}

void Effect::_Release()
{
}

bool Effect::_Initialize()
{
	type_ = OBJECT::EFFECT;

	return true;
}

void Effect::_Input(float _time)
{
	Object::_Input(_time);
}

void Effect::_Update(float _time)
{
	Object::_Update(_time);
}

void Effect::_LateUpdate(float _time)
{
	Object::_LateUpdate(_time);
}

void Effect::_Collision(float _time)
{
	Object::_Collision(_time);
}

void Effect::_Render(HDC _device_context, float _time)
{
	Object::_Render(_device_context, _time);
}

unique_ptr<Object, function<void(Object*)>> Effect::_Clone() const
{
	return unique_ptr<Object, function<void(Object*)>>{new Effect{ *this }, [](Object* _p) {
		dynamic_cast<Effect*>(_p)->_Release();
		delete dynamic_cast<Effect*>(_p);
	}};
}