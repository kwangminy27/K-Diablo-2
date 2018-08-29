#pragma once

#include "missile.h"

class FrozenOrb final : public Missile
{
	friend class ObjectManager;
private:
	FrozenOrb() = default;
	FrozenOrb(FrozenOrb const& _other);
	FrozenOrb(FrozenOrb&& _other) noexcept;
	FrozenOrb& operator=(FrozenOrb const&) = default;
	FrozenOrb& operator=(FrozenOrb&&) noexcept = default;

	virtual void _Release() override;

	virtual bool _Initialize() override;
	virtual void _Input(float _time) override;
	virtual void _Update(float _time) override;
	virtual void _LateUpdate(float _time) override;
	virtual void _Collision(float _time) override;
	virtual void _Render(HDC _device_context, float _time) override;

	virtual std::unique_ptr<Object, std::function<void(Object*)>> _Clone() const override;

	float life_time_{};
};