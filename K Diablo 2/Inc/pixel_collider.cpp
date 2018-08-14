#include "stdafx.h"
#include "pixel_collider.h"

#include "core.h"
#include "math.h"
#include "camera_manager.h"
#include "object.h"
#include "collision_manager.h"
#include "point_collider.h"
#include "rect_collider.h"
#include "circle_collider.h"

using namespace std;
using namespace TYPE;

Point const& PixelCollider::position() const
{
	return pixel_collider_.lock()->position;
}

Pixel_24 const& PixelCollider::criterion() const
{
	return pixel_collider_.lock()->criterion;
}

shared_ptr<Pixel_24_Info> PixelCollider::pixel_collider() const
{
	return pixel_collider_.lock();
}

void PixelCollider::set_position(Point const& _position)
{
	pixel_collider_.lock()->position = _position;
}

void PixelCollider::set_criterion(Pixel_24 const& _criterion)
{
	pixel_collider_.lock()->criterion = _criterion;
}

void PixelCollider::set_pixel_collider(string const& _tag)
{
	pixel_collider_ = CollisionManager::GetSingleton()->FindPixelCollider(_tag);
}

PixelCollider::PixelCollider(PixelCollider const& _other) : Collider(_other)
{
	pixel_collider_ = _other.pixel_collider_;
}

PixelCollider::PixelCollider(PixelCollider&& _other) noexcept : Collider(move(_other))
{
	pixel_collider_ = move(_other.pixel_collider_);
}

void PixelCollider::_Release()
{
	Collider::_Release();
}

bool PixelCollider::_Initialize()
{
	type_ = COLLIDER::PIXEL;

	return true;
}

void PixelCollider::_Update(float _time)
{
	auto caching_object = object();
	auto object_position = caching_object->position();

	if (caching_object->type() == OBJECT::UI)
		object_position += CameraManager::GetSingleton()->position();

	pixel_collider()->position.x = object_position.x - pixel_collider()->collection.at(0).size() * pivot_.x;
	pixel_collider()->position.y = object_position.y - pixel_collider()->collection.size() * pivot_.y;
}

bool PixelCollider::_Collision(weak_ptr<Collider> const& _dest)
{
	auto caching_dest = _dest.lock();

	switch (caching_dest->type())
	{
	case COLLIDER::POINT:
		return _CollisionBetweenPointAndPixel(dynamic_pointer_cast<PointCollider>(caching_dest)->world_info(), pixel_collider_.lock());
	case COLLIDER::RECT:
		return _CollisionBetweenRectAndPixel(dynamic_pointer_cast<RectCollider>(caching_dest)->world_info(), pixel_collider_.lock());
	case COLLIDER::CIRCLE:
		return _CollisionBetweenCircleAndPixel(dynamic_pointer_cast<CircleCollider>(caching_dest)->world_info(), pixel_collider_.lock());
	case COLLIDER::PIXEL:
		return _CollisionBetweenPixelAndPixel(pixel_collider_.lock(), dynamic_pointer_cast<PixelCollider>(caching_dest)->pixel_collider_.lock());
	}

	return false;
}

void PixelCollider::_Render(HDC _device_context, float _time)
{
}

unique_ptr<Collider, function<void(Collider*)>> PixelCollider::_Clone()
{
	return unique_ptr<Collider, function<void(Collider*)>>{new PixelCollider(*this), [](Collider* _p) {
		dynamic_cast<PixelCollider*>(_p)->_Release();
		delete dynamic_cast<PixelCollider*>(_p);
	}};
}