#include "stdafx.h"
#include "path_manager.h"

using namespace std;
using namespace filesystem;

bool PathManager::Initialize()
{
	path path_buffer = current_path();
	path_buffer.replace_filename("Bin\\");

	path_map_.insert(make_pair("RootPath", move(path_buffer)));

	_AddPath("DataPath", "Data\\");
	_AddPath("AnimationPath", "Animation\\", "DataPath");
	_AddPath("SoundPath", "Sound\\");
	_AddPath("TexturePath", "Texture\\");
	_AddPath("VideoPath", "Video\\");
	
	return true;
}

path const& PathManager::FindPath(string const& _tag) const
{
	auto iter = path_map_.find(_tag);

	if (iter == path_map_.end())
		return path_nullptr_;

	return iter->second;
}

void PathManager::_Release()
{
}

bool PathManager::_AddPath(string const& _tag, path const& _path, string const& _base_path_tag)
{
	if (!FindPath(_tag).empty())
		return false;

	path path_buffer = FindPath(_base_path_tag);

	if (path_buffer.empty())
		return false;

	path_buffer /= _path;

	path_map_.insert(make_pair(_tag, move(path_buffer)));

	return true;
}