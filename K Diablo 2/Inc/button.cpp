#include "stdafx.h"
#include "button.h"

#include "input_manager.h"
#include "collider.h"

using namespace std;

void Button::set_state(BUTTON_STATE _state)
{
	state_ = _state;
}

void Button::set_callback(function<void(float)> const& _callback)
{
	callback_list_.push_back(_callback);
}

void Button::OnCollisionEnter(shared_ptr<Collider> const& _src, shared_ptr<Collider> const& _dest, float _time)
{
	auto src_tag = _src->tag();
	auto dest_tag = _dest->tag();

	if (src_tag == "MouseCursor" && dest_tag == "Button")
		dynamic_pointer_cast<Button>(_dest->object())->set_state(BUTTON_STATE::UNDER_MOUSE);
	if (src_tag == "Button" && dest_tag == "MouseCursor")
		dynamic_pointer_cast<Button>(_src->object())->set_state(BUTTON_STATE::UNDER_MOUSE);
}

void Button::OnCollision(shared_ptr<Collider> const& _src, shared_ptr<Collider> const& _dest, float _time)
{
}

void Button::OnCollisionLeave(shared_ptr<Collider> const& _src, shared_ptr<Collider> const& _dest, float _time)
{
	auto src_tag = _src->tag();
	auto dest_tag = _dest->tag();

	if (src_tag == "MouseCursor" && dest_tag == "Button")
		dynamic_pointer_cast<Button>(_dest->object())->set_state(BUTTON_STATE::IDLE);
	if (src_tag == "Button" && dest_tag == "MouseCursor")
		dynamic_pointer_cast<Button>(_src->object())->set_state(BUTTON_STATE::IDLE);
}

Button::Button(Button const& _other) : UI(_other)
{
	state_ = _other.state_;
}

Button::Button(Button&& _other) noexcept : UI(move(_other))
{
	state_ = move(_other.state_);
}

void Button::_Release()
{
}

bool Button::_Initialize()
{
	return true;
}

void Button::_Input(float _time)
{
	UI::_Input(_time);

	auto const& input_manager = InputManager::GetSingleton();

	if (state_ != BUTTON_STATE::IDLE)
	{
		if (input_manager->KeyPush("MouseLeft"))
			state_ = BUTTON_STATE::PUSH;
		else if(input_manager->KeyPressed("MouseLeft"))
			state_ = BUTTON_STATE::PRESSED;
		else if (input_manager->KeyUp("MouseLeft"))
		{
			for (auto const& callback : callback_list_)
				callback(_time);

			state_ = BUTTON_STATE::UNDER_MOUSE;
		}
	}
}

void Button::_Update(float _time)
{
	UI::_Update(_time);

	switch (state_)
	{
	case BUTTON_STATE::IDLE:
	case BUTTON_STATE::UNDER_MOUSE:
		offset_.y = 0.f;
		break;
	case BUTTON_STATE::PUSH:
	case BUTTON_STATE::PRESSED:
		offset_.y = size_.y;
		break;
	}
}

void Button::_LateUpdate(float _time)
{
	UI::_LateUpdate(_time);
}

void Button::_Collision(float _time)
{
	UI::_Collision(_time);
}

void Button::_Render(HDC _device_context, float _time)
{
	UI::_Render(_device_context, _time);
}

unique_ptr<Object, function<void(Object*)>> Button::_Clone() const
{
	return unique_ptr<Object, function<void(Object*)>>{new Button{ *this }, [](Object* _p) {
		dynamic_cast<Button*>(_p)->_Release();
		delete dynamic_cast<Button*>(_p);
	}};
}