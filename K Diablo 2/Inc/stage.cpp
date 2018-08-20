#include "stdafx.h"
#include "stage.h"

using namespace std;

Stage::Stage(Stage const& _other)
{
}

Stage::Stage(Stage&& _other) noexcept
{
}

void Stage::_Release()
{
}

bool Stage::_Initialize()
{
	type_ = OBJECT::STAGE;

	return true;
}

void Stage::_Input(float _time)
{
	Object::_Input(_time);
}

void Stage::_Update(float _time)
{
	Object::_Update(_time);
}

void Stage::_LateUpdate(float _time)
{
	Object::_LateUpdate(_time);
}

void Stage::_Collision(float _time)
{
	Object::_Collision(_time);
}

void Stage::_Render(HDC _device_context, float _time)
{
	Object::_Render(_device_context, _time);
}

unique_ptr<Object, function<void(Object*)>> Stage::_Clone() const
{
	return unique_ptr<Object, function<void(Object*)>>{new Stage{ *this }, [](Object* _p) {
		dynamic_cast<Stage*>(_p)->_Release();
		delete dynamic_cast<Stage*>(_p);
	}};
}