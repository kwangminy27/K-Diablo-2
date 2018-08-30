#include "stdafx.h"
#include "collision_manager.h"

#include "path_manager.h"
#include "object.h"
#include "collider.h"

using namespace std;
using namespace filesystem;
using namespace TYPE;

bool CollisionManager::Initialize()
{
	if (!_CreateCollisionGroup("Default"))
		return false;

	if (!_CreateCollisionGroup("UI"))
		return false;

	return true;
}

void CollisionManager::Collision(float _time)
{
	for (auto iter = collision_group_map_.begin(); iter != collision_group_map_.end(); ++iter)
	{
		auto& collsion_group = iter->second;

		if (collsion_group.size() < 2)
		{
			collsion_group.clear();
			continue;
		}

		for (auto i = 0; i < collsion_group.size() - 1; ++i)
		{
			if (collsion_group.at(i).expired())
				continue;

			auto src = collsion_group.at(i).lock();
			auto src_object = src->object();

			for (auto j = i + 1; j < collsion_group.size(); ++j)
			{
				if (collsion_group.at(j).expired())
					continue;

				auto dest = collsion_group.at(j).lock();
				auto dest_object = dest->object();

				if (src_object == dest_object)
					continue;

				if ((src_object->type() == OBJECT::MONSTER) && (dest_object->type() == OBJECT::MONSTER))
					continue;

				if (src->_Collision(dest))
				{
					dest->set_intersect_point(src->intersect_point());

					if (!src->_IsAffectedCollider(dest))
					{
						src->_AddAffectedCollider(dest);
						dest->_AddAffectedCollider(src);

						src->_OnCollisionEnter(dest, _time);
						dest->_OnCollisionEnter(src, _time);
					}
					else
					{
						src->_OnCollision(dest, _time);
						dest->_OnCollision(src, _time);
					}
				}
				else
				{
					if (src->_IsAffectedCollider(dest))
					{
						src->_OnCollisionLeave(dest, _time);
						dest->_OnCollisionLeave(src, _time);

						src->_RemoveAffectedCollider(dest);
						dest->_RemoveAffectedCollider(src);
					}
				}
			}
		}

		collsion_group.clear();
	}
}

void CollisionManager::AddCollider(shared_ptr<Object> const& _object)
{
	auto const& collider_list = _object->collider_list();

	if (collider_list.empty())
		return;

	for (auto const& collider : collider_list)
	{
		if (!collider->enablement())
			continue;

		auto& collision_group = GetCollisionGroup(collider->collision_group_tag());

		if (&collision_group == &collision_group_nullptr_)
			continue;

		collision_group.push_back(collider);
	}
}

vector<weak_ptr<Collider>>& CollisionManager::GetCollisionGroup(string const& _tag)
{
	auto iter = collision_group_map_.find(_tag);

	if (iter == collision_group_map_.end())
		return collision_group_nullptr_;

	return iter->second;
}

shared_ptr<Pixel_24_Info> const& CollisionManager::FindPixelCollider(string const& _tag) const
{
	auto iter = pixel_collider_map_.find(_tag);

	if (iter == pixel_collider_map_.end())
		return pixel_collider_nullptr_;

	return iter->second;
}

void CollisionManager::_Release()
{
}

bool CollisionManager::_CreateCollisionGroup(string const& _tag)
{
	if (&GetCollisionGroup(_tag) != &collision_group_nullptr_)
		return false;

	auto collision_group = vector<std::weak_ptr<Collider>>{};

	collision_group.reserve(1000);

	collision_group_map_.insert(make_pair(_tag, move(collision_group)));

	return true;
}

bool CollisionManager::_CreatePixelCollider(string const& _tag, wstring const& _file_name, string const& _path_tag)
{
	if (FindPixelCollider(_tag))
		return false;

	path path_buffer = PathManager::GetSingleton()->FindPath(_path_tag);

	if (path_buffer.empty())
		return false;

	wstring full_path = path_buffer.wstring() + _file_name;

	ifstream file{ full_path, ios::binary };

	if (file.fail())
		return false;

	BITMAPFILEHEADER bitmap_file_header{};
	BITMAPINFOHEADER bitmap_info_header{};

	file.read(reinterpret_cast<char*>(&bitmap_file_header), sizeof(BITMAPFILEHEADER));
	file.read(reinterpret_cast<char*>(&bitmap_info_header), sizeof(BITMAPINFOHEADER));

	auto pixel_collider = make_shared<Pixel_24_Info>();

	pixel_collider->collection.resize(bitmap_info_header.biHeight);
	for (auto i = 0; i < bitmap_info_header.biHeight; ++i)
	{
		pixel_collider->collection.at(i).resize(bitmap_info_header.biWidth);
		file.read(reinterpret_cast<char*>(&pixel_collider->collection.at(i).at(0)), sizeof(Pixel_24) * bitmap_info_header.biWidth);
	}
	reverse(pixel_collider->collection.begin(), pixel_collider->collection.end());

	pixel_collider_map_.insert(make_pair(_tag, move(pixel_collider)));

	return true;
}