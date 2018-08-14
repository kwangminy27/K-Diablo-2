#include "stdafx.h"
#include "input_manager.h"

using namespace std;

bool InputManager::Initialize()
{
	_AddKey("Pause"s, VK_F1);

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