#pragma once

#include "singleton.h"

class Scene;

class SceneManager final : public Singleton<SceneManager>
{
	friend class Singleton<SceneManager>;
public:
	bool Initialize();
	void Input(float _time);
	void Update(float _time);
	void LateUpdate(float _time);
	void Collision(float _time);
	void Render(HDC _device_context, float _time);

	template <typename T> bool CreateNextScene(std::string const& _tag);
	void TrySceneChange();

private:
	SceneManager() = default;
	SceneManager(SceneManager const&) = delete;
	SceneManager& operator=(SceneManager const&) = delete;

	virtual void _Release() override;

	std::shared_ptr<Scene> _CreateScene(std::string const& _tag);

	std::shared_ptr<Scene> scene_nullptr_{};

	std::shared_ptr<Scene> scene_{};
	std::shared_ptr<Scene> next_scene_{};
};

#include "scene_manager.inl"