#pragma once

#include "object.h"

class Missile : public Object
{
	friend class ObjectManager;
public:
	void set_dir(TYPE::Point _dir);
	void set_move_speed(float _speed);
	void set_move_range(float _range);

protected:
	Missile() = default;
	Missile(Missile const& _other);
	Missile(Missile&& _other) noexcept;
	Missile& operator=(Missile const&) = default;
	Missile& operator=(Missile&&) noexcept = default;

	virtual void _Release() override;

	virtual bool _Initialize() override;
	virtual void _Input(float _time) override;
	virtual void _Update(float _time) override;
	virtual void _LateUpdate(float _time) override;
	virtual void _Collision(float _time) override;
	virtual void _Render(HDC _device_context, float _time) override;

	virtual std::unique_ptr<Object, std::function<void(Object*)>> _Clone() const override;

	TYPE::Point dir_{};
	float move_speed_{};
	float move_range_{};
};