#include "stdafx.h"
#include "input_manager.h"

#include "core.h"
#include "camera_manager.h"
#include "object.h"

using namespace std;
using namespace TYPE;

bool InputManager::Initialize()
{
	_AddKey("Pause"s, VK_F1);
	_AddKey("MouseLeft"s, VK_LBUTTON);

	ShowCursor(false);

	return true;
}

void InputManager::Update()
{
	for (auto const& key : key_map_)
	{
		int count{};
		for (auto const& value : key.second->element)
		{
			if (GetAsyncKeyState(value) & 0x8000)
				++count;
		}

		if (count == key.second->element.size())
		{
			if (!key.second->pressed)
			{
				key.second->push = true;
				key.second->pressed = true;
			}
			else
				key.second->push = false;
		}
		else if (key.second->pressed)
		{
			key.second->push = false;
			key.second->pressed = false;
			key.second->up = true;
		}
		else if (key.second->up)
			key.second->up = false;
	}

	UpdateMouseCursor();
}

bool InputManager::KeyPush(string const& _tag) const
{
	auto const& key = _FindKey(_tag);

	return key->push;
}

bool InputManager::KeyPressed(string const& _tag) const
{
	auto const& key = _FindKey(_tag);

	return key->pressed;
}

bool InputManager::KeyUp(string const& _tag) const
{
	auto const& key = _FindKey(_tag);

	return key->up;
}

shared_ptr<Object> InputManager::mouse() const
{
	return mouse_;
}

Point const& InputManager::mouse_client_position() const
{
	return mouse_client_position_;
}

Point const& InputManager::mouse_world_position() const
{
	return mouse_world_position_;
}

Point const& InputManager::mouse_displacement() const
{
	return mouse_displacement_;
}

void InputManager::set_mouse(shared_ptr<Object> const& _mouse)
{
	mouse_ = _mouse;
}

void InputManager::UpdateMouseCursor()
{
	POINT position{};
	GetCursorPos(&position);
	ScreenToClient(Core::GetSingleton()->window(), &position);

	mouse_displacement_.x = static_cast<float>(position.x) - mouse_client_position_.x;
	mouse_displacement_.y = static_cast<float>(position.y) - mouse_client_position_.y;

	mouse_client_position_ = { static_cast<float>(position.x), static_cast<float>(position.y) };
	mouse_world_position_ = mouse_client_position_ + CameraManager::GetSingleton()->position();
	mouse_->set_position(mouse_client_position_);

	if (!mouse_cursor_show_state_ && mouse_client_position_.x <= 0.f && mouse_client_position_.x >= static_cast<float>(RESOLUTION::WIDTH) && mouse_client_position_.y <= 0.f && mouse_client_position_.y >= static_cast<float>(RESOLUTION::HEIGHT))
	{
		mouse_cursor_show_state_ = true;
		ShowCursor(true);
	}
	else if (mouse_cursor_show_state_ && mouse_client_position_.x > 0.f && mouse_client_position_.x < static_cast<float>(RESOLUTION::WIDTH) && mouse_client_position_.y > 0.f && mouse_client_position_.y < static_cast<float>(RESOLUTION::HEIGHT))
	{
		mouse_cursor_show_state_ = false;
		ShowCursor(false);
	}
}

void InputManager::RenderMouseCursor(HDC _device_context, float _time)
{
	mouse_->_Render(_device_context, _time);
}

void InputManager::_Release()
{
}

void InputManager::_AddKey()
{
	key_map_.insert(make_pair(move(key_tag_buffer_), move(key_buffer_)));
}

unique_ptr<KeyInfo> const& InputManager::_FindKey(string const& _tag) const
{
	auto iter = key_map_.find(_tag);

	if (iter == key_map_.end())
		return key_nullptr_;

	return iter->second;
}