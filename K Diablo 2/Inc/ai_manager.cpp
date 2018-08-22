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
	// ����Ÿ���� open_list�� ����ִ��� Ȯ���ϴ� �Լ�
	static auto IsInOpenList = [](pair<int, int> const& _idx, list<shared_ptr<AStarInfo>> const& _open_list) -> bool {
		for (auto const& element : _open_list)
		{
			if (element->idx == _idx)
				return true;
		}

		return false;
	};

	// ����Ÿ���� close_list�� ����ִ��� Ȯ���ϴ� �Լ�
	static auto IsInCloseList = [](pair<int, int> const& _idx, list<shared_ptr<AStarInfo>> const& _close_list) -> bool {
		for (auto const& element : _close_list)
		{
			if (element->idx == _idx)
				return true;
		}

		return false;
	};

	// �ʿ��� �ڷᱸ��
	list<shared_ptr<AStarInfo>> open_list{}; // �˻簡 �ʿ��� ����
	list<shared_ptr<AStarInfo>> close_list{}; // �˻� �Ϸ�� ����
	stack<pair<int, int>> travel_path_stack{}; // ���� ��θ� ��� ������ ����
	shared_ptr<AStarInfo> parent_buffer{}; // �θ� Ÿ���� ������ ���� ����
	shared_ptr<AStarInfo> astar_info_buffer{}; // list�� �ֱ����� ä�� AStar ������ ��� ����

	switch (_stage->type())
	{
	case STAGE::NORMAL:
		break;
	case STAGE::ISOMETRIC:
		// �������� ��ǥ������ �ε����� ���Ѵ�.
		auto const& src_idx = _stage->FindIsometricIndex(_src);
		auto const& dest_idx = _stage->FindIsometricIndex(_dest);

		// �������� ��ǥ������ ��ġ�� ���Ѵ�.
		auto const& src_center_position = _stage->GetTileCenterPosition(src_idx);
		auto const& dest_center_position = _stage->GetTileCenterPosition(dest_idx);

		// �������� close_list�� �ִ´�.
		parent_buffer = make_shared<AStarInfo>();
		parent_buffer->idx = src_idx;

		close_list.push_back(parent_buffer);

		// ���������� ��ǥ�������� ���� ��θ� ã�Ƴ���.
		while (parent_buffer->idx != dest_idx) // ���� �� ã�� ��쵵 ���� ��?
		{
			// �θ�Ÿ���� ����Ÿ�ϵ��� ���´�.
			auto const& adjacency_list = _stage->GetTileAdjacencyList(parent_buffer->idx);

			// �θ�Ÿ���� ��ġ�� ���Ѵ�.
			auto const& parent_center_position = _stage->GetTileCenterPosition(parent_buffer->idx);

			// ����Ÿ�ϵ��� ó���Ѵ�.(open_list, close_list, etc...)
			for (auto const& idx : adjacency_list)
			{
				// ����Ÿ���� �̹� close_list�� ������ �۾��� �� �ʿ䰡 ����.
				if (IsInCloseList(idx, close_list))
					continue;

				// ����Ÿ���� ��ġ�� ���Ѵ�.
				auto const& adjacent_tile_center_position = _stage->GetTileCenterPosition(idx);

				// ����Ÿ���� �̹� open_list�� ������ G���� �����Ѵ�.
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

				// ���ο� ����Ÿ�ϵ��� open_list�� �߰��Ѵ�.
				astar_info_buffer = make_shared<AStarInfo>();
				astar_info_buffer->idx = idx;
				astar_info_buffer->G = Math::GetDistance(parent_center_position, adjacent_tile_center_position) + parent_buffer->G;
				astar_info_buffer->H = Math::GetDistance(adjacent_tile_center_position, dest_center_position);
				astar_info_buffer->F = astar_info_buffer->G + astar_info_buffer->H;
				astar_info_buffer->parent = parent_buffer;

				open_list.push_back(move(astar_info_buffer));
			}

			// ����Ÿ�ϵ��� ó���ϴ� ���� �Ϸ�Ǿ����Ƿ� open_list���� F���� ���� ���� Ÿ���� ã�Ƴ���.
			auto minimum_F_iter = min_element(open_list.begin(), open_list.end(), [](shared_ptr<AStarInfo> const& _lhs, shared_ptr<AStarInfo> const& _rhs) -> bool {
				return _lhs->F < _rhs->F;
			});
			auto minimum_F = *minimum_F_iter;

			// F���� ���� ���� Ÿ���� open_list���� �����ϰ� close_list�� �߰��Ѵ�.
			open_list.erase(minimum_F_iter);
			close_list.push_back(minimum_F);

			// ����Ÿ�� ó���� �Ϸ� �Ǿ����Ƿ� �θ� Ÿ���� �����Ѵ�.
			parent_buffer = move(minimum_F);
		}

		// ��ǥ�����κ��� ���������� ���� ��θ� �����Ѵ�.
		while (parent_buffer->idx != src_idx)
		{
			travel_path_stack.push(parent_buffer->idx);
			parent_buffer = parent_buffer->parent;
		}

		break;
	}

	// ��θ� ��ȯ�Ѵ�.
	return travel_path_stack;
}

void AIManager::_Release()
{
}