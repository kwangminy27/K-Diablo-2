#pragma once

#include "object.h"

class Character : public Object
{
	friend class ObjectManager;
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
};