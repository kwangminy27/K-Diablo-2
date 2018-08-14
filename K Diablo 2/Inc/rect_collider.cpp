#include "stdafx.h"
#include "rect_collider.h"

#include "core.h"
#include "math.h"
#include "camera_manager.h"
#include "object.h"
#include "point_collider.h"
#include "circle_collider.h"
#include "pixel_collider.h"

using namespace std;
using namespace TYPE;

TYPE::Rectangle const& RectCollider::world_info() const
{
	return world_info_;
}

void RectCollider::set_model_info(TYPE::Rectangle const& _info)
{
	model_info_ = _info;

	size_.x = _info.right - _info.left;
	size_.y = _info.bottom - _info.top;
}

RectCollider::RectCollider(RectCollider const& _other) : Collider(_other)
{
	model_info_ = _other.model_info_;
	world_info_ = _other.world_info_;
}

RectCollider::RectCollider(RectCollider&& _other) noexcept : Collider(move(_other))
{
	model_info_ = move(_other.model_info_);
	world_info_ = move(_other.world_info_);
}

void RectCollider::_Release()
{
	Collider::_Release();
}

bool RectCollider::_Initialize()
{
	type_ = COLLIDER::RECT;

	return true;
}

void RectCollider::_Update(float _time)
{
	auto caching_object = object();
	auto object_position = caching_object->position();

	if (caching_object->type() == OBJECT::UI)
		object_position += CameraManager::GetSingleton()->position();

	size_.x = model_info_.right - model_info_.left;
	size_.y = model_info_.bottom - model_info_.top;

	world_info_.left = object_position.x + model_info_.left - pivot_.x * size_.x;
	world_info_.top = object_position.y + model_info_.top - pivot_.y * size_.y;
	world_info_.right = world_info_.left + size_.x;
	world_info_.bottom = world_info_.top + size_.y;
}

bool RectCollider::_Collision(weak_ptr<Collider> const& _dest)
{
	auto caching_dest = _dest.lock();

	switch (caching_dest->type())
	{
	case COLLIDER::POINT:
		return _CollisionBetweenPointAndRect(dynamic_pointer_cast<PointCollider>(caching_dest)->world_info(), world_info_);
	case COLLIDER::RECT:
		return _CollisionBetweenRectAndRect(world_info_, dynamic_pointer_cast<RectCollider>(caching_dest)->world_info_);
	case COLLIDER::CIRCLE:
		return _CollisionBetweenRectAndCircle(world_info_, dynamic_pointer_cast<CircleCollider>(caching_dest)->world_info());
	case COLLIDER::PIXEL:
		return _CollisionBetweenRectAndPixel(world_info_, dynamic_pointer_cast<PixelCollider>(caching_dest)->pixel_collider());
	}

	return false;
}

void RectCollider::_Render(HDC _device_context, float _time)
{
#ifdef _DEBUG
	if (affected_collider_list_.empty())
		brush_ = Core::GetSingleton()->brush(COLOR::GREEN);
	else
		brush_ = Core::GetSingleton()->brush(COLOR::BLUE);

	auto camera_position = CameraManager::GetSingleton()->position();

	auto rendering_rect = world_info_;
	rendering_rect.left -= camera_position.x;
	rendering_rect.right -= camera_position.x;
	rendering_rect.top -= camera_position.y;
	rendering_rect.bottom -= camera_position.y;

	RECT rc{ static_cast<long>(rendering_rect.left), static_cast<long>(rendering_rect.top), static_cast<long>(rendering_rect.right), static_cast<long>(rendering_rect.bottom) };
	FrameRect(_device_context, &rc, brush_);
#endif
}

unique_ptr<Collider, function<void(Collider*)>> RectCollider::_Clone() const
{
	return unique_ptr<Collider, function<void(Collider*)>>{new RectCollider, [](Collider* _p) {
		dynamic_cast<RectCollider*>(_p)->_Release();
		delete dynamic_cast<RectCollider*>(_p);
	}};
}