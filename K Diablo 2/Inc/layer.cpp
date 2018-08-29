#include "stdafx.h"
#include "layer.h"

#include "scene.h"
#include "object.h"
#include "button.h"
#include "collision_manager.h"
#include "collider.h"
#include "animation.h"

using namespace std;

int Layer::z_order() const
{
	return z_order_;
}

shared_ptr<Scene> Layer::scene() const
{
	return scene_.lock();
}

void Layer::set_z_order(int _z_order)
{
	z_order_ = _z_order;
}

void Layer::set_scene(shared_ptr<Scene> const& _scene)
{
	scene_ = _scene;
}

shared_ptr<Object> const& Layer::FindObject(string const& _tag) const
{
	for (auto iter = object_list_.begin(); iter != object_list_.end(); ++iter)
	{
		if ((*iter)->tag() == _tag)
			return *iter;
	}

	return object_nullptr_;
}

Layer::Layer(Layer const& _other)
{
	*this = _other;
}

Layer::Layer(Layer && _other) noexcept
{
	*this = move(_other);
}

void Layer::_Release()
{
}

bool Layer::_Initialize()
{
	return true;
}

void Layer::_Input(float _time)
{
	if (tag() == "Default")
	{
		object_list_.sort([](shared_ptr<Object> const _first, shared_ptr<Object> const _second) -> bool {
			if (_first->animation() && _second->animation())
				return _first->position().y < _second->position().y;
			else
				return _first->position().y < _second->position().y;
		});
	}

	for (auto iter = object_list_.begin(); iter != object_list_.end();)
	{
		if (!(*iter)->activation())
			iter = object_list_.erase(iter);
		else if (!(*iter)->enablement())
			++iter;
		else
		{
			(*iter)->_Input(_time);
			++iter;
		}
	}
}

void Layer::_Update(float _time)
{
	for (auto iter = object_list_.begin(); iter != object_list_.end();)
	{
		if (!(*iter)->activation())
			iter = object_list_.erase(iter);
		else if (!(*iter)->enablement())
			++iter;
		else
		{
			(*iter)->_Update(_time);
			++iter;
		}
	}
}

void Layer::_LateUpdate(float _time)
{
	for (auto iter = object_list_.begin(); iter != object_list_.end();)
	{
		if (!(*iter)->activation())
			iter = object_list_.erase(iter);
		else if (!(*iter)->enablement())
			++iter;
		else
		{
			(*iter)->_LateUpdate(_time);
			++iter;
		}
	}
}

void Layer::_Collision(float _time)
{
	for (auto iter = object_list_.begin(); iter != object_list_.end();)
	{
		if (!(*iter)->activation())
			iter = object_list_.erase(iter);
		else if (!(*iter)->enablement())
		{
			for (auto& collider : (*iter)->collider_list_)
			{
				for (auto affected_collider_iter = collider->affected_collider_list_.begin(); affected_collider_iter != collider->affected_collider_list_.end();)
				{
					affected_collider_iter->lock()->_RemoveAffectedCollider(collider);
					affected_collider_iter = collider->affected_collider_list_.erase(affected_collider_iter);
				}
			}

			if (auto button = dynamic_pointer_cast<Button>(*iter))
				button->set_state(BUTTON_STATE::IDLE);

			++iter;
		}
		else
		{
			CollisionManager::GetSingleton()->AddCollider(*iter);
			++iter;
		}
	}
}

void Layer::_Render(HDC _device_context, float _time)
{
	for (auto iter = object_list_.begin(); iter != object_list_.end();)
	{
		if (!(*iter)->activation())
			iter = object_list_.erase(iter);
		else if (!(*iter)->enablement())
			++iter;
		else
		{
			(*iter)->_Render(_device_context, _time);
			++iter;
		}
	}
}

void Layer::_AddObject(shared_ptr<Object> const& _object)
{
	object_list_.push_back(_object);
}