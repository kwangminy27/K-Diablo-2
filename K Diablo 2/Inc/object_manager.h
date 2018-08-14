#pragma once

#include "singleton.h"

class Scene;
class Layer;
class Object;

class ObjectManager : public Singleton<ObjectManager>
{
	friend class Singleton<ObjectManager>;
public:
	template <typename T> bool CreatePrototype(std::string const& _tag, std::shared_ptr<Scene> const& _scene);
	template <typename T> std::shared_ptr<Object> CreateObject(std::string const& _tag, std::shared_ptr<Layer> const& _layer);
	std::shared_ptr<Object> CreateCloneObject(std::string const& _tag, std::shared_ptr<Layer> const& _layer);

	auto FindObjects(std::string const& _tag) const;

	void ErasePrototype(std::shared_ptr<Scene> const& _scene);
	void EraseExpiredSceneObject();

private:
	ObjectManager() = default;
	ObjectManager(ObjectManager const&) = delete;
	ObjectManager& operator=(ObjectManager const&) = delete;

	virtual void _Release() override;

	std::unique_ptr<Object, std::function<void(Object*)>> const& _FindPrototype(std::string const& _tag) const;

	std::unique_ptr<Object, std::function<void(Object*)>> prototype_nullptr_{};
	std::shared_ptr<Object> object_nullptr_{};

	std::unordered_map<std::string, std::unique_ptr<Object, std::function<void(Object*)>>> prototype_map_{};
	std::unordered_multimap<std::string, std::weak_ptr<Object>> scene_object_multimap_{};
};

#include "object_manager.inl"