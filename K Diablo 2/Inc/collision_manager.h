#pragma once

#include "singleton.h"

class Object;
class Collider;

class CollisionManager : public Singleton<CollisionManager>
{
	friend class Singleton<CollisionManager>;
public:
	bool Initialize();
	void Collision(float _time);

	void AddCollider(std::shared_ptr<Object> const& _object);

	std::vector<std::weak_ptr<Collider>>& GetCollisionGroup(std::string const& _tag);
	std::shared_ptr<TYPE::Pixel_24_Info> const& FindPixelCollider(std::string const& _tag) const;

private:
	CollisionManager() = default;
	CollisionManager(CollisionManager const&) = delete;
	CollisionManager& operator=(CollisionManager const&) = delete;

	virtual void _Release() override;

	bool _CreateCollisionGroup(std::string const& _tag);
	bool _CreatePixelCollider(std::string const& _tag, std::wstring const& _file_name, std::string const& _path_tag = "TexturePath");

	std::vector<std::weak_ptr<Collider>> collision_group_nullptr_{};
	std::shared_ptr<TYPE::Pixel_24_Info> pixel_collider_nullptr_{};

	std::unordered_map<std::string, std::vector<std::weak_ptr<Collider>>> collision_group_map_{};
	std::unordered_map<std::string, std::shared_ptr<TYPE::Pixel_24_Info>> pixel_collider_map_{};
};