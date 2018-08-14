#include "stdafx.h"
#include "scene.h"

#include "scene_component.h"
#include "layer.h"

using namespace std;

shared_ptr<Layer> const& Scene::FindLayer(string const& _tag) const
{
	for (auto const& layer : layer_list_)
	{
		if (layer->tag() == _tag)
			return layer;
	}

	return layer_nullptr_;
}

Scene::Scene(Scene&& _other) noexcept
{
	*this = move(_other);
}

void Scene::_Release()
{
}

bool Scene::_Initialize()
{
	if (!_CreateLayer("Background", numeric_limits<int>::min()))
		return false;

	if (!_CreateLayer("Default"))
		return false;

	if (!_CreateLayer("UI", numeric_limits<int>::max()))
		return false;

	return true;
}

void Scene::_Input(float _time)
{
	scene_component_->_Input(_time);

	for (auto iter = layer_list_.begin(); iter != layer_list_.end();)
	{
		if (!(*iter)->activation())
			iter = layer_list_.erase(iter);
		else if (!(*iter)->enablement())
			++iter;
		else
		{
			(*iter)->_Input(_time);
			++iter;
		}
	}
}

void Scene::_Update(float _time)
{
	scene_component_->_Update(_time);

	for (auto iter = layer_list_.begin(); iter != layer_list_.end();)
	{
		if (!(*iter)->activation())
			iter = layer_list_.erase(iter);
		else if (!(*iter)->enablement())
			++iter;
		else
		{
			(*iter)->_Update(_time);
			++iter;
		}
	}
}

void Scene::_LateUpdate(float _time)
{
	scene_component_->_LateUpdate(_time);

	for (auto iter = layer_list_.begin(); iter != layer_list_.end();)
	{
		if (!(*iter)->activation())
			iter = layer_list_.erase(iter);
		else if (!(*iter)->enablement())
			++iter;
		else
		{
			(*iter)->_LateUpdate(_time);
			++iter;
		}
	}
}

void Scene::_Collision(float _time)
{
	scene_component_->_Collision(_time);

	for (auto iter = layer_list_.begin(); iter != layer_list_.end();)
	{
		if (!(*iter)->activation())
			iter = layer_list_.erase(iter);
		else if (!(*iter)->enablement())
			++iter;
		else
		{
			(*iter)->_Collision(_time);
			++iter;
		}
	}
}

void Scene::_Render(HDC _device_context, float _time)
{
	scene_component_->_Render(_device_context, _time);

	for (auto iter = layer_list_.begin(); iter != layer_list_.end();)
	{
		if (!(*iter)->activation())
			iter = layer_list_.erase(iter);
		else if (!(*iter)->enablement())
			++iter;
		else
		{
			(*iter)->_Render(_device_context, _time);
			++iter;
		}
	}
}

bool Scene::_CreateLayer(string const& _tag, int _z_order)
{
	auto layer = shared_ptr<Layer>(new Layer, [](Layer* _p) {
		_p->_Release();
		delete _p;
	});

	layer->set_tag(_tag);
	layer->set_z_order(_z_order);
	layer->set_scene(shared_from_this());

	if (!layer->_Initialize())
		return false;

	layer_list_.push_back(layer);

	layer_list_.sort([](auto const& _left, auto const& _right) -> bool {
		return _left->z_order() < _right->z_order();
	});

	return true;
}