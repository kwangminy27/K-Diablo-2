#pragma once

#include "collider.h"

class PixelCollider : public Collider
{
	friend class Object;
public:
	TYPE::Point const& position() const;
	TYPE::Pixel_24 const& criterion() const;
	std::shared_ptr<TYPE::Pixel_24_Info> pixel_collider() const;

	void set_position(TYPE::Point const& _info);
	void set_criterion(TYPE::Pixel_24 const& _criterion);
	void set_pixel_collider(std::string const& _tag);

private:
	PixelCollider() = default;
	PixelCollider(PixelCollider const& _other);
	PixelCollider(PixelCollider&& _other) noexcept;
	PixelCollider& operator=(PixelCollider const&) = default;
	PixelCollider& operator=(PixelCollider&&) noexcept = default;

	virtual void _Release() override;

	virtual bool _Initialize() override;
	virtual void _Update(float _time) override;
	virtual bool _Collision(std::weak_ptr<Collider> const& _dest) override;
	virtual void _Render(HDC _device_context, float _time) override;

	virtual std::unique_ptr<Collider, std::function<void(Collider*)>> _Clone() override;

	std::weak_ptr<TYPE::Pixel_24_Info> pixel_collider_{};
};