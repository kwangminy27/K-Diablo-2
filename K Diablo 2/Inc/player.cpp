#include "stdafx.h"
#include "player.h"

using namespace std;
using namespace TYPE;

void Player::Move(Point const& _direction, float _time)
{
	static const float kPlayerMoveSpeed = 200.f;

	position_ += _direction * kPlayerMoveSpeed * _time;
}

Player::Player(Player const& _other) : Character(_other)
{
}

Player::Player(Player&& _other) noexcept : Character(move(_other))
{
}

void Player::_Release()
{
}

bool Player::_Initialize()
{
	type_ = OBJECT::CHARACTER;

	return true;
}

void Player::_Input(float _time)
{
	Character::_Input(_time);
}

void Player::_Update(float _time)
{
	Character::_Update(_time);
}

void Player::_LateUpdate(float _time)
{
	Character::_LateUpdate(_time);
}

void Player::_Collision(float _time)
{
	Character::_Collision(_time);
}

void Player::_Render(HDC _device_context, float _time)
{
	Character::_Render(_device_context, _time);
}

unique_ptr<Object, function<void(Object*)>> Player::_Clone() const
{
	return unique_ptr<Object, function<void(Object*)>>{new Player{ *this }, [](Object* _p) {
		dynamic_cast<Player*>(_p)->_Release();
		delete dynamic_cast<Player*>(_p);
	}};
}