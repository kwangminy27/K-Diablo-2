#pragma once

#include "missile.h"

class Nova final : public Missile
{
	friend class ObjectManager;
private:
	Nova() = default;
	Nova(Nova const& _other);
	Nova(Nova&& _other) noexcept;
	Nova& operator=(Nova const&) = default;
	Nova& operator=(Nova&&) noexcept = default;

	virtual void _Release() override;

	virtual bool _Initialize() override;
	virtual void _Input(float _time) override;
	virtual void _Update(float _time) override;
	virtual void _LateUpdate(float _time) override;
	virtual void _Collision(float _time) override;
	virtual void _Render(HDC _device_context, float _time) override;

	virtual std::unique_ptr<Object, std::function<void(Object*)>> _Clone() const override;
};