#include "stdafx.h"
#include "point_collider.h"

#include "core.h"
#include "math.h"
#include "camera_manager.h"
#include "object.h"
#include "rect_collider.h"
#include "circle_collider.h"
#include "pixel_collider.h"

using namespace std;
using namespace TYPE;

Point const& PointCollider::world_info() const
{
	return world_info_;
}

void PointCollider::set_model_info(Point const& _info)
{
	model_info_ = _info;
}

PointCollider::PointCollider(PointCollider const& _other) : Collider(_other)
{
	model_info_ = _other.model_info_;
	world_info_ = _other.world_info_;
}

PointCollider::PointCollider(PointCollider&& _other) noexcept : Collider(move(_other))
{
	model_info_ = move(_other.model_info_);
	world_info_ = move(_other.world_info_);
}

void PointCollider::_Release()
{
	Collider::_Release();
}

bool PointCollider::_Initialize()
{
	type_ = COLLIDER::POINT;

	return true;
}

void PointCollider::_Update(float _time)
{
	auto caching_object = object();
	auto object_position = caching_object->position();

	if (caching_object->type() == OBJECT::UI)
		object_position += CameraManager::GetSingleton()->position();

	world_info_ = model_info_ + object_position;
}

bool PointCollider::_Collision(weak_ptr<Collider> const& _dest)
{
	auto caching_dest = _dest.lock();

	switch (caching_dest->type())
	{
	case COLLIDER::POINT:
		return _CollisionBetweenPointAndPoint(world_info_, dynamic_pointer_cast<PointCollider>(caching_dest)->world_info_);
	case COLLIDER::RECT:
		return _CollisionBetweenPointAndRect(world_info_, dynamic_pointer_cast<RectCollider>(caching_dest)->world_info());
	case COLLIDER::CIRCLE:
		return _CollisionBetweenPointAndCircle(world_info_, dynamic_pointer_cast<CircleCollider>(caching_dest)->world_info());
	case COLLIDER::PIXEL:
		return _CollisionBetweenPointAndPixel(world_info_, dynamic_pointer_cast<PixelCollider>(caching_dest)->pixel_collider());
	}

	return false;
}

void PointCollider::_Render(HDC _device_context, float _time)
{
	if (tag_ == "ice_blast_collider")
		int a = 3;

#ifdef _DEBUG
	if (object_.expired())
		return;

	if (affected_collider_list_.empty())
		pen_ = Core::GetSingleton()->pen(COLOR::GREEN);
	else
		pen_ = Core::GetSingleton()->pen(COLOR::RED);

	auto camera_position = CameraManager::GetSingleton()->position();

	old_pen_ = static_cast<HPEN>(SelectObject(_device_context, pen_));

	auto rendering_position = world_info_ - camera_position;

	MoveToEx(_device_context, static_cast<int>(rendering_position.x + 1), static_cast<int>(rendering_position.y), nullptr);
	for (auto i = 60; i <= 360; i += 60)
	{
		rendering_position.x = world_info_.x - camera_position.x + cosf(Math::ConvertToRadians(static_cast<float>(i))) * 1;
		rendering_position.y = world_info_.y - camera_position.y + sinf(Math::ConvertToRadians(static_cast<float>(i))) * 1;

		LineTo(_device_context, static_cast<int>(rendering_position.x), static_cast<int>(rendering_position.y));
	}

	SelectObject(_device_context, old_pen_);
#endif
}

unique_ptr<Collider, function<void(Collider*)>> PointCollider::_Clone() const
{
	return unique_ptr<Collider, function<void(Collider*)>>{new PointCollider(*this), [](Collider* _p) {
		dynamic_cast<PointCollider*>(_p)->_Release();
		delete dynamic_cast<PointCollider*>(_p);
	}};
}