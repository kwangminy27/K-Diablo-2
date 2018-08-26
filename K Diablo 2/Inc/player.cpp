#include "stdafx.h"
#include "player.h"

#include "math.h"
#include "input_manager.h"
#include "scene.h"
#include "layer.h"
#include "object_manager.h"
#include "stage.h"
#include "missile.h"
#include "animation.h"
#include "animation_clip.h"
#include "audio_manager.h"
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
		if (state_ != PLAYER::CASTING)
			ChangeToDefaultClip();

		auto const& audio_manager = AudioManager::GetSingleton();

		audio_manager->RemoveSoundEffectInstance("LightDirtRun1");
		audio_manager->RemoveSoundEffectInstance("LightDirt1");

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

	if (run_flag_)
		position_ += dir * kPlayerRunSpeed * _time;
	else
		position_ += dir * kPlayerWalkSpeed * _time;

	if (Math::GetDistance(position_, final_target_point_) <= 1.f)
		astar_complete_flag_ = true;

	if (Math::GetDistance(position_, next_target_point_) <= 1.f)
		test_ = true;
}

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
	auto const& audio_manager = AudioManager::GetSingleton();

	auto mouse_position = InputManager::GetSingleton()->mouse_world_position();
	float angle = Math::GetAngle(position_, mouse_position);

	int dir_idx = static_cast<int>((static_cast<int>(angle + 281.25) % 360) / 22.5f); // 0 ~ 15

	string town_neutral_tag = "town_neutral_" + to_string(dir_idx);
	string town_walk_tag = "town_walk_" + to_string(dir_idx);
	string run_tag = "run_" + to_string(dir_idx);
	string skill_casting_tag = "skill_casting_" + to_string(dir_idx);
	string skill_casting_special_tag = "skill_casting_special_" + to_string(dir_idx);
	string ice_bolt_tag = "ice_bolt_" + to_string(dir_idx);

	if (input_manager->KeyPush("MouseLeft"))
	{
		switch (state_)
		{
		case PLAYER::IDLE:
			if (run_flag_)
			{
				auto LightDirtRun1 = audio_manager->FindSoundEffect("LightDirtRun1")->CreateInstance();
				LightDirtRun1->SetVolume(5.f);
				LightDirtRun1->SetPitch(-0.75f);
				LightDirtRun1->Play(true);
				audio_manager->AddSoundEffectInstance("LightDirtRun1", move(LightDirtRun1));

				ChangeAnimationClip(run_tag.c_str());
			}
			else
			{
				auto LightDirt1 = audio_manager->FindSoundEffect("LightDirt1")->CreateInstance();
				LightDirt1->SetVolume(5.f);
				LightDirt1->SetPitch(0.f);
				LightDirt1->Play(true);
				audio_manager->AddSoundEffectInstance("LightDirt1", move(LightDirt1));

				ChangeAnimationClip(town_walk_tag.c_str());
			}

			SetDefaultClip(town_neutral_tag.c_str());

			travel_path_stack_ = ai_manager->ProcessAStar(position_, mouse_position, dynamic_pointer_cast<Stage>(stage_));
			if (!travel_path_stack_.empty())
			{
				astar_complete_flag_ = false;
				test_ = false;
				next_target_point_ = dynamic_pointer_cast<Stage>(stage_)->GetTileCenterPosition(travel_path_stack_.top());
				final_target_point_ = mouse_position;
			}
			break;
		case PLAYER::CASTING:
			break;
		}
	}
	else if (input_manager->KeyPressed("MouseLeft"))
	{
		switch (state_)
		{
		case PLAYER::IDLE:
			if (run_flag_)
			{
				if(animation_->current_clip_->tag() == town_neutral_tag)
					ChangeAnimationClip(run_tag.c_str());
				else
					ChangeAnimationClipWithDirection(run_tag.c_str());
			}
			else
			{
				if (animation_->current_clip_->tag() == town_neutral_tag)
					ChangeAnimationClip(town_walk_tag.c_str());
				else
					ChangeAnimationClipWithDirection(town_walk_tag.c_str());
			}
			SetDefaultClip(town_neutral_tag.c_str());

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
			break;
		case PLAYER::CASTING:
			break;
		}
	}

	if (input_manager->KeyPush("RunToggle"))
	{
		switch (state_)
		{
		case PLAYER::IDLE:
			if (run_flag_)
			{
				run_flag_ = false;

				audio_manager->RemoveSoundEffectInstance("LightDirtRun1");

				auto LightDirt1 = audio_manager->FindSoundEffect("LightDirt1")->CreateInstance();
				LightDirt1->SetVolume(5.f);
				LightDirt1->SetPitch(0.f);
				LightDirt1->Play(true);
				audio_manager->AddSoundEffectInstance("LightDirt1", move(LightDirt1));

				ChangeAnimationClip(town_walk_tag.c_str());
			}
			else
			{
				run_flag_ = true;

				audio_manager->RemoveSoundEffectInstance("LightDirt1");

				auto LightDirtRun1 = audio_manager->FindSoundEffect("LightDirtRun1")->CreateInstance();
				LightDirtRun1->SetVolume(5.f);
				LightDirtRun1->SetPitch(-0.75f);
				LightDirtRun1->Play(true);
				audio_manager->AddSoundEffectInstance("LightDirtRun1", move(LightDirtRun1));

				ChangeAnimationClip(run_tag.c_str());
			}
			SetDefaultClip(town_neutral_tag.c_str());
		case PLAYER::CASTING:
			break;
		}
	}

	if (input_manager->KeyPush("MouseRight"))
	{
		AudioManager::GetSingleton()->FindSoundEffect("coldcast")->Play();

		auto ice_cast_new_1 = layer()->FindObject("ice_cast_new_1");
		ice_cast_new_1->set_position(position_ - Point{ 50.f, 70.f });
		ice_cast_new_1->set_enablement(true);

		state_ = PLAYER::CASTING;

		astar_complete_flag_ = true;

		ChangeAnimationClip(skill_casting_special_tag.c_str());
		SetDefaultClip(town_neutral_tag.c_str());

		SetAnimationCallback(skill_casting_special_tag.c_str(), [this, _ice_bolt_tag = ice_bolt_tag, _angle = angle]() {
			auto ice_bolt = dynamic_pointer_cast<Missile>(ObjectManager::GetSingleton()->CreateCloneObject("ice_bolt", layer()));
			ice_bolt->set_position(position_ + Point{ -50.f, -50.f });
			ice_bolt->AddAnimationClip(_ice_bolt_tag);
			ice_bolt->set_dir({ cos(Math::ConvertToRadians(_angle)), sin(Math::ConvertToRadians(_angle)) });
		});
	}
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