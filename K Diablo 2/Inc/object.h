#pragma once

#include "tag.h"

class Scene;
class Layer;
class Texture;
class Animation;
class Collider;

class Object : public Tag, public std::enable_shared_from_this<Object>
{
	friend class InputManager;
	friend class Layer;
	friend class ObjectManager;
public:
	OBJECT type() const;
	TYPE::Point const& position() const;
	TYPE::Point const& size() const;
	TYPE::Point const& pivot() const;
	std::shared_ptr<Scene> scene() const;
	std::shared_ptr<Layer> layer() const;
	std::shared_ptr<Texture> texture() const;
	COLORREF const& color_key() const;
	std::unique_ptr<Animation, std::function<void(Animation*)>> const& animation() const;
	std::list<std::shared_ptr<Collider>>& collider_list();

	void set_position(TYPE::Point const& _position);
	void set_size(TYPE::Point const& _size);
	void set_pivot(TYPE::Point const& _pivot);
	void set_scene(std::shared_ptr<Scene> const& _scene);
	void set_layer(std::shared_ptr<Layer> const& _layer);
	void set_texture(std::string const& _tag);
	void set_texture(std::shared_ptr<Texture> const& _texture);
	void set_color_key(COLORREF const& _color_key);

	bool AddAnimationClip(std::string const& _tag);
	template <typename T> std::shared_ptr<Collider> AddCollider(std::string const& _tag);

	void SetAnimationCallback(std::string const& _tag, std::function<void()> const& _function);
	void SetDefaultClip(std::string const& _tag);
	void ChangeAnimationClip(std::string const& _tag);
	void ChangeAnimationClipWithDirection(std::string const& _tag);
	void ChangeToDefaultClip();

	std::string const& GetCurrentAnimationClipTag() const;
	std::shared_ptr<Collider> GetCollider(std::string const& _tag);

protected:
	Object() = default;
	Object(Object const& _other);
	Object(Object&& _other) noexcept;
	Object& operator=(Object const&) = default;
	Object& operator=(Object&&) noexcept = default;
	virtual ~Object() = default;

	virtual void _Release() override;

	virtual bool _Initialize();
	virtual void _Input(float _time);
	virtual void _Update(float _time);
	virtual void _LateUpdate(float _time);
	virtual void _Collision(float _time);
	virtual void _Render(HDC _device_context, float _time);

	virtual std::unique_ptr<Object, std::function<void(Object*)>> _Clone() const = 0;

	std::shared_ptr<Collider> collider_nullptr_{};

	OBJECT type_{};
	TYPE::Point position_{};
	TYPE::Point size_{};
	TYPE::Point pivot_{};
	std::weak_ptr<Scene> scene_{};
	std::weak_ptr<Layer> layer_{};
	std::shared_ptr<Texture> texture_{};
	COLORREF color_key_{};
	bool is_color_key_{};
	std::unique_ptr<Animation, std::function<void(Animation*)>> animation_{};
	std::list<std::shared_ptr<Collider>> collider_list_{};
};

#include "object.inl"