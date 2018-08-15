#pragma once

#include "scene_component.h"

class LogoScene : public SceneComponent
{
	friend class Scene;
private:
	LogoScene() = default;
	LogoScene(LogoScene const& _other);
	LogoScene(LogoScene&& _other) noexcept;
	LogoScene& operator=(LogoScene const&) = default;
	LogoScene& operator=(LogoScene&&) noexcept = default;

	virtual void _Release() override;

	virtual bool _Initialize() override;
	virtual void _Input(float _time) override;
	virtual void _Update(float _time) override;
	virtual void _LateUpdate(float _time) override;
	virtual void _Collision(float _time) override;
	virtual void _Render(HDC _device_context, float _time) override;

	LOGO_SCENE state_{ LOGO_SCENE::TRADEMARK };
};