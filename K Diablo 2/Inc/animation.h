#pragma once

class Object;
class AnimationClip;

class Animation
{
	friend class Layer;
	friend class Object;
	friend class UI;
	friend class Player;
public:
	int current_x() const;
	int current_y() const;
	int GetFrameWidth() const;
	int GetFrameHeight() const;
	int GetOffsetX() const;
	int GetOffsetY() const;

	void set_default_clip(std::string const& _tag);
	void set_current_clip(std::string const& _tag);
	void set_object(std::shared_ptr<Object> const& _object);

	void InsertAnimationCallback(std::string const& _tag, std::function<void(void)> const& _function);

private:
	Animation() = default;
	Animation(Animation const& _other);
	Animation(Animation&& _other);
	Animation& operator=(Animation const&) = default;
	Animation& operator=(Animation&&) = default;

	void _Release();

	bool _Initialize();
	void _Update(float _time);
	void _ChangeClip(std::string const& _tag);
	void _ChangeClipWithDirection(std::string const& _tag);
	void _ChangeToDefaultClip();

	std::unique_ptr<Animation, std::function<void(Animation*)>> _Clone() const;

	bool _AddAnimationClip(std::string const& _tag);
	std::shared_ptr<AnimationClip> const& _FindAnimationClip(std::string const& _tag) const;

	std::shared_ptr<AnimationClip> animation_clip_nullptr_{};

	std::unordered_map<std::string, std::shared_ptr<AnimationClip>> animation_clip_map_{};
	int current_x_{};
	int current_y_{};
	float elapsed_time_{};
	std::shared_ptr<AnimationClip> default_clip_{};
	std::shared_ptr<AnimationClip> current_clip_{};
	std::weak_ptr<Object> object_{};
	std::unordered_map<std::string, std::function<void(void)>> callback_map_{};
};