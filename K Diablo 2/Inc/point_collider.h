#pragma once

#include "collider.h"

class PointCollider : public Collider
{
	friend class Object;
public:
	TYPE::Point const& world_info() const;

	void set_model_info(TYPE::Point const& _info);

private:
	PointCollider() = default;
	PointCollider(PointCollider const& _other);
	PointCollider(PointCollider&& _other) noexcept;
	PointCollider& operator=(PointCollider const&) = default;
	PointCollider& operator=(PointCollider&&) noexcept = default;

	virtual void _Release() override;

	virtual bool _Initialize() override;
	virtual void _Update(float _time) override;
	virtual bool _Collision(std::weak_ptr<Collider> const& _dest) override;
	virtual void _Render(HDC _device_context, float _time) override;

	virtual std::unique_ptr<Collider, std::function<void(Collider*)>> _Clone() const override;

	HPEN pen_{};
	HPEN old_pen_{};
	TYPE::Point model_info_{};
	TYPE::Point world_info_{};
};