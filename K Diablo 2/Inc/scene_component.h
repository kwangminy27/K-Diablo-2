#pragma once

#include "tag.h"

class Scene;

class SceneComponent : public Tag
{
	friend class SceneManager;
	friend class Scene;
public:
	std::shared_ptr<Scene> scene() const;

	void set_scene(std::shared_ptr<Scene> const& _scene);

protected:
	SceneComponent() = default;
	SceneComponent(SceneComponent const& _other);
	SceneComponent(SceneComponent&& _other) noexcept;
	SceneComponent& operator=(SceneComponent const&) = default;
	SceneComponent& operator=(SceneComponent&&) noexcept = default;
	virtual ~SceneComponent() = default;

	virtual void _Release() = 0;

	virtual bool _Initialize() = 0;
	virtual void _Input(float _time) = 0;
	virtual void _Update(float _time) = 0;
	virtual void _LateUpdate(float _time) = 0;
	virtual void _Collision(float _time) = 0;
	virtual void _Render(HDC _device_context, float _time) = 0;

	std::weak_ptr<Scene> scene_{};
};