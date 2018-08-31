#include "stdafx.h"
#include "monster.h"

#include "math.h"
#include "scene.h"
#include "layer.h"
#include "stage.h"
#include "bar.h"
#include "ui.h"
#include "text.h"
#include "audio_manager.h"
#include "camera_manager.h"

using namespace std;
using namespace TYPE;

float Monster::territory_radius() const
{
	return territory_radius_;
}

shared_ptr<Object> Monster::target() const
{
	return target_.lock();
}

int Monster::dir_idx() const
{
	return dir_idx_;
}

shared_ptr<Object> Monster::stage() const
{
	return stage_.lock();
}

stack<pair<int, int>>& Monster::travel_path_stack()
{
	return travel_path_stack_;
}

float Monster::astar_elapsed_time() const
{
	return astar_elapsed_time_;
}

float Monster::astar_interval() const
{
	return astar_interval_;
}

void Monster::set_territory_radius(float _radius)
{
	territory_radius_ = _radius;
}

void Monster::set_target(shared_ptr<Object> const& _target)
{
	target_ = _target;
}

void Monster::set_dir_idx(int _idx)
{
	dir_idx_ = _idx;
}

void Monster::set_walk_speed(float _speed)
{
	walk_speed_ = _speed;
}

void Monster::set_run_speed(float _speed)
{
	run_speed_ = _speed;
}

void Monster::set_stage(shared_ptr<Object> const& _stage)
{
	stage_ = _stage;
}

void Monster::set_astar_complete_flag(bool _flag)
{
	astar_complete_flag_ = _flag;
}

void Monster::set_arrival_flag(bool _flag)
{
	arrival_flag_ = _flag;
}

void Monster::set_next_target_point(Point const& _point)
{
	next_target_point_ = _point;
}

void Monster::set_final_target_point(Point const& _point)
{
	final_target_point_ = _point;
}

void Monster::set_astar_elapsed_time(float _time)
{
	astar_elapsed_time_ = _time;
}

void Monster::set_astar_interval(float _interval)
{
	astar_interval_ = _interval;
}

void Monster::clear_target()
{
	target_.reset();
}

void Monster::MoveByAStar(float _time)
{
	auto const& caching_stage = dynamic_pointer_cast<Stage>(stage());

	if (astar_complete_flag_)
	{
		if (state_ != CHARACTER_STATE::DEATH && state_ != CHARACTER_STATE::DEAD && state_ != CHARACTER_STATE::ATTACK1)
			set_state(CHARACTER_STATE::ATTACK1);

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
			next_target_point_ = caching_stage->GetTileCenterPosition(travel_path_stack_.top());
			travel_path_stack_.pop();
			arrival_flag_ = false;
		}
	}

	auto dir = (next_target_point_ - position_) / Math::GetDistance(position_, next_target_point_);
	float angle = Math::GetAngle({ 0.f, 0.f }, dir);
	dir_idx_ = static_cast<int>((static_cast<int>(angle + 270.f) % 360) / 45.f);

	float isometric_correction_factor = sqrtf(1.f - cos(Math::ConvertToRadians(angle)) * cos(Math::ConvertToRadians(angle)) * 0.25f) * 0.5f;
	float stride{};

	if (state_ == CHARACTER_STATE::RUN)
		stride = run_speed_ * _time;
	else if(state_ == CHARACTER_STATE::WALK)
		stride = walk_speed_ * _time;

	position_ += {dir.x * stride, dir.y * stride * isometric_correction_factor};

	if (Math::GetDistance(position_, final_target_point_) <= 15.f)
		astar_complete_flag_ = true;

	if (Math::GetDistance(position_, next_target_point_) <= 15.f)
		arrival_flag_ = true;
}

Monster::Monster(Monster const& _other) : Character(_other)
{
	state_ = _other.state_;
}

Monster::Monster(Monster&& _other) noexcept : Character(move(_other))
{
	state_ = move(_other.state_);
}

void Monster::_Release()
{
}

bool Monster::_Initialize()
{
	Object::type_ = OBJECT::MONSTER;
	state_ = CHARACTER_STATE::NEUTRAL;

	astar_interval_ = 0.3f;

	return true;
}

void Monster::_Input(float _time)
{
	Character::_Input(_time);
}

void Monster::_Update(float _time)
{
	auto const& ui_layer = scene()->FindLayer("UI");
	auto const& hell_bovine_hp_bar = dynamic_pointer_cast<Bar>(ui_layer->FindObject("hell_bovine_hp_bar"));
	hell_bovine_hp_bar->set_value(hp_);

	Character::_Update(_time);

	float angle{};

	if (target_.expired())
	{
		neutral_time_ += _time;

		if (neutral_time_ >= 5.f)
		{
			neutral_time_ -= 5.f;

			random_device r;
			default_random_engine gen(r());
			uniform_real_distribution uniform_dist(0.f, 360.f);
			angle = uniform_dist(gen);
			dir_idx_ = static_cast<int>((static_cast<int>(angle + 270.f) % 360) / 45.f);
		}
	}
	else
		MoveByAStar(_time);

	switch (state_)
	{
	case CHARACTER_STATE::NEUTRAL:
		if (state_ == prev_state_)
			ChangeAnimationClipWithDirection(tag() + "_neutral_"s + to_string(dir_idx_));
		else
			ChangeAnimationClip(tag() + "_neutral_"s + to_string(dir_idx_));

		break;
	case CHARACTER_STATE::WALK:
		if (state_ == prev_state_)
			ChangeAnimationClipWithDirection(tag() + "_walk_"s + to_string(dir_idx_));
		else
			ChangeAnimationClip(tag() + "_walk_"s + to_string(dir_idx_));

		break;
	case CHARACTER_STATE::ATTACK1:
		if (state_ == prev_state_)
			ChangeAnimationClipWithDirection(tag() + "_attack1_"s + to_string(dir_idx_));
		else
			ChangeAnimationClip(tag() + "_attack1_"s + to_string(dir_idx_));

		break;
	case CHARACTER_STATE::ATTACK2:
		if (state_ == prev_state_)
			ChangeAnimationClipWithDirection(tag() + "_attack2_"s + to_string(dir_idx_));
		else
			ChangeAnimationClip(tag() + "_attack2_"s + to_string(dir_idx_));

		break;
	case CHARACTER_STATE::GET_HIT:
		if (state_ == prev_state_)
		{
			ChangeAnimationClipWithDirection(tag() + "_get_hit_"s + to_string(dir_idx_));
			SetAnimationCallback(tag() + "_get_hit_"s + to_string(dir_idx_), [this]() {
				random_device r;
				default_random_engine gen(r());
				uniform_int_distribution uniform_dist(1, 4);
				int number = uniform_dist(gen);

				AudioManager::GetSingleton()->FindSoundEffect("hell_bovine_gethit"s + to_string(number))->Play();

				set_state(CHARACTER_STATE::NEUTRAL);
			});
			SetDefaultClip(tag() + "_neutral_"s + to_string(dir_idx_));
		}
		else
		{
			ChangeAnimationClip(tag() + "_get_hit_"s + to_string(dir_idx_));
			SetAnimationCallback(tag() + "_get_hit_"s + to_string(dir_idx_), [this]() {
				random_device r;
				default_random_engine gen(r());
				uniform_int_distribution uniform_dist(1, 4);
				int number = uniform_dist(gen);

				AudioManager::GetSingleton()->FindSoundEffect("hell_bovine_gethit"s + to_string(number))->Play();

				set_state(CHARACTER_STATE::NEUTRAL);
			});
			SetDefaultClip(tag() + "_neutral_"s + to_string(dir_idx_));
		}

		break;
	case CHARACTER_STATE::DEATH:
		if(!death_flag_)
		{
			death_flag_ = true;

			ChangeAnimationClip(tag() + "_death_"s + to_string(dir_idx_));
			SetDefaultClip(tag() + "_dead_"s + to_string(dir_idx_));
		}

		break;
	case CHARACTER_STATE::DEAD:
		break;
	}
}

void Monster::_LateUpdate(float _time)
{
	Character::_LateUpdate(_time);
}

void Monster::_Collision(float _time)
{
	Character::_Collision(_time);
}

void Monster::_Render(HDC _device_context, float _time)
{
	//auto const& camera_position = CameraManager::GetSingleton()->position();

	//if (position_.x < camera_position.x - 500.f || position_.y < camera_position.y - 500.f)
	//	return;

	//if (position_.x > camera_position.x + 1000.f || position_.y > camera_position.y + 1000.f)
	//	return;

	Character::_Render(_device_context, _time);
}

unique_ptr<Object, function<void(Object*)>> Monster::_Clone() const
{
	return unique_ptr<Object, function<void(Object*)>>{new Monster{ *this }, [](Object* _p) {
		dynamic_cast<Monster*>(_p)->_Release();
		delete dynamic_cast<Monster*>(_p);
	}};
}