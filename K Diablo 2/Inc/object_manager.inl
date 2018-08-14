#pragma once

#include "layer.h"

using namespace std;

template<typename T>
bool ObjectManager::CreatePrototype(string const& _tag, shared_ptr<Scene> const& _scene)
{
	if (_FindPrototype(_tag))
		return false;

	if (!_scene)
		return false;

	auto prototype = unique_ptr<Object, function<void(Object*)>>{ new T, [](Object* _p) {
		_p->_Release();
		delete _p;
	} };

	prototype->set_tag(_tag);
	prototype->set_scene(_scene);

	if (!prototype->_Initialize())
		return false;

	prototype_map_.insert(make_pair(_tag, move(prototype)));

	return true;
}

template<typename T>
shared_ptr<Object> ObjectManager::CreateObject(string const& _tag, shared_ptr<Layer> const& _layer)
{
	if (!_layer)
		return object_nullptr_;

	auto object = shared_ptr<Object>{new T, [](Object* _p) {
		_p->_Release();
		delete _p;
	}};

	object->set_tag(_tag);
	object->set_scene(_layer->scene());
	object->set_layer(_layer);

	if (!object->_Initialize())
		return object_nullptr_;

	_layer->_AddObject(object);
	scene_object_multimap_.insert(make_pair(_tag, object));

	return object;
}