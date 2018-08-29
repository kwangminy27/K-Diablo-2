#include "stdafx.h"
#include "player.h"

#include "math.h"
#include "input_manager.h"
#include "scene.h"
#include "layer.h"
#include "object_manager.h"
#include "stage.h"
#include "missile.h"
#include "effect.h"
#include "frozen_armor.h"
#include "frozen_orb.h"
#include "nova.h"
#include "point_collider.h"
#include "text.h"
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

shared_ptr<Object> Player::stage() const
{
	return stage_.lock();
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

void Player::set_skill(SKILL _skill)
{
	skill_ = _skill;

	auto const& ui_layer = scene()->FindLayer("UI");

	auto right_skill_tap = ui_layer->FindObject("right_skill_tap");
	auto right_skill_tap_text = dynamic_pointer_cast<Text>(ui_layer->FindObject("right_skill_tap_text"));

	switch (skill_)
	{
	case SKILL::ICE_BOLT:
		right_skill_tap->set_texture("ice_bolt_icon");
		right_skill_tap_text->set_string("ICE BOLT");
		break;
	case SKILL::ICE_BLAST:
		right_skill_tap->set_texture("ice_blast_icon");
		right_skill_tap_text->set_string("ICE_BLAST");
		break;
	case SKILL::FROST_NOVA:
		right_skill_tap->set_texture("frost_nova_icon");
		right_skill_tap_text->set_string("FROST_NOVA");
		break;
	case SKILL::FROZEN_ORB:
		right_skill_tap->set_texture("frozen_orb_icon");
		right_skill_tap_text->set_string("FROZEN_ORB");
		break;
	case SKILL::FROZEN_ARMOR:
		right_skill_tap->set_texture("frozen_armor_icon");
		right_skill_tap_text->set_string("FROZEN_ARMOR");
		break;
	}
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
		if (arrival_flag_)
		{
			next_target_point_ = dynamic_pointer_cast<Stage>(stage())->GetTileCenterPosition(travel_path_stack_.top());
			travel_path_stack_.pop();
			arrival_flag_ = false;
		}
	}

	auto dir = (next_target_point_ - position_) / Math::GetDistance(position_, next_target_point_);
	float angle = Math::GetAngle({ 0.f, 0.f }, dir);
	float isometric_correction_factor = sqrtf(1.f - cos(Math::ConvertToRadians(angle)) * cos(Math::ConvertToRadians(angle)) * 0.25f) * 0.5f;
	float stride{};

	if (run_flag_)
		stride = kPlayerRunSpeed * _time;
	else
		stride = kPlayerWalkSpeed * _time;

	position_ += {dir.x * stride, dir.y * stride * isometric_correction_factor};

	if (Math::GetDistance(position_, final_target_point_) <= 1.f)
		astar_complete_flag_ = true;

	if (Math::GetDistance(position_, next_target_point_) <= 1.f)
		arrival_flag_ = true;
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

	auto const& caching_stage = stage();

	auto mouse_position = InputManager::GetSingleton()->mouse_world_position();
	float angle = Math::GetAngle(position_, mouse_position);

	int dir_idx = static_cast<int>((static_cast<int>(angle + 281.25f) % 360) / 22.5f); // 0 ~ 15

	string town_neutral_tag = "town_neutral_" + to_string(dir_idx);
	string town_walk_tag = "town_walk_" + to_string(dir_idx);
	string run_tag = "run_" + to_string(dir_idx);
	string skill_casting_tag = "skill_casting_" + to_string(dir_idx);
	string skill_casting_special_tag = "skill_casting_special_" + to_string(dir_idx);
	string ice_bolt_tag = "ice_bolt_" + to_string(dir_idx);
	string ice_blast_tag = "ice_blast_" + to_string(dir_idx);

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

			travel_path_stack_ = ai_manager->ProcessAStar(position_, mouse_position, dynamic_pointer_cast<Stage>(caching_stage));
			if (!travel_path_stack_.empty())
			{
				astar_complete_flag_ = false;
				arrival_flag_ = false;
				next_target_point_ = dynamic_pointer_cast<Stage>(caching_stage)->GetTileCenterPosition(travel_path_stack_.top());
				final_target_point_ = mouse_position;
			}
			break;
		case PLAYER::CASTING:
			break;
		}
	}
	else if (input_manager->KeyPressed("MouseLeft"))
	{
		bool is_neutral = false;
		string animation_tag = animation_->current_clip_->tag();

		for (auto i = 0; i < 16; ++i)
		{
			if (animation_tag == "town_neutral_"s + to_string(i))
				is_neutral = true;
		}

		switch (state_)
		{
		case PLAYER::IDLE:
			if (run_flag_)
			{
				if(is_neutral)
					ChangeAnimationClip(run_tag.c_str());
				else
					ChangeAnimationClipWithDirection(run_tag.c_str());
			}
			else
			{
				if (is_neutral)
					ChangeAnimationClip(town_walk_tag.c_str());
				else
					ChangeAnimationClipWithDirection(town_walk_tag.c_str());
			}
			SetDefaultClip(town_neutral_tag.c_str());

			astar_elapsed_time += _time;
			if (astar_elapsed_time >= astar_interval_)
			{
				astar_elapsed_time -= astar_interval_;

				travel_path_stack_ = ai_manager->ProcessAStar(position_, mouse_position, dynamic_pointer_cast<Stage>(caching_stage));
				if (!travel_path_stack_.empty())
				{
					astar_complete_flag_ = false;
					arrival_flag_ = false;
					next_target_point_ = dynamic_pointer_cast<Stage>(caching_stage)->GetTileCenterPosition(travel_path_stack_.top());
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
		state_ = PLAYER::CASTING;

		astar_complete_flag_ = true;

		switch (skill_)
		{
		case SKILL::ICE_BOLT:
		{
			AudioManager::GetSingleton()->FindSoundEffect("coldcast")->Play();

			auto ice_cast_new_1 = scene()->FindLayer("UI")->FindObject("ice_cast_new_1");
			ice_cast_new_1->set_position(position_ - Point{ 50.f, 112.f });
			ice_cast_new_1->set_enablement(true);

			ChangeAnimationClip(skill_casting_special_tag.c_str());
			SetDefaultClip(town_neutral_tag.c_str());

			SetAnimationCallback(skill_casting_special_tag.c_str(), [this, _ice_bolt_tag = ice_bolt_tag, _angle = angle, _dir_idx = dir_idx]() {
				random_device r;
				default_random_engine gen(r());
				uniform_int_distribution uniform_dist(1, 3);
				auto i = uniform_dist(gen);

				AudioManager::GetSingleton()->FindSoundEffect("icebolt"s + to_string(i))->Play();

				auto ice_bolt = dynamic_pointer_cast<Missile>(ObjectManager::GetSingleton()->CreateCloneObject("ice_bolt", layer()));
				ice_bolt->set_position(position_ - Point{ 50.f, 42.f });
				ice_bolt->AddAnimationClip(_ice_bolt_tag);
				ice_bolt->set_dir({ cos(Math::ConvertToRadians(_angle)), sin(Math::ConvertToRadians(_angle)) });

				auto ice_bolt_collider = dynamic_pointer_cast<PointCollider>(ice_bolt->AddCollider<PointCollider>("ice_bolt_collider"));

				switch (_dir_idx)
				{
				case 0:
					ice_bolt_collider->set_model_info({ 52.f, 10.f });
					break;
				case 1:
					ice_bolt_collider->set_model_info({ 31.f, 9.f });
					break;
				case 2:
					ice_bolt_collider->set_model_info({ 15.f, -4.f });
					break;
				case 3:
					ice_bolt_collider->set_model_info({ 7.f, -17.f });
					break;
				case 4:
					ice_bolt_collider->set_model_info({ 1.f, -21.f });
					break;
				case 5:
					ice_bolt_collider->set_model_info({ -5.f, -28.f });
					break;
				case 6:
					ice_bolt_collider->set_model_info({ 7.f, -35.f });
					break;
				case 7:
					ice_bolt_collider->set_model_info({ 20.f, -40.f });
					break;
				case 8:
					ice_bolt_collider->set_model_info({ 50.f, -54.f });
					break;
				case 9:
					ice_bolt_collider->set_model_info({ 82.f, -44.f });
					break;
				case 10:
					ice_bolt_collider->set_model_info({ 96.f, -37.f });
					break;
				case 11:
					ice_bolt_collider->set_model_info({ 103.f, -28.f });
					break;
				case 12:
					ice_bolt_collider->set_model_info({ 102.f, -20.f });
					break;
				case 13:
					ice_bolt_collider->set_model_info({ 93.f, -17.f });
					break;
				case 14:
					ice_bolt_collider->set_model_info({ 85.f, -4.f });
					break;
				case 15:
					ice_bolt_collider->set_model_info({ 71.f, 7.f });
					break;
				}
			});
		}
			break;
		case SKILL::FROZEN_ORB:
		{
			AudioManager::GetSingleton()->FindSoundEffect("coldcast")->Play();

			auto ice_cast_new_3 = scene()->FindLayer("UI")->FindObject("ice_cast_new_3");
			ice_cast_new_3->set_position(position_ - Point{ 65.f, 112.f });
			ice_cast_new_3->set_enablement(true);

			ChangeAnimationClip(skill_casting_special_tag.c_str());
			SetDefaultClip(town_neutral_tag.c_str());

			SetAnimationCallback(skill_casting_special_tag.c_str(), [this, _angle = angle]() {
				AudioManager::GetSingleton()->FindSoundEffect("blizzloop")->Play();

				auto ice_orb = dynamic_pointer_cast<FrozenOrb>(ObjectManager::GetSingleton()->CreateCloneObject("ice_orb", layer()));

				auto offset = Point{ cos(Math::ConvertToRadians(_angle)), sin(Math::ConvertToRadians(_angle)) };

				ice_orb->set_position(position_ - Point{ 25.f, 67.f } + offset * 40.f);
				ice_orb->AddAnimationClip("ice_orb");
				ice_orb->set_dir({ cos(Math::ConvertToRadians(_angle)), sin(Math::ConvertToRadians(_angle)) });
				ice_orb->set_move_range(300.f);
				ice_orb->set_move_speed(200.f);
			});
		}
			break;
		case SKILL::FROZEN_ARMOR:
		{
			AudioManager::GetSingleton()->FindSoundEffect("coldcast")->Play();

			auto ice_cast_new_3 = scene()->FindLayer("UI")->FindObject("ice_cast_new_3");
			ice_cast_new_3->set_position(position_ - Point{ 65.f, 112.f });
			ice_cast_new_3->set_enablement(true);

			ChangeAnimationClip(skill_casting_special_tag.c_str());
			SetDefaultClip(town_neutral_tag.c_str());

			SetAnimationCallback(skill_casting_special_tag.c_str(), [this]() {
				AudioManager::GetSingleton()->FindSoundEffect("frozenarmor")->Play();

				auto frozen_armor = layer()->FindObject("frozen_armor");
				frozen_armor->set_enablement(true);
			});
		}
			break;
		case SKILL::FROST_NOVA:
		{
			AudioManager::GetSingleton()->FindSoundEffect("coldcast")->Play();

			auto ice_cast_new_2 = scene()->FindLayer("UI")->FindObject("ice_cast_new_2");
			ice_cast_new_2->set_position(position_ - Point{ 65.f, 112.f });
			ice_cast_new_2->set_enablement(true);

			ChangeAnimationClip(skill_casting_tag.c_str());
			SetDefaultClip(town_neutral_tag.c_str());

			SetAnimationCallback(skill_casting_tag.c_str(), [this]() {
				AudioManager::GetSingleton()->FindSoundEffect("novaice")->Play();

				auto frost_nova = dynamic_pointer_cast<Nova>(ObjectManager::GetSingleton()->CreateCloneObject("frost_nova", layer()));
				frost_nova->set_position(position_ - Point{ 60.f, 52.f });
			});
		}
			break;
		case SKILL::ICE_BLAST:
		{
			AudioManager::GetSingleton()->FindSoundEffect("coldcast")->Play();

			auto ice_cast_new_1 = scene()->FindLayer("UI")->FindObject("ice_cast_new_1");
			ice_cast_new_1->set_position(position_ - Point{ 50.f, 112.f });
			ice_cast_new_1->set_enablement(true);

			ChangeAnimationClip(skill_casting_special_tag.c_str());
			SetDefaultClip(town_neutral_tag.c_str());

			SetAnimationCallback(skill_casting_special_tag.c_str(), [this, _ice_blast_tag = ice_blast_tag, _angle = angle, _dir_idx = dir_idx]() {
				random_device r;
				default_random_engine gen(r());
				uniform_int_distribution uniform_dist(1, 3);
				auto i = uniform_dist(gen);

				AudioManager::GetSingleton()->FindSoundEffect("icebolt"s + to_string(i))->Play();

				auto ice_blast = dynamic_pointer_cast<Missile>(ObjectManager::GetSingleton()->CreateCloneObject("ice_blast", layer()));
				ice_blast->set_position(position_ - Point{ 50.f, 42.f });
				ice_blast->AddAnimationClip(_ice_blast_tag);
				ice_blast->set_color_key(RGB(1, 1, 1));
				ice_blast->set_dir({ cos(Math::ConvertToRadians(_angle)), sin(Math::ConvertToRadians(_angle)) });

				auto ice_blast_collider = dynamic_pointer_cast<PointCollider>(ice_blast->AddCollider<PointCollider>("ice_blast_collider"));

				switch (_dir_idx)
				{
				case 0:
					ice_blast_collider->set_model_info({ 52.f, 38.f });
					break;
				case 1:
					ice_blast_collider->set_model_info({ 35.f, 18.f });
					break;
				case 2:
					ice_blast_collider->set_model_info({ 10.f, 0.f });
					break;
				case 3:
					ice_blast_collider->set_model_info({ 3.f, -5.f });
					break;
				case 4:
					ice_blast_collider->set_model_info({ -17.f, -15.f });
					break;
				case 5:
					ice_blast_collider->set_model_info({ -5.f, -28.f });
					break;
				case 6:
					ice_blast_collider->set_model_info({ 5.f, -40.f });
					break;
				case 7:
					ice_blast_collider->set_model_info({ 30.f, -45.f });
					break;
				case 8:
					ice_blast_collider->set_model_info({ 55.f, -55.f });
					break;
				case 9:
					ice_blast_collider->set_model_info({ 85.f, -35.f });
					break;
				case 10:
					ice_blast_collider->set_model_info({ 105.f, -30.f });
					break;
				case 11:
					ice_blast_collider->set_model_info({ 115.f, -30.f });
					break;
				case 12:
					ice_blast_collider->set_model_info({ 145.f, -20.f });
					break;
				case 13:
					ice_blast_collider->set_model_info({ 125.f, -5.f });
					break;
				case 14:
					ice_blast_collider->set_model_info({ 95.f, 0.f });
					break;
				case 15:
					ice_blast_collider->set_model_info({ 85.f, 0.f });
					break;
				}
			});
		}
			break;
		};
	}

	if (input_manager->KeyPush("Skill1"))
		set_skill(SKILL::ICE_BOLT);

	if (input_manager->KeyPush("Skill2"))
		set_skill(SKILL::ICE_BLAST);

	if (input_manager->KeyPush("Skill3"))
		set_skill(SKILL::FROST_NOVA);

	if (input_manager->KeyPush("Skill4"))
		set_skill(SKILL::FROZEN_ORB);
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