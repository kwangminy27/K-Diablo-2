#include "stdafx.h"
#include "melee_monster.h"

#include "math.h"

using namespace std;
using namespace TYPE;

MeleeMonster::MeleeMonster(MeleeMonster const& _other) : Monster(_other)
{
}

MeleeMonster::MeleeMonster(MeleeMonster&& _other) noexcept : Monster(move(_other))
{
}

void MeleeMonster::_Release()
{
}

bool MeleeMonster::_Initialize()
{
	Monster::_Initialize();

	return true;
}

void MeleeMonster::_Input(float _time)
{
	Monster::_Input(_time);
}

void MeleeMonster::_Update(float _time)
{
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
	case MONSTER_STATE::NEUTRAL:
		if (state_ == prev_state_)
			ChangeAnimationClipWithDirection(tag() + "_neutral_"s + to_string(dir_idx_));
		else
			ChangeAnimationClip(tag() + "_neutral_"s + to_string(dir_idx_));
		break;
	case MONSTER_STATE::WALK:
		if (state_ == prev_state_)
			ChangeAnimationClipWithDirection(tag() + "_walk_"s + to_string(dir_idx_));
		else
			ChangeAnimationClip(tag() + "_walk_"s + to_string(dir_idx_));
		break;
	case MONSTER_STATE::ATTACK1:
		if (state_ == prev_state_)
			ChangeAnimationClipWithDirection(tag() + "_attack1_"s + to_string(dir_idx_));
		else
			ChangeAnimationClip(tag() + "_attack1_"s + to_string(dir_idx_));
		break;
	case MONSTER_STATE::ATTACK2:
		if (state_ == prev_state_)
			ChangeAnimationClipWithDirection(tag() + "_attack2_"s + to_string(dir_idx_));
		else
			ChangeAnimationClip(tag() + "_attack2_"s + to_string(dir_idx_));
		break;
	case MONSTER_STATE::GET_HIT:
		break;
	case MONSTER_STATE::DEATH:
		break;
	case MONSTER_STATE::DEAD:
		break;
	}

	Monster::_Update(_time);
}

void MeleeMonster::_LateUpdate(float _time)
{
	Monster::_LateUpdate(_time);
}

void MeleeMonster::_Collision(float _time)
{
	Monster::_Collision(_time);
}

void MeleeMonster::_Render(HDC _device_context, float _time)
{
	Monster::_Render(_device_context, _time);
}

unique_ptr<Object, function<void(Object*)>> MeleeMonster::_Clone() const
{
	return unique_ptr<Object, function<void(Object*)>>{new MeleeMonster{ *this }, [](Object* _p) {
		dynamic_cast<MeleeMonster*>(_p)->_Release();
		delete dynamic_cast<MeleeMonster*>(_p);
	}};
}