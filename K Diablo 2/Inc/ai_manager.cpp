#include "stdafx.h"
#include "ai_manager.h"

#include "math.h"
#include "stage.h"

using namespace std;
using namespace TYPE;

bool AIManager::Initialize()
{
	return true;
}

stack<pair<int, int>> AIManager::ProcessAStar(Point const& _src, Point const& _dest, shared_ptr<Stage> const& _stage)
{
	// 인접타일이 open_list에 들어있는지 확인하는 함수
	static auto IsInOpenList = [](pair<int, int> const& _idx, list<shared_ptr<AStarInfo>> const& _open_list) -> bool {
		for (auto const& element : _open_list)
		{
			if (element->idx == _idx)
				return true;
		}

		return false;
	};

	// 인접타일이 close_list에 들어있는지 확인하는 함수
	static auto IsInCloseList = [](pair<int, int> const& _idx, list<shared_ptr<AStarInfo>> const& _close_list) -> bool {
		for (auto const& element : _close_list)
		{
			if (element->idx == _idx)
				return true;
		}

		return false;
	};

	// 필요한 자료구조
	list<shared_ptr<AStarInfo>> open_list{}; // 검사가 필요한 공간
	list<shared_ptr<AStarInfo>> close_list{}; // 검사 완료된 공간
	stack<pair<int, int>> travel_path_stack{}; // 최종 경로를 담아 전달할 공간
	shared_ptr<AStarInfo> parent_buffer{}; // 부모 타일의 정보를 담을 공간
	shared_ptr<AStarInfo> astar_info_buffer{}; // list에 넣기전에 채울 AStar 정보를 담는 공간

	switch (_stage->type())
	{
	case STAGE::NORMAL:
		break;
	case STAGE::ISOMETRIC:
		// 시작점과 목표지점의 인덱스를 구한다.
		auto const& src_idx = _stage->FindIsometricIndex(_src);
		auto const& dest_idx = _stage->FindIsometricIndex(_dest);

		// 시작점과 목표지점의 위치를 구한다.
		auto const& src_center_position = _stage->GetTileCenterPosition(src_idx);
		auto const& dest_center_position = _stage->GetTileCenterPosition(dest_idx);

		// 시작점을 close_list에 넣는다.
		parent_buffer = make_shared<AStarInfo>();
		parent_buffer->idx = src_idx;

		close_list.push_back(parent_buffer);

		// 시작점부터 목표지점으로 가는 경로를 찾아낸다.
		while (parent_buffer->idx != dest_idx) // 길을 못 찾는 경우도 있을 듯?
		{
			// 부모타일의 인접타일들을 얻어온다.
			auto const& adjacency_list = _stage->GetTileAdjacencyList(parent_buffer->idx);

			// 부모타일의 위치를 구한다.
			auto const& parent_center_position = _stage->GetTileCenterPosition(parent_buffer->idx);

			// 인접타일들을 처리한다.(open_list, close_list, etc...)
			for (auto const& idx : adjacency_list)
			{
				// 인접타일이 이미 close_list에 있으면 작업을 할 필요가 없다.
				if (IsInCloseList(idx, close_list))
					continue;

				// 인접타일의 위치를 구한다.
				auto const& adjacent_tile_center_position = _stage->GetTileCenterPosition(idx);

				// 인접타일이 이미 open_list에 있으면 G값을 갱신한다.
				if (IsInOpenList(idx, open_list))
				{
					auto iter = find_if(open_list.begin(), open_list.end(), [&idx](shared_ptr<AStarInfo> const& _element) -> bool {
						return _element->idx == idx;
					});

					auto const& adjacent_tile = *iter;

					auto new_G = Math::GetDistance(parent_center_position, adjacent_tile_center_position) + parent_buffer->G;
					if (new_G < adjacent_tile->G)
					{
						adjacent_tile->G = new_G;
						adjacent_tile->F = new_G + adjacent_tile->H;
						adjacent_tile->parent = parent_buffer;
					}

					continue;
				}

				// 새로운 인접타일들을 open_list에 추가한다.
				astar_info_buffer = make_shared<AStarInfo>();
				astar_info_buffer->idx = idx;
				astar_info_buffer->G = Math::GetDistance(parent_center_position, adjacent_tile_center_position) + parent_buffer->G;
				astar_info_buffer->H = Math::GetDistance(adjacent_tile_center_position, dest_center_position);
				astar_info_buffer->F = astar_info_buffer->G + astar_info_buffer->H;
				astar_info_buffer->parent = parent_buffer;

				open_list.push_back(move(astar_info_buffer));
			}

			// 인접타일들을 처리하는 것이 완료되었으므로 open_list에서 F값이 가장 작은 타일을 찾아낸다.
			auto minimum_F_iter = min_element(open_list.begin(), open_list.end(), [](shared_ptr<AStarInfo> const& _lhs, shared_ptr<AStarInfo> const& _rhs) -> bool {
				return _lhs->F < _rhs->F;
			});
			auto minimum_F = *minimum_F_iter;

			// F값이 가장 작은 타일을 open_list에서 제거하고 close_list에 추가한다.
			open_list.erase(minimum_F_iter);
			close_list.push_back(minimum_F);

			// 인접타일 처리가 완료 되었으므로 부모 타일을 변경한다.
			parent_buffer = move(minimum_F);
		}

		// 목표점으로부터 시작점으로 가는 경로를 저장한다.
		while (parent_buffer->idx != src_idx)
		{
			travel_path_stack.push(parent_buffer->idx);
			parent_buffer = parent_buffer->parent;
		}

		break;
	}

	// 경로를 반환한다.
	return travel_path_stack;
}

void AIManager::_Release()
{
}