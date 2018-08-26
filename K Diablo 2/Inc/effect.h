#pragma once

#include "object.h"

class Effect : public Object
{
	friend class ObjectManager;
protected:
	Effect() = default;
	Effect(Effect const& _other);
	Effect(Effect&& _other) noexcept;
	Effect& operator=(Effect const&) = default;
	Effect& operator=(Effect&&) noexcept = default;

	virtual void _Release() override;

	virtual bool _Initialize() override;
	virtual void _Input(float _time) override;
	virtual void _Update(float _time) override;
	virtual void _LateUpdate(float _time) override;
	virtual void _Collision(float _time) override;
	virtual void _Render(HDC _device_context, float _time) override;

	virtual std::unique_ptr<Object, std::function<void(Object*)>> _Clone() const override;
};