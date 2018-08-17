#pragma once

#include "object.h"

class UI : public Object
{
	friend class ObjectManager;
public:
	void set_offset_flag(bool _flag);

protected:
	UI() = default;
	UI(UI const& _other);
	UI(UI&& _other) noexcept;
	UI& operator=(UI const&) = default;
	UI& operator=(UI&&) noexcept = default;

	virtual void _Release() override;

	virtual bool _Initialize() override;
	virtual void _Input(float _time) override;
	virtual void _Update(float _time) override;
	virtual void _LateUpdate(float _time) override;
	virtual void _Collision(float _time) override;
	virtual void _Render(HDC _device_context, float _time) override;

	virtual std::unique_ptr<Object, std::function<void(Object*)>> _Clone() const override;

	TYPE::Point offset_{};
	bool offset_flag_{};
};