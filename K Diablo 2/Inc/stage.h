#pragma once

#include "object.h"

class Stage final : public Object
{
	friend class ObjectManager;
private:
	Stage() = default;
	Stage(Stage const& _other);
	Stage(Stage&& _other) noexcept;
	Stage& operator=(Stage const&) = default;
	Stage& operator=(Stage&&) noexcept = default;

	virtual void _Release() override;

	virtual bool _Initialize() override;
	virtual void _Input(float _time) override;
	virtual void _Update(float _time) override;
	virtual void _LateUpdate(float _time) override;
	virtual void _Collision(float _time) override;
	virtual void _Render(HDC _device_context, float _time) override;

	virtual std::unique_ptr<Object, std::function<void(Object*)>> _Clone() const override;
};