#pragma once

#include "effect.h"

class FrozenArmor : public Effect
{
	friend class ObjectManager;
protected:
	FrozenArmor() = default;
	FrozenArmor(FrozenArmor const& _other);
	FrozenArmor(FrozenArmor&& _other) noexcept;
	FrozenArmor& operator=(FrozenArmor const&) = default;
	FrozenArmor& operator=(FrozenArmor&&) noexcept = default;

	virtual void _Release() override;

	virtual bool _Initialize() override;
	virtual void _Input(float _time) override;
	virtual void _Update(float _time) override;
	virtual void _LateUpdate(float _time) override;
	virtual void _Collision(float _time) override;
	virtual void _Render(HDC _device_context, float _time) override;

	virtual std::unique_ptr<Object, std::function<void(Object*)>> _Clone() const override;
};