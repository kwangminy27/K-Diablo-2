#include "stdafx.h"
#include "scene_component.h"

#include "scene.h"

using namespace std;

shared_ptr<Scene> SceneComponent::scene() const
{
	return scene_.lock();
}

void SceneComponent::set_scene(shared_ptr<Scene> const& _scene)
{
	scene_ = _scene;
}

SceneComponent::SceneComponent(SceneComponent const& _other)
{
	*this = _other;
}

SceneComponent::SceneComponent(SceneComponent&& _other) noexcept
{
	*this = move(_other);
}