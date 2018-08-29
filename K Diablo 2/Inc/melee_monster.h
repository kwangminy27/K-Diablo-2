#pragma once

#include "monster.h"

class MeleeMonster : public Monster
{
	friend class ObjectManager;
protected:
	MeleeMonster() = default;
	MeleeMonster(MeleeMonster const& _other);
	MeleeMonster(MeleeMonster&& _other) noexcept;
	MeleeMonster& operator=(MeleeMonster const&) = default;
	MeleeMonster& operator=(MeleeMonster&&) noexcept = default;

	virtual void _Release() override;

	virtual bool _Initialize() override;
	virtual void _Input(float _time) override;
	virtual void _Update(float _time) override;
	virtual void _LateUpdate(float _time) override;
	virtual void _Collision(float _time) override;
	virtual void _Render(HDC _device_context, float _time) override;

	virtual std::unique_ptr<Object, std::function<void(Object*)>> _Clone() const override;
};