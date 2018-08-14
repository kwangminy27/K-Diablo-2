#pragma once

#include "tag.h"

class SceneComponent;
class Layer;

class Scene final : public Tag, public std::enable_shared_from_this<Scene>
{
	friend class SceneManager;
public:
	std::shared_ptr<Layer> const& FindLayer(std::string const& _tag) const;

private:
	Scene() = default;
	Scene(Scene const& _other) = delete;
	Scene(Scene&& _other) noexcept;
	Scene& operator=(Scene const&) = delete;
	Scene& operator=(Scene&&) noexcept = default;

	virtual void _Release() override;

	bool _Initialize();
	void _Input(float _time);
	void _Update(float _time);
	void _LateUpdate(float _time);
	void _Collision(float _time);
	void _Render(HDC _device_context, float _time);

	template <typename T> std::unique_ptr<SceneComponent, std::function<void(SceneComponent*)>> _CreateSceneComponent(std::string const& _tag);
	bool _CreateLayer(std::string const& _tag, int _z_order = 0);

	std::shared_ptr<Layer> layer_nullptr_{};

	std::unique_ptr<SceneComponent, std::function<void(SceneComponent*)>> scene_component_{};
	std::list<std::shared_ptr<Layer>> layer_list_{};
};

#include "scene.inl"