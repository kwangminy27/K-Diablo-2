#pragma once

#include "collider.h"

class RectCollider : public Collider
{
	friend class Object;
public:
	TYPE::Rectangle const& world_info() const;

	void set_model_info(TYPE::Rectangle const& _info);

private:
	RectCollider() = default;
	RectCollider(RectCollider const& _other);
	RectCollider(RectCollider&& _other) noexcept;
	RectCollider& operator=(RectCollider const&) = default;
	RectCollider& operator=(RectCollider&&) noexcept = default;

	virtual void _Release() override;

	virtual bool _Initialize() override;
	virtual void _Update(float _time) override;
	virtual bool _Collision(std::weak_ptr<Collider> const& _dest) override;
	virtual void _Render(HDC _device_context, float _time) override;

	virtual std::unique_ptr<Collider, std::function<void(Collider*)>> _Clone() const override;

	HBRUSH brush_{};
	TYPE::Rectangle model_info_{};
	TYPE::Rectangle world_info_{};
};