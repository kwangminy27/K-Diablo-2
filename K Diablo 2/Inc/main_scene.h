#pragma once

#include "scene_component.h"

class MainScene final : public SceneComponent
{
	friend class Scene;
private:
	MainScene() = default;
	MainScene(MainScene const& _other);
	MainScene(MainScene&& _other) noexcept;
	MainScene& operator=(MainScene const&) = default;
	MainScene& operator=(MainScene&&) noexcept = default;

	virtual void _Release() override;

	virtual bool _Initialize() override;
	virtual void _Input(float _time) override;
	virtual void _Update(float _time) override;
	virtual void _LateUpdate(float _time) override;
	virtual void _Collision(float _time) override;
	virtual void _Render(HDC _device_context, float _time) override;

	void _CreateCharacterWindow();
	void _CreateInventoryWindow();

	void _ToggleCharacterWindow();
	void _ToggleInventoryWindow();
};