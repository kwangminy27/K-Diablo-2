#pragma once

#include "tag.h"

class Scene;
class Object;

class Layer final : public Tag
{
	friend class SceneManager;
	friend class Scene;
	friend class ObjectManager;
public:
	int z_order() const;
	std::shared_ptr<Scene> scene() const;

	void set_z_order(int _z_order);
	void set_scene(std::shared_ptr<Scene> const& _scene);

	std::shared_ptr<Object> const& FindObject(std::string const& _tag) const;

private:
	Layer() = default;
	Layer(Layer const& _other);
	Layer(Layer&& _other) noexcept;
	Layer& operator=(Layer const&) = default;
	Layer& operator=(Layer&&) noexcept = default;

	virtual void _Release() override;

	bool _Initialize();
	void _Input(float _time);
	void _Update(float _time);
	void _LateUpdate(float _time);
	void _Collision(float _time);
	void _Render(HDC _device_context, float _time);

	void _AddObject(std::shared_ptr<Object> const& _object);

	std::shared_ptr<Object> object_nullptr_{};

	int z_order_{};
	std::weak_ptr<Scene> scene_{};
	std::list<std::shared_ptr<Object>> object_list_{};
};