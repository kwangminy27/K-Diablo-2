#pragma once

using namespace std;

template <typename T>
unique_ptr<SceneComponent, function<void(SceneComponent*)>> Scene::_CreateSceneComponent(string const& _tag)
{
	auto scene_component = unique_ptr<SceneComponent, function<void(SceneComponent*)>>(new T, [](SceneComponent* _p) {
		_p->_Release();
		delete _p;
	});

	scene_component->set_tag(_tag);
	scene_component->set_scene(shared_from_this());

	if (!scene_component->_Initialize())
		return unique_ptr<SceneComponent, function<void(SceneComponent*)>>{};

	return scene_component;
}