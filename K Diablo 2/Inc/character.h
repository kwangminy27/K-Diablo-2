#pragma once

#include "object.h"

class Character : public Object
{
	friend class ObjectManager;
public:
	CHARACTER_STATE state() const;
	float hp() const;
	float max_hp() const;
	float mp() const;
	float max_mp() const;

	void set_hp(float _hp);
	void set_max_hp(float _hp);
	void set_mp(float _mp);
	void set_max_mp(float _mp);
	void set_state(CHARACTER_STATE _state);
	void set_dir_idx(int _idx);
	void set_death_flag(bool _flag);

	void AddHp(float _amount);
	void AddMp(float _amount);

protected:
	Character() = default;
	Character(Character const& _other);
	Character(Character&& _other) noexcept;
	Character& operator=(Character const&) = default;
	Character& operator=(Character&&) noexcept = default;

	virtual void _Release() override;

	virtual bool _Initialize() override;
	virtual void _Input(float _time) override;
	virtual void _Update(float _time) override;
	virtual void _LateUpdate(float _time) override;
	virtual void _Collision(float _time) override;
	virtual void _Render(HDC _device_context, float _time) override;

	virtual std::unique_ptr<Object, std::function<void(Object*)>> _Clone() const override;

	CHARACTER_STATE state_;
	CHARACTER_STATE prev_state_;
	float hp_{};
	float max_hp_{};
	float mp_{};
	float max_mp_{};
	int dir_idx_;
	bool death_flag_{};
};