#pragma once

#include "scene_component.h"

class LogoScene final : public SceneComponent
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

	void _CreateCharacter();

	void _ChangeToTitle();
	void _ChangeToCharacterSelect();
	void _ChangeToCharacterCreate();
	void _ChangeToCredit();

	void _ClearCharacter();

	LOGO_SCENE state_{ LOGO_SCENE::TRADEMARK };
};