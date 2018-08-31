#pragma once

#include "character.h"

class Bar;
class UI;
class Text;

class Monster : public Character
{
	friend class ObjectManager;
public:
	float territory_radius() const;
	std::shared_ptr<Object> target() const;
	int dir_idx() const;
	std::shared_ptr<Object> stage() const;
	std::stack<std::pair<int, int>>& travel_path_stack();
	float astar_elapsed_time() const;
	float astar_interval() const;

	void set_territory_radius(float _radius);
	void set_target(std::shared_ptr<Object> const& _target);
	void set_dir_idx(int _idx);
	void set_walk_speed(float _speed);
	void set_run_speed(float _speed);
	void set_stage(std::shared_ptr<Object> const& _stage);
	void set_astar_complete_flag(bool _flag);
	void set_arrival_flag(bool _flag);
	void set_next_target_point(TYPE::Point const& _point);
	void set_final_target_point(TYPE::Point const& _point);
	void set_astar_elapsed_time(float _time);
	void set_astar_interval(float _interval);

	void clear_target();

	void MoveByAStar(float _time);

protected:
	Monster() = default;
	Monster(Monster const& _other);
	Monster(Monster&& _other) noexcept;
	Monster& operator=(Monster const&) = default;
	Monster& operator=(Monster&&) noexcept = default;

	virtual void _Release() override;

	virtual bool _Initialize() override;
	virtual void _Input(float _time) override;
	virtual void _Update(float _time) override;
	virtual void _LateUpdate(float _time) override;
	virtual void _Collision(float _time) override;
	virtual void _Render(HDC _device_context, float _time) override;

	virtual std::unique_ptr<Object, std::function<void(Object*)>> _Clone() const override;

	float territory_radius_{};
	std::weak_ptr<Object> target_{};
	float neutral_time_{};
	int dir_idx_{};
	float walk_speed_{};
	float run_speed_{};

	// astar
	std::weak_ptr<Object> stage_{};
	std::stack<std::pair<int, int>> travel_path_stack_{};
	bool astar_complete_flag_{ true };
	bool arrival_flag_{};
	TYPE::Point next_target_point_{};
	TYPE::Point final_target_point_{};
	float astar_elapsed_time_{};
	float astar_interval_{};
};