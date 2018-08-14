#include "stdafx.h"
#include "object.h"

#include "texture_manager.h"
#include "animation.h"

using namespace std;
using namespace TYPE;

OBJECT Object::type() const
{
	return type_;
}

Point const& Object::position() const
{
	return position_;
}

Point const& Object::size() const
{
	return size_;
}

Point const& Object::pivot() const
{
	return pivot_;
}

shared_ptr<Scene> Object::scene() const
{
	return scene_.lock();
}

shared_ptr<Layer> Object::layer() const
{
	return layer_.lock();
}

shared_ptr<Texture> Object::texture() const
{
	return texture_;
}

COLORREF const& Object::color_key() const
{
	return color_key_;
}

list<shared_ptr<Collider>>& Object::collider_list()
{
	return collider_list_;
}

void Object::set_position(Point const& _position)
{
	position_ = _position;
}

void Object::set_size(Point const& _size)
{
	size_ = _size;
}

void Object::set_pivot(Point const& _pivot)
{
	pivot_ = _pivot;
}

void Object::set_scene(shared_ptr<Scene> const& _scene)
{
	scene_ = _scene;
}

void Object::set_layer(shared_ptr<Layer> const& _layer)
{
	layer_ = _layer;
}

void Object::set_texture(string const& _tag)
{
	texture_ = TextureManager::GetSingleton()->FindTexture(_tag);
}

void Object::set_texture(shared_ptr<Texture> const& _texture)
{
	texture_ = _texture;
}

void Object::set_color_key(COLORREF const& _color_key)
{
	color_key_ = _color_key;
	is_color_key_ = true;
}

Object::Object(Object const& _other) : Tag(_other)
{
	type_ = _other.type_;
	position_ = _other.position_;
	size_ = _other.size_;
	pivot_ = _other.pivot_;
	scene_ = _other.scene_;
	layer_ = _other.layer_;
	texture_ = _other.texture_;
	color_key_ = _other.color_key_;
	is_color_key_ = _other.is_color_key_;
	animation_ = _other.animation_->_Clone();
}

Object::Object(Object&& _other) noexcept
{
	*this = move(_other);
}

void Object::_Release()
{
}

bool Object::_Initialize()
{
	return true;
}

void Object::_Input(float _time)
{
}

void Object::_Update(float _time)
{
}

void Object::_LateUpdate(float _time)
{
}

void Object::_Collision(float _time)
{
}

void Object::_Render(HDC _device_context, float _time)
{
}

bool Object::_AddAnimationClip(string const& _tag)
{
	if (!animation_)
	{
		animation_ = unique_ptr<Animation, function<void(Animation*)>>{ new Animation, [](Animation* _p) {
			_p->_Release();
			delete _p;
		} };

		animation_->set_object(shared_from_this());
	}

	if (!(animation_->_AddAnimationClip(_tag)))
		return false;

	return true;
}