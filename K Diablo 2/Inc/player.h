#pragma once

#include "character.h"

class Player final : public Character
{
	friend class ObjectManager;
public:
	void Move(TYPE::Point const& _direction, float _time);

protected:
	Player() = default;
	Player(Player const& _other);
	Player(Player&& _other) noexcept;
	Player& operator=(Player const&) = default;
	Player& operator=(Player&&) noexcept = default;

	virtual void _Release() override;

	virtual bool _Initialize() override;
	virtual void _Input(float _time) override;
	virtual void _Update(float _time) override;
	virtual void _LateUpdate(float _time) override;
	virtual void _Collision(float _time) override;
	virtual void _Render(HDC _device_context, float _time) override;

	virtual std::unique_ptr<Object, std::function<void(Object*)>> _Clone() const override;
};