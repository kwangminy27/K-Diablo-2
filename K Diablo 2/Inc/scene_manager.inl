#pragma once

using namespace std;

template<typename T>
bool SceneManager::CreateNextScene(string const& _tag)
{
	if (!(next_scene_ = _CreateScene(_tag)))
		return false;

	if (!(next_scene_->scene_component_ = next_scene_->_CreateSceneComponent<T>(_tag)))
		return false;

	return true;
}