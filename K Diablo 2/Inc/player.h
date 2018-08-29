#pragma once

#include "character.h"

class Player final : public Character
{
	friend class ObjectManager;
public:
	PLAYER state() const;

	void set_state(PLAYER _state);
	void set_stage(std::shared_ptr<Object> const& _stage);
	void set_astar_interval(float _interval);
	void set_skill(SKILL _skill);

	void MoveByAStar(float _time);

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

	PLAYER state_{};
	bool run_flag_{};
	std::shared_ptr<Object> stage_{};
	std::stack<std::pair<int, int>> travel_path_stack_{};
	bool astar_complete_flag_{ true };
	bool test_{};
	TYPE::Point next_target_point_{};
	TYPE::Point final_target_point_{};
	float astar_interval_{};
	SKILL skill_{};
};