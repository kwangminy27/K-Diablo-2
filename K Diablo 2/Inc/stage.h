#pragma once

#include "object.h"

class Stage final : public Object
{
	friend class ObjectManager;
public:
	STAGE type() const;
	std::vector<std::vector<std::shared_ptr<Object>>> const& tile_collection() const;
	TYPE::Point const& stage_size() const;

	void set_type(STAGE _type);

	void CreateTile(STAGE _type, int _idx_width, int _idx_height, TYPE::Point const& _stage_size, TYPE::Point const& _tile_size, std::string const& _tag);

	std::pair<int, int> FindIsometricIndex(TYPE::Point const& _position);
	TILE_OPTION GetTileOption(int _x, int _y) const;
	std::list<std::pair<int, int>> const& GetTileAdjacencyList(std::pair<int, int> const& _idx) const;
	TYPE::Point GetTileCenterPosition(std::pair<int, int> const& _idx) const;

private:
	Stage() = default;
	Stage(Stage const& _other);
	Stage(Stage&& _other) noexcept;
	Stage& operator=(Stage const&) = default;
	Stage& operator=(Stage&&) noexcept = default;

	virtual void _Release() override;

	virtual bool _Initialize() override;
	virtual void _Input(float _time) override;
	virtual void _Update(float _time) override;
	virtual void _LateUpdate(float _time) override;
	virtual void _Collision(float _time) override;
	virtual void _Render(HDC _device_context, float _time) override;

	virtual std::unique_ptr<Object, std::function<void(Object*)>> _Clone() const override;

	STAGE type_{ STAGE::NORMAL };
	std::vector<std::vector<std::shared_ptr<Object>>> tile_collection_{};
	TYPE::Point stage_size_{};
	TYPE::Point tile_size_{};
	std::pair<int, int> view_range_idx_x_{};
	std::pair<int, int> view_range_idx_y_{};
	std::vector<std::vector<std::list<std::pair<int, int>>>> tile_graph_{};
};