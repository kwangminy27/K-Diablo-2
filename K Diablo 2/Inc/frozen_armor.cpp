#include "stdafx.h"
#include "frozen_armor.h"

#include "scene.h"
#include "layer.h"

using namespace std;
using namespace TYPE;

FrozenArmor::FrozenArmor(FrozenArmor const& _other) : Effect(_other)
{
}

FrozenArmor::FrozenArmor(FrozenArmor&& _other) noexcept : Effect(move(_other))
{
}

void FrozenArmor::_Release()
{
}

bool FrozenArmor::_Initialize()
{
	Object::type_ = OBJECT::EFFECT;

	return true;
}

void FrozenArmor::_Input(float _time)
{
	Effect::_Input(_time);
}

void FrozenArmor::_Update(float _time)
{
	Effect::_Update(_time);

	auto player = scene()->FindLayer("Default")->FindObject("player");
	set_position(player->position() - Point{37.f, 70.f});
}

void FrozenArmor::_LateUpdate(float _time)
{
	Effect::_LateUpdate(_time);
}

void FrozenArmor::_Collision(float _time)
{
	Effect::_Collision(_time);
}

void FrozenArmor::_Render(HDC _device_context, float _time)
{
	Effect::_Render(_device_context, _time);
}

unique_ptr<Object, function<void(Object*)>> FrozenArmor::_Clone() const
{
	return unique_ptr<Object, function<void(Object*)>>{ new FrozenArmor{ *this }, [](Object* _p) {
		dynamic_cast<FrozenArmor*>(_p)->_Release();
		delete dynamic_cast<FrozenArmor*>(_p);
	}};
}