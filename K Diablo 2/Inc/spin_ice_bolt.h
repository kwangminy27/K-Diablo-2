#pragma once

#include "missile.h"

class SpinIceBolt : public Missile
{
	friend class ObjectManager;
private:
	SpinIceBolt() = default;
	SpinIceBolt(SpinIceBolt const& _other);
	SpinIceBolt(SpinIceBolt&& _other) noexcept;
	SpinIceBolt& operator=(SpinIceBolt const&) = default;
	SpinIceBolt& operator=(SpinIceBolt&&) noexcept = default;

	virtual void _Release() override;

	virtual bool _Initialize() override;
	virtual void _Input(float _time) override;
	virtual void _Update(float _time) override;
	virtual void _LateUpdate(float _time) override;
	virtual void _Collision(float _time) override;
	virtual void _Render(HDC _device_context, float _time) override;

	virtual std::unique_ptr<Object, std::function<void(Object*)>> _Clone() const override;

	bool spin_point_1_{ true };
	bool spin_point_2_{ true };
};