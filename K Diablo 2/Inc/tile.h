#pragma once

#include "object.h"

class Tile final : public Object
{
	friend class ObjectManager;
	friend class Stage;
public:
	std::pair<int, int> const& idx() const;
	TILE_OPTION option() const;

	void set_idx(std::pair<int, int> const& _idx);
	void set_option(TILE_OPTION _option);

private:
	Tile() = default;
	Tile(Tile const& _other);
	Tile(Tile&& _other) noexcept;
	Tile& operator=(Tile const&) = default;
	Tile& operator=(Tile&&) noexcept = default;

	virtual void _Release() override;

	virtual bool _Initialize() override;
	virtual void _Input(float _time) override;
	virtual void _Update(float _time) override;
	virtual void _LateUpdate(float _time) override;
	virtual void _Collision(float _time) override;
	virtual void _Render(HDC _device_context, float _time) override;

	virtual std::unique_ptr<Object, std::function<void(Object*)>> _Clone() const override;

	TILE_OPTION option_{ TILE_OPTION::NONE };
	std::pair<int, int> idx_{};
};