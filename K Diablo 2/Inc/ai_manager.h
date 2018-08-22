#pragma once

#include "singleton.h"

class Stage;

struct AStarInfo
{
	std::pair<int, int> idx{};
	float F{};
	float G{};
	float H{};
	std::shared_ptr<AStarInfo> parent{};
};

class AIManager : public Singleton<AIManager>
{
	friend class Singleton<AIManager>;
public:
	bool Initialize();

	std::stack<std::pair<int, int>> ProcessAStar(TYPE::Point const& _src, TYPE::Point const& _dest, std::shared_ptr<Stage> const& _stage);

private:
	AIManager() = default;
	AIManager(AIManager const&) = delete;
	AIManager& operator=(AIManager const&) = delete;

	virtual void _Release() override;
};