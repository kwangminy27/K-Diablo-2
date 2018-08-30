#pragma once

#include "object.h"

class Bar : public Object
{
	friend class ObjectManager;
public:
	void set_range(std::pair<float, float> const& _range);
	void set_value(float _value);
	void set_cutting_direction(BAR_CUTTING_DIRECTION _direction);
	void set_ui_flag(bool _flag);

	void AddValue(float _value);

protected:
	Bar() = default;
	Bar(Bar const& _other);
	Bar(Bar&& _other) noexcept;
	Bar& operator=(Bar const&) = default;
	Bar& operator=(Bar&&) noexcept = default;

	virtual void _Release() override;

	virtual bool _Initialize() override;
	virtual void _Input(float _time) override;
	virtual void _Update(float _time) override;
	virtual void _LateUpdate(float _time) override;
	virtual void _Collision(float _time) override;
	virtual void _Render(HDC _device_context, float _time) override;

	std::unique_ptr<Object, std::function<void(Object*)>> _Clone() const override;

	std::pair<float, float> range_{};
	float value_{};
	BAR_CUTTING_DIRECTION cutting_direction_{ BAR_CUTTING_DIRECTION::LEFT };
	bool ui_flag_{};
};