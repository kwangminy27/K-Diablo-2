#include "stdafx.h"
#include "circle_collider.h"

#include "core.h"
#include "math.h"
#include "camera_manager.h"
#include "object.h"
#include "point_collider.h"
#include "rect_collider.h"
#include "pixel_collider.h"

using namespace std;
using namespace TYPE;

Circle const& CircleCollider::world_info() const
{
	return world_info_;
}

void CircleCollider::set_model_info(Circle const& _info)
{
	model_info_ = _info;
}

CircleCollider::CircleCollider(CircleCollider const& _other)
{
	model_info_ = _other.model_info_;
	world_info_ = _other.world_info_;
}

CircleCollider::CircleCollider(CircleCollider&& _other) noexcept
{
	model_info_ = move(_other.model_info_);
	world_info_ = move(_other.world_info_);
}

void CircleCollider::_Release()
{
	Collider::_Release();
}

bool CircleCollider::_Initialize()
{
	type_ = COLLIDER::CIRCLE;

	return true;
}

void CircleCollider::_Update(float _time)
{
	auto caching_object = object();
	auto object_position = caching_object->position();

	if (caching_object->type() == OBJECT::UI)
		object_position += CameraManager::GetSingleton()->position();

	world_info_.center.x = object_position.x + model_info_.center.x + model_info_.radius * pivot_.x;
	world_info_.center.y = object_position.y + model_info_.center.y + model_info_.radius * pivot_.y;

	world_info_.radius = model_info_.radius;
}

bool CircleCollider::_Collision(weak_ptr<Collider> const & _dest)
{
	auto caching_dest = _dest.lock();

	switch (caching_dest->type())
	{
	case COLLIDER::POINT:
		return _CollisionBetweenPointAndCircle(dynamic_pointer_cast<PointCollider>(caching_dest)->world_info(), world_info_);
	case COLLIDER::RECT:
		return _CollisionBetweenRectAndCircle(dynamic_pointer_cast<RectCollider>(caching_dest)->world_info(), world_info_);
	case COLLIDER::CIRCLE:
		return _CollisionBetweenCircleAndCircle(world_info_, dynamic_pointer_cast<CircleCollider>(caching_dest)->world_info_);
	case COLLIDER::PIXEL:
		return _CollisionBetweenCircleAndPixel(world_info_, dynamic_pointer_cast<PixelCollider>(caching_dest)->pixel_collider());
	}

	return false;
}

void CircleCollider::_Render(HDC _device_context, float _time)
{
#ifdef _DEBUG
	if (affected_collider_list_.empty())
		pen_ = Core::GetSingleton()->pen(COLOR::GREEN);
	else
		pen_ = Core::GetSingleton()->pen(COLOR::RED);

	auto camera_position = CameraManager::GetSingleton()->position();

	old_pen_ = static_cast<HPEN>(SelectObject(_device_context, pen_));

	auto rendering_position = world_info_.center - camera_position;

	MoveToEx(_device_context, static_cast<int>(rendering_position.x + world_info_.radius), static_cast<int>(rendering_position.y), nullptr);
	for (auto i = 30; i <= 360; i += 30)
	{
		rendering_position.x = world_info_.center.x - camera_position.x + cosf(Math::ConvertToRadians(static_cast<float>(i))) * world_info_.radius;
		rendering_position.y = world_info_.center.y - camera_position.y + sinf(Math::ConvertToRadians(static_cast<float>(i))) * world_info_.radius;

		LineTo(_device_context, static_cast<int>(rendering_position.x), static_cast<int>(rendering_position.y));
	}

	SelectObject(_device_context, old_pen_);
#endif
}

unique_ptr<Collider, function<void(Collider*)>> CircleCollider::_Clone() const
{
	return unique_ptr<Collider, function<void(Collider*)>>{new CircleCollider, [](Collider* _p) {
		dynamic_cast<CircleCollider*>(_p)->_Release();
		delete dynamic_cast<CircleCollider*>(_p);
	}};
}