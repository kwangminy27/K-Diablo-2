#include "stdafx.h"
#include "object_manager.h"

#include "scene.h"
#include "layer.h"
#include "object.h"

using namespace std;

shared_ptr<Object> ObjectManager::CreateCloneObject(string const& _tag, shared_ptr<Layer> const& _layer)
{
	auto const& prototype = _FindPrototype(_tag);

	if (!prototype)
		return object_nullptr_;

	if (!_layer)
		return object_nullptr_;

	shared_ptr<Object> object{ prototype->_Clone() };

	object->set_layer(_layer);

	_layer->_AddObject(object);
	scene_object_multimap_.insert(make_pair(_tag, object));

	return object;
}

auto ObjectManager::FindObjects(string const& _tag) const
{
	return scene_object_multimap_.equal_range(_tag);
}

void ObjectManager::ErasePrototype(shared_ptr<Scene> const& _scene)
{
	if (!_scene)
		return;

	for (auto iter = prototype_map_.begin(); iter != prototype_map_.end();)
	{
		if (iter->second->scene() == _scene)
			iter = prototype_map_.erase(iter);
		else
			++iter;
	}
}

void ObjectManager::EraseExpiredSceneObject()
{
	for (auto iter = scene_object_multimap_.begin(); iter != scene_object_multimap_.end();)
	{
		if (iter->second.expired())
			iter = scene_object_multimap_.erase(iter);
		else
			++iter;
	}
}

void ObjectManager::_Release()
{
}

unique_ptr<Object, function<void(Object*)>> const& ObjectManager::_FindPrototype(string const& _tag) const
{
	auto iter = prototype_map_.find(_tag);

	if (iter == prototype_map_.end())
		return prototype_nullptr_;

	return iter->second;
}