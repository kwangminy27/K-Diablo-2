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
	return false;
}

void Button::_Input(float _time)
{
	UI::_Input(_time);

	auto const& input_manager = InputManager::GetSingleton();

	if (state_ != BUTTON_STATE::NORMAL)
	{
		if (input_manager->KeyPush("LeftButton"))
			state_ = BUTTON_STATE::CLICK;
		else if (input_manager->KeyUp("LeftButton"))
		{
			for (auto const& callback : callback_list_)
				callback(_time);

			state_ = BUTTON_STATE::MOUSEON;
		}
	}
}

void Button::_Update(float _time)
{
	UI::_Update(_time);
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

void Button::_OnCollisionEnter(weak_ptr<Collider> const& _src, weak_ptr<Collider> const& _dest, float _time)
{
	auto caching_src = _src.lock();
	auto caching_dest = _dest.lock();

	auto src_tag = caching_src->tag();
	auto dest_tag = caching_dest->tag();

	if ((src_tag == "MouseCursor" && dest_tag == "Button") || (src_tag == "Button" && dest_tag == "MouseCursor"))
		dynamic_pointer_cast<Button>(caching_dest->object())->set_state(BUTTON_STATE::MOUSEON);
}

void Button::_OnCollision(weak_ptr<Collider> const& _src, weak_ptr<Collider> const& _dest, float _time)
{
}

void Button::_OnCollisionLeave(weak_ptr<Collider> const& _src, weak_ptr<Collider> const& _dest, float _time)
{
	auto caching_src = _src.lock();
	auto caching_dest = _dest.lock();

	auto src_tag = caching_src->tag();
	auto dest_tag = caching_dest->tag();

	if ((src_tag == "MouseCursor" && dest_tag == "Button") || (src_tag == "Button" && dest_tag == "MouseCursor"))
		dynamic_pointer_cast<Button>(caching_dest->object())->set_state(BUTTON_STATE::NORMAL);
}