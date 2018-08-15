#pragma once

#include "ui.h"

class Button : public UI
{
	friend class ObjectManager;
public:
	void set_state(BUTTON_STATE _state);
	void set_callback(std::function<void(float)> const& _callback);

	void OnCollisionEnter(std::shared_ptr<Collider> const& _src, std::shared_ptr<Collider> const& _dest, float _time);
	void OnCollision(std::shared_ptr<Collider> const& _src, std::shared_ptr<Collider> const& _dest, float _time);
	void OnCollisionLeave(std::shared_ptr<Collider> const& _src, std::shared_ptr<Collider> const& _dest, float _time);

protected:
	Button() = default;
	Button(Button const& _other);
	Button(Button&& _other) noexcept;
	Button& operator=(Button const&) = default;
	Button& operator=(Button&&) noexcept = default;

	virtual void _Release() override;

	virtual bool _Initialize() override;
	virtual void _Input(float _time) override;
	virtual void _Update(float _time) override;
	virtual void _LateUpdate(float _time) override;
	virtual void _Collision(float _time) override;
	virtual void _Render(HDC _device_context, float _time) override;

	virtual std::unique_ptr<Object, std::function<void(Object*)>> _Clone() const override;

	BUTTON_STATE state_{};
	std::list<std::function<void(float)>> callback_list_{};
};