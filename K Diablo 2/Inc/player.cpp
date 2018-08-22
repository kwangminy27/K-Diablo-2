#include "stdafx.h"
#include "player.h"

#include "math.h"
#include "input_manager.h"
#include "scene.h"
#include "layer.h"
#include "stage.h"
#include "animation.h"
#include "ai_manager.h"

using namespace std;
using namespace TYPE;

PLAYER Player::state() const
{
	return state_;
}

void Player::set_state(PLAYER _state)
{
	state_ = _state;
}

void Player::set_stage(shared_ptr<Object> const& _stage)
{
	stage_ = _stage;
}

void Player::set_astar_interval(float _interval)
{
	astar_interval_ = _interval;
}

void Player::MoveByAStar(float _time)
{
	static const float kPlayerWalkSpeed = 200.f;
	static const float kPlayerRunSpeed = 300.f;

	if (astar_complete_flag_)
	{
		ChangeToDefaultClip();
		return;
	}

	if (travel_path_stack_.empty())
		next_target_point_ = final_target_point_;
	else if (travel_path_stack_.size() == 1)
	{
		next_target_point_ = final_target_point_;
		travel_path_stack_.pop();
	}
	else
	{
		if (test_)
		{
			next_target_point_ = dynamic_pointer_cast<Stage>(stage_)->GetTileCenterPosition(travel_path_stack_.top());
			travel_path_stack_.pop();
			test_ = false;
		}
	}

	auto dir = (next_target_point_ - position_) / Math::GetDistance(position_, next_target_point_);

	switch (state_)
	{
	case PLAYER::WALK:
		position_ += dir * kPlayerWalkSpeed * _time;
		break;
	case PLAYER::RUN:
		position_ += dir * kPlayerRunSpeed * _time;
		break;
	}

	if (Math::GetDistance(position_, final_target_point_) <= 5.f)
		astar_complete_flag_ = true;

	if (Math::GetDistance(position_, next_target_point_) <= 5.f)
		test_ = true;

	/*;

	if (on_final_target_)
		return;

	if (on_next_target_)
	{
		if (travel_path_stack_.empty())
			next_target_point_ = final_target_point_;
		else
		{
			if (travel_path_stack_.size() == 1)
				next_target_point_ = final_target_point_;
			else
				next_target_point_ = dynamic_pointer_cast<Stage>(stage_)->GetTileCenterPosition(travel_path_stack_.top());
			travel_path_stack_.pop();
		}

		on_next_target_ = false;
	}

	auto dir = (next_target_point_ - position_) / Math::GetDistance(position_, next_target_point_);

	switch (state_)
	{
	case PLAYER::WALK:
		position_ += dir * kPlayerWalkSpeed * _time;
		break;
	case PLAYER::RUN:
		position_ += dir * kPlayerRunSpeed * _time;
		break;
	}

	if (Math::GetDistance(position_, next_target_point_) <= 10.f)
		on_next_target_ = true;

	if (Math::GetDistance(position_, final_target_point_) <= 10.f)
		on_final_target_ = true;*/
}

//void Player::MoveByPath(float _time)
//{
//	static const float kPlayerWalkSpeed = 200.f;
//	static const float kPlayerRunSpeed = 300.f;
//
//	if (move_path_stack_.empty())
//		return;
//
//	next_target_position_ = dynamic_pointer_cast<Stage>(stage_)->GetTileCenterPosition(move_path_stack_.top());
//
//	if (arrival_flag_)
//	{
//		move_path_stack_.pop();
//		arrival_flag_ = false;
//	}
//
//	auto dir = (next_target_position_ - position_) / Math::GetDistance(position_, next_target_position_);
//
//	switch (state_)
//	{
//	case PLAYER::WALK:
//		position_ += dir * kPlayerWalkSpeed * _time;
//		break;
//	case PLAYER::RUN:
//		position_ += dir * kPlayerRunSpeed * _time;
//		break;
//	}
//
//	if (Math::GetDistance(position_, next_target_position_) <= 10.f)
//		arrival_flag_ = true;
//}

Player::Player(Player const& _other) : Character(_other)
{
	state_ = _other.state_;
}

Player::Player(Player&& _other) noexcept : Character(move(_other))
{
	state_ = move(_other.state_);
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
	static float astar_elapsed_time = 0.f;

	Character::_Input(_time);

	auto const& input_manager = InputManager::GetSingleton();
	auto const& ai_manager = AIManager::GetSingleton();

	auto mouse_position = InputManager::GetSingleton()->mouse_world_position();
	float angle = Math::GetAngle(position_, mouse_position);

	int dir_idx = static_cast<int>((static_cast<int>(angle + 270.f) % 360) / 22.5f); // 0 ~ 15

	string town_neutral = "town_neutral_" + to_string(dir_idx);
	string town_walk = "town_walk_" + to_string(dir_idx);
	string run = "run_" + to_string(dir_idx);

	if (input_manager->KeyPush("MouseLeft"))
	{
		switch (state_)
		{
		case PLAYER::WALK:
			ChangeAnimationClip(town_walk.c_str());
			break;
		case PLAYER::RUN:
			ChangeAnimationClip(run.c_str());
			break;
		}
		SetDefaultClip(town_neutral.c_str());

		travel_path_stack_ = ai_manager->ProcessAStar(position_, mouse_position, dynamic_pointer_cast<Stage>(stage_));
		if (!travel_path_stack_.empty())
		{
			astar_complete_flag_ = false;
			test_ = false;
			next_target_point_ = dynamic_pointer_cast<Stage>(stage_)->GetTileCenterPosition(travel_path_stack_.top());
			final_target_point_ = mouse_position;
		}
	}
	else if (input_manager->KeyPressed("MouseLeft"))
	{
		ChangeAnimationDirection(dir_idx);
		SetDefaultClip(town_neutral.c_str());

		astar_elapsed_time += _time;
		if (astar_elapsed_time >= astar_interval_)
		{
			astar_elapsed_time -= astar_interval_;

			travel_path_stack_ = ai_manager->ProcessAStar(position_, mouse_position, dynamic_pointer_cast<Stage>(stage_));
			if (!travel_path_stack_.empty())
			{
				astar_complete_flag_ = false;
				test_ = false;
				next_target_point_ = dynamic_pointer_cast<Stage>(stage_)->GetTileCenterPosition(travel_path_stack_.top());
				final_target_point_ = mouse_position;
			}
		}
	}

	if (input_manager->KeyPush("RunToggle"))
		state_ = static_cast<PLAYER>(static_cast<bool>(state_) ^ true);
}

void Player::_Update(float _time)
{
	Character::_Update(_time);

	MoveByAStar(_time);
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