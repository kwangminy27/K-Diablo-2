#pragma once

#include "monster.h"

class RangedMonster : public Monster
{
	friend class ObjectManager;
protected:
	RangedMonster() = default;
	RangedMonster(RangedMonster const& _other);
	RangedMonster(RangedMonster&& _other) noexcept;
	RangedMonster& operator=(RangedMonster const&) = default;
	RangedMonster& operator=(RangedMonster&&) noexcept = default;

	virtual void _Release() override;

	virtual bool _Initialize() override;
	virtual void _Input(float _time) override;
	virtual void _Update(float _time) override;
	virtual void _LateUpdate(float _time) override;
	virtual void _Collision(float _time) override;
	virtual void _Render(HDC _device_context, float _time) override;

	virtual std::unique_ptr<Object, std::function<void(Object*)>> _Clone() const override;
};