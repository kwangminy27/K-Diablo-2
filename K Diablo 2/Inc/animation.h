#pragma once

class Object;
class AnimationClip;

class Animation
{
	friend class Object;
	friend class UI;
public:
	int current_x() const;
	int current_y() const;
	int GetFrameWidth() const;
	int GetFrameHeight() const;

	void set_default_clip(std::string const& _tag);
	void set_current_clip(std::string const& _tag);
	void set_object(std::shared_ptr<Object> const& _object);

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
};