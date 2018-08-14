#pragma once

#include "singleton.h"

class AnimationClip;

class AnimationManager final : public Singleton<AnimationManager>
{
	friend class Singleton<AnimationManager>;
public:
	bool Initialize();

	std::shared_ptr<AnimationClip> const& FindAnimationClip(std::string const& _tag);

private:
	AnimationManager() = default;
	AnimationManager(AnimationManager const&) = delete;
	AnimationManager& operator=(AnimationManager const&) = delete;

	virtual void _Release() override;

	bool _LoadAnimationDataSheet(std::wstring const& _file_name, std::string const& _path_tag = "AnimationPath");
	bool _LoadAnimationClip(std::wstring const& _file_name, std::string const& _path_tag = "AnimationPath");

	std::shared_ptr<AnimationClip> animation_clip_nullptr_{};

	std::unordered_map<std::string, std::shared_ptr<AnimationClip>> animation_clip_map_{};
};