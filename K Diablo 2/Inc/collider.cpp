#include "stdafx.h"
#include "collider.h"

#include "math.h"

using namespace std;
using namespace TYPE;

string const& Collider::collision_group_tag() const
{
	return collision_group_tag_;
}

COLLIDER Collider::type() const
{
	return type_;
}

Point const& Collider::pivot() const
{
	return pivot_;
}

Point const& Collider::size() const
{
	return size_;
}

Point const& Collider::intersect_point() const
{
	return intersect_point_;
}

list<weak_ptr<Collider>>& Collider::affected_collider_list()
{
	return affected_collider_list_;
}

shared_ptr<Object> Collider::object() const
{
	return object_.lock();
}

void Collider::set_collision_group_tag(string const& _tag)
{
	collision_group_tag_ = _tag;
}

void Collider::set_pivot(Point const& _pivot)
{
	pivot_ = _pivot;
}

void Collider::set_size(Point const& _size)
{
	size_ = _size;
}

void Collider::set_intersect_point(Point const& _point)
{
	intersect_point_ = _point;
}

void Collider::set_object(shared_ptr<Object> const& _object)
{
	object_ = _object;
}

void Collider::SetCallBack(function<void(shared_ptr<Collider> const&, shared_ptr<Collider> const&, float)> const& _function, COLLISION_CALLBACK _type)
{
	collision_callback_array_.at(static_cast<size_t>(_type)).push_back(_function);
}

Collider::Collider(Collider const& _other)
{
	*this = _other;
}

Collider::Collider(Collider&& _other) noexcept
{
	*this = move(_other);
}

void Collider::_Release()
{
	for (auto iter = affected_collider_list_.begin(); iter != affected_collider_list_.end(); ++iter)
		(*iter).lock()->_RemoveAffectedCollider();
}

bool Collider::_Initialize()
{
	return true;
}

bool Collider::_CollisionBetweenPointAndPoint(Point const& _src, Point const& _dest)
{
	return (_src.x == _dest.x) && (_src.y == _dest.y);
}

bool Collider::_CollisionBetweenPointAndRect(Point const & _src, TYPE::Rectangle const& _dest)
{
	if (_src.x < _dest.left)
		return false;

	if (_src.x > _dest.right)
		return false;

	if (_src.y < _dest.top)
		return false;

	if (_src.y > _dest.bottom)
		return false;

	return true;
}

bool Collider::_CollisionBetweenPointAndCircle(Point const& _src, Circle const& _dest)
{
	return Math::GetDistance(_src, _dest.center) <= _dest.radius;
}

bool Collider::_CollisionBetweenPointAndPixel(Point const& _src, shared_ptr<Pixel_24_Info> const& _dest)
{
	return false;
}

bool Collider::_CollisionBetweenRectAndRect(TYPE::Rectangle const& _src, TYPE::Rectangle const& _dest)
{
	if (_src.left > _dest.right)
		return false;

	if (_src.top > _dest.bottom)
		return false;

	if (_src.right < _dest.left)
		return false;

	if (_src.bottom < _dest.top)
		return false;

	intersect_point_.x = (max(_src.left, _dest.left) + max(_src.top, _dest.top)) / 2.f;
	intersect_point_.y = (min(_src.right, _dest.right) + min(_src.bottom, _dest.bottom)) / 2.f;

	return true;
}

bool Collider::_CollisionBetweenRectAndCircle(TYPE::Rectangle const& _src, Circle const& _dest)
{
	float closest_x = clamp(_dest.center.x, _src.left, _src.right);
	float closest_y = clamp(_dest.center.y, _src.top, _src.bottom);

	float distance = Math::GetDistance({ closest_x, closest_y }, _dest.center);

	bool result = distance <= _dest.radius;

	if (result)
	{
		float t = (distance - _dest.radius) / distance;
		intersect_point_.x = closest_x + (_dest.center.x - closest_x) * t;
		intersect_point_.y = closest_y + (_dest.center.y - closest_y) * t;
	}

	return result;
}

bool Collider::_CollisionBetweenRectAndPixel(TYPE::Rectangle const& _src, shared_ptr<Pixel_24_Info> const& _dest)
{
	if (!_dest)
		return false;

	TYPE::Rectangle src_on_the_texture_coordinate_system = { _src.left - _dest->position.x, _src.top - _dest->position.y, _src.right - _dest->position.x, _src.bottom - _dest->position.y };

	float pixel_collider_width = static_cast<float>(_dest->collection.at(0).size());
	float pixel_collider_height = static_cast<float>(_dest->collection.size());

	if (src_on_the_texture_coordinate_system.left > pixel_collider_width ||
		src_on_the_texture_coordinate_system.top > pixel_collider_height ||
		src_on_the_texture_coordinate_system.right < 0.f ||
		src_on_the_texture_coordinate_system.bottom < 0.f)
		return false;

	src_on_the_texture_coordinate_system.left = src_on_the_texture_coordinate_system.left < 0.f ? 0.f : src_on_the_texture_coordinate_system.left;
	src_on_the_texture_coordinate_system.top = src_on_the_texture_coordinate_system.top < 0.f ? 0.f : src_on_the_texture_coordinate_system.top;
	src_on_the_texture_coordinate_system.right = src_on_the_texture_coordinate_system.right > pixel_collider_width ? pixel_collider_width : src_on_the_texture_coordinate_system.right;
	src_on_the_texture_coordinate_system.bottom = src_on_the_texture_coordinate_system.bottom > pixel_collider_height ? pixel_collider_height : src_on_the_texture_coordinate_system.bottom;

	auto const& collection = _dest->collection;
	auto const& criterion = _dest->criterion;
	for (int i = static_cast<int>(src_on_the_texture_coordinate_system.top); i < static_cast<int>(src_on_the_texture_coordinate_system.bottom); ++i)
	{
		for (int j = static_cast<int>(src_on_the_texture_coordinate_system.left); j < static_cast<int>(src_on_the_texture_coordinate_system.right); ++j)
		{
			if (collection.at(i).at(j) == criterion)
			{
				intersect_point_ = { static_cast<float>(j), static_cast<float>(i) };
				return true;
			}
		}
	}

	return false;
}

bool Collider::_CollisionBetweenCircleAndCircle(Circle const& _src, Circle const& _dest)
{
	float distance = Math::GetDistance(_src.center, _dest.center);

	bool result = distance <= _src.radius + _dest.radius;

	if (result)
	{
		float t = _src.radius / distance;
		intersect_point_ = _src.center + (_dest.center - _src.center) * t;
	}

	return result;
}

bool Collider::_CollisionBetweenCircleAndPixel(Circle const& _src, shared_ptr<Pixel_24_Info> const& _dest)
{
	return false;
}

bool Collider::_CollisionBetweenPixelAndPixel(shared_ptr<Pixel_24_Info> const& _src, shared_ptr<Pixel_24_Info> const& _dest)
{
	return false;
}

void Collider::_OnCollisionEnter(shared_ptr<Collider> const& _dest, float _time)
{
	for (auto const& callback : collision_callback_array_.at(static_cast<size_t>(COLLISION_CALLBACK::ENTER)))
		callback(shared_from_this(), _dest, _time);
}

void Collider::_OnCollision(shared_ptr<Collider> const& _dest, float _time)
{
	for (auto const& callback : collision_callback_array_.at(static_cast<size_t>(COLLISION_CALLBACK::STAY)))
		callback(shared_from_this(), _dest, _time);
}

void Collider::_OnCollisionLeave(shared_ptr<Collider> const& _dest, float _time)
{
	for (auto const& callback : collision_callback_array_.at(static_cast<size_t>(COLLISION_CALLBACK::LEAVE)))
		callback(shared_from_this(), _dest, _time);
}

bool Collider::_IsAffectedCollider(shared_ptr<Collider> const& _collider)
{
	for (auto const& affected_collider : affected_collider_list_)
	{
		if (affected_collider.lock() == _collider)
			return true;
	}

	return false;
}

void Collider::_AddAffectedCollider(shared_ptr<Collider> const& _collider)
{
	affected_collider_list_.push_back(_collider);
}

void Collider::_RemoveAffectedCollider(shared_ptr<Collider> const& _collider)
{
	for (auto iter = affected_collider_list_.begin(); iter != affected_collider_list_.end(); ++iter)
	{
		if((*iter).lock() == _collider)
		{
			affected_collider_list_.erase(iter);
			return;
		}
	}
}

void Collider::_RemoveAffectedCollider()
{
	for (auto iter = affected_collider_list_.begin(); iter != affected_collider_list_.end(); ++iter)
	{
		if ((*iter).expired())
		{
			affected_collider_list_.erase(iter);
			return;
		}
	}
}