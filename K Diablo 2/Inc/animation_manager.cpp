#include "stdafx.h"
#include "animation_manager.h"

#include "path_manager.h"
#include "texture_manager.h"
#include "animation_clip.h"

using namespace std;
using namespace filesystem;
using namespace TYPE;

bool AnimationManager::Initialize()
{
	if (!_LoadAnimationDataSheet(L"animation.csv"))
		return false;

	return true;
}

shared_ptr<AnimationClip> const& AnimationManager::FindAnimationClip(string const& _tag)
{
	auto iter = animation_clip_map_.find(_tag);

	if (iter == animation_clip_map_.end())
		return animation_clip_nullptr_;

	return iter->second;
}

void AnimationManager::_Release()
{
}

bool AnimationManager::_LoadAnimationDataSheet(wstring const& _file_name, string const& _path_tag)
{
	path path_buffer = PathManager::GetSingleton()->FindPath(_path_tag);

	if (path_buffer.empty())
		return false;

	wstring full_path = path_buffer.wstring() + _file_name;

	ifstream file{ full_path };

	if (file.fail())
		return false;

	while (true)
	{
		string line{};
		stringstream line_stream{};

		getline(file, line);

		if (line.empty())
			break;

		line_stream.str(line);

		string animation_clip_file_name{};

		getline(line_stream, animation_clip_file_name, ',');

		wstring wstring_animation_clip_file_name{ animation_clip_file_name.begin(), animation_clip_file_name.end() };

		if (!_LoadAnimationClip(wstring_animation_clip_file_name))
			return false;
	}

	return true;
}

bool AnimationManager::_LoadAnimationClip(wstring const& _file_name, string const& _path_tag)
{
	path path_buffer = PathManager::GetSingleton()->FindPath(_path_tag);

	if (path_buffer.empty())
		return false;

	wstring full_path = path_buffer.wstring() + _file_name;

	ifstream file{ full_path };

	if (file.fail())
		return false;

	string texture_atlas_tag{};
	stringstream sstream{};

	getline(file, texture_atlas_tag);
	sstream.str(texture_atlas_tag);
	getline(sstream, texture_atlas_tag, ',');

	auto texture = TextureManager::GetSingleton()->FindTexture(texture_atlas_tag);

	if (!texture)
		return false;

	while (true)
	{
		string line{};
		stringstream line_stream{};

		getline(file, line);

		if (line.empty())
			break;

		line_stream.str(line);

		string tag{};
		string type{};
		string option{};
		string frame_width{};
		string frame_height{};
		string start_x{};
		string start_y{};
		string end_x{};
		string count_x{};
		string count_y{};
		string total_count{};
		string completion_time{};

		getline(line_stream, tag, ',');

		if (FindAnimationClip(tag))
			return false;

		getline(line_stream, type, ',');
		getline(line_stream, option, ',');
		getline(line_stream, frame_width, ',');
		getline(line_stream, frame_height, ',');
		getline(line_stream, start_x, ',');
		getline(line_stream, start_y, ',');
		getline(line_stream, end_x, ',');
		getline(line_stream, count_x, ',');
		getline(line_stream, count_y, ',');
		getline(line_stream, total_count, ',');
		getline(line_stream, completion_time, ',');

		auto animation_clip = shared_ptr<AnimationClip>{ new AnimationClip, [](AnimationClip* _p) {
			_p->_Release();
			delete _p;
		} };

		animation_clip->set_tag(tag);

		animation_clip->type_ = static_cast<ANIMATION_CLIP>(stoi(type));
		animation_clip->option_ = static_cast<ANIMATION_OPTION>(stoi(option));
		animation_clip->animation_clip_info_.frame_width = stoi(frame_width);
		animation_clip->animation_clip_info_.frame_height = stoi(frame_height);
		animation_clip->animation_clip_info_.start_x = stoi(start_x);
		animation_clip->animation_clip_info_.start_y = stoi(start_y);
		animation_clip->animation_clip_info_.end_x = stoi(end_x);
		animation_clip->animation_clip_info_.count_x = stoi(count_x);
		animation_clip->animation_clip_info_.count_y = stoi(count_y);
		animation_clip->animation_clip_info_.total_count = stoi(total_count);
		animation_clip->completion_time_ = stof(completion_time);

		animation_clip->texture_ = texture;

		animation_clip_map_.insert(make_pair(move(tag), move(animation_clip)));
	}

	return true;
}