#pragma once

#include "singleton.h"

class Object;

class CameraManager final : public Singleton<CameraManager>
{
	friend class Singleton<CameraManager>;
public:
	bool Initialize();
	void Update(float _time);

	void Move(TYPE::Point const& _direction, float _time);

	TYPE::Point const& position() const;
	TYPE::Point const& size() const;
	TYPE::Point const& pivot() const;
	TYPE::Point const& world_size() const;
	std::shared_ptr<Object> target() const;

	void set_size(TYPE::Point const& _size);
	void set_pivot(TYPE::Point const& _pivot);
	void set_world_size(TYPE::Point const& _size);
	void set_target(std::shared_ptr<Object> const& _target);

private:
	CameraManager() = default;
	CameraManager(CameraManager const&) = delete;
	CameraManager& operator=(CameraManager const&) = delete;

	virtual void _Release() override;

	void _Calibrate();

	TYPE::Point position_{};
	TYPE::Point size_{};
	TYPE::Point pivot_{};
	TYPE::Point world_size_{};
	std::weak_ptr<Object> target_{};
};