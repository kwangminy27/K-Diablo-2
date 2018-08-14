#pragma once

#include "tag.h"

class Object;

class Collider : public Tag, public std::enable_shared_from_this<Collider>
{
	friend class ObjectManager;
	friend class Object;
	friend class UI;
	friend class CollisionManager;
public:
	std::string const& collision_group_tag() const;
	COLLIDER type() const;
	TYPE::Point const& pivot() const;
	TYPE::Point const& size() const;
	TYPE::Point const& intersect_point() const;
	std::list<std::weak_ptr<Collider>>& affected_collider_list();
	std::shared_ptr<Object> object() const;

	void set_collision_group_tag(std::string const& _tag);
	void set_pivot(TYPE::Point const& _pivot);
	void set_size(TYPE::Point const& _size);
	void set_intersect_point(TYPE::Point const& _point);
	void set_object(std::shared_ptr<Object> const& _object);

	void SetCallBack(std::function<void(std::weak_ptr<Collider> const&, std::weak_ptr<Collider> const&, float)> const& _function, COLLISION_CALLBACK _type);

protected:
	Collider() = default;
	Collider(Collider const& _other);
	Collider(Collider&& _other) noexcept;
	Collider& operator=(Collider const&) = default;
	Collider& operator=(Collider&&) noexcept = default;
	virtual ~Collider() = default;

	virtual void _Release() override;

	virtual bool _Initialize();
	virtual void _Update(float _time) = 0;
	virtual bool _Collision(std::weak_ptr<Collider> const& _dest) = 0;
	virtual void _Render(HDC _device_context, float _time) = 0;

	virtual std::unique_ptr<Collider, std::function<void(Collider*)>>_Clone() const = 0;

	bool _CollisionBetweenPointAndPoint(TYPE::Point const& _src, TYPE::Point const& _dest);
	bool _CollisionBetweenPointAndRect(TYPE::Point const& _src, TYPE::Rectangle const& _dest);
	bool _CollisionBetweenPointAndCircle(TYPE::Point const& _src, TYPE::Circle const& _dest);
	bool _CollisionBetweenPointAndPixel(TYPE::Point const& _src, std::shared_ptr<TYPE::Pixel_24_Info> const& _dest);
	bool _CollisionBetweenRectAndRect(TYPE::Rectangle const& _src, TYPE::Rectangle const& _dest);
	bool _CollisionBetweenRectAndCircle(TYPE::Rectangle const& _src, TYPE::Circle const& _dest);
	bool _CollisionBetweenRectAndPixel(TYPE::Rectangle const& _src, std::shared_ptr<TYPE::Pixel_24_Info> const& _dest);
	bool _CollisionBetweenCircleAndCircle(TYPE::Circle const& _src, TYPE::Circle const& _dest);
	bool _CollisionBetweenCircleAndPixel(TYPE::Circle const& _src, std::shared_ptr<TYPE::Pixel_24_Info> const& _dest);
	bool _CollisionBetweenPixelAndPixel(std::shared_ptr<TYPE::Pixel_24_Info> const& _src, std::shared_ptr<TYPE::Pixel_24_Info> const& _dest);

	void _OnCollisionEnter(std::shared_ptr<Collider> const& _dest, float _time);
	void _OnCollision(std::shared_ptr<Collider> const& _dest, float _time);
	void _OnCollisionLeave(std::shared_ptr<Collider> const& _dest, float _time);

	bool _IsAffectedCollider(std::shared_ptr<Collider> const& _collider);
	void _AddAffectedCollider(std::shared_ptr<Collider> const& _collider);
	void _RemoveAffectedCollider(std::shared_ptr<Collider> const& _collider);
	void _RemoveAffectedCollider();

	std::string collision_group_tag_{ "Default" };
	COLLIDER type_{};
	TYPE::Point pivot_{};
	TYPE::Point size_{};
	TYPE::Point intersect_point_{};
	std::array<std::list<std::function<void(std::weak_ptr<Collider> const&, std::weak_ptr<Collider> const&, float)>>, static_cast<size_t>(COLLISION_CALLBACK::END)> collision_callback_array_{};
	std::list<std::weak_ptr<Collider>> affected_collider_list_{};
	std::weak_ptr<Object> object_{};
};