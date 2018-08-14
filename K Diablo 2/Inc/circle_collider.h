#pragma once

#include "collider.h"

class CircleCollider : public Collider
{
	friend class Object;
public:
	TYPE::Circle const& world_info() const;

	void set_model_info(TYPE::Circle const& _info);

private:
	CircleCollider() = default;
	CircleCollider(CircleCollider const& _other);
	CircleCollider(CircleCollider&& _other) noexcept;
	CircleCollider& operator=(CircleCollider const&) = default;
	CircleCollider& operator=(CircleCollider&&) noexcept = default;

	virtual void _Release() override;

	virtual bool _Initialize() override;
	virtual void _Update(float _time) override;
	virtual bool _Collision(std::weak_ptr<Collider> const& _dest) override;
	virtual void _Render(HDC _device_context, float _time) override;

	virtual std::unique_ptr<Collider, std::function<void(Collider*)>> _Clone() const override;

	HPEN pen_{};
	HPEN old_pen_{};
	TYPE::Circle model_info_{};
	TYPE::Circle world_info_{};
};