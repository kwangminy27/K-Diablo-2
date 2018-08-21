#include "stdafx.h"
#include "camera_manager.h"

#include "object.h"

using namespace std;
using namespace TYPE;

bool CameraManager::Initialize()
{
	set_size({ static_cast<float>(RESOLUTION::WIDTH), static_cast<float>(RESOLUTION::HEIGHT) });
	set_pivot({ 0.5f, 0.5f });

	return true;
}

void CameraManager::Update(float _time)
{
	if (target_.expired())
		return;

	Point target_position = target_.lock()->position();
	position_ = target_position - size_ * pivot_;

	_Calibrate();
}

void CameraManager::Move(Point const& _direction, float _time)
{
	static float const kMoveSpeed = 1000.f;

	position_.x += _direction.x * kMoveSpeed * _time;
	position_.y += _direction.y * kMoveSpeed * _time;

	_Calibrate();
}

Point const& CameraManager::position() const
{
	return position_;
}

Point const& CameraManager::size() const
{
	return size_;
}

Point const& CameraManager::pivot() const
{
	return pivot_;
}

Point const& CameraManager::world_size() const
{
	return world_size_;
}

shared_ptr<Object> CameraManager::target() const
{
	return target_.lock();
}

void CameraManager::set_size(Point const& _size)
{
	size_ = _size;
}

void CameraManager::set_pivot(Point const& _pivot)
{
	pivot_ = _pivot;
}

void CameraManager::set_world_size(Point const& _size)
{
	world_size_ = _size;
}

void CameraManager::set_target(shared_ptr<Object> const& _target)
{
	target_ = _target;
}

void CameraManager::_Release()
{
}

void CameraManager::_Calibrate()
{
	float x_upper_bound = (world_size_.x - size_.x) * (1.f - numeric_limits<float>::epsilon()) < 0.f ? 0.f : (world_size_.x - size_.x) * (1.f - numeric_limits<float>::epsilon());
	float y_upper_bound = (world_size_.y - size_.y) * (1.f - numeric_limits<float>::epsilon()) < 0.f ? 0.f : (world_size_.y - size_.y) * (1.f - numeric_limits<float>::epsilon());

	position_.x = clamp(position_.x, 0.f, x_upper_bound);
	position_.y = clamp(position_.y, 0.f, y_upper_bound);
}