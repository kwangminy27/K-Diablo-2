#include "stdafx.h"
#include "texture_manager.h"

#include "path_manager.h"
#include "texture.h"

using namespace std;
using namespace filesystem;

bool TextureManager::Initialize()
{
	if (!_LoadTextureDataSheet(L"texture.csv"))
		return false;

	return true;
}

shared_ptr<Texture> const& TextureManager::FindTexture(string const& _tag) const
{
	auto iter = texture_map_.find(_tag);

	if (iter == texture_map_.end())
		return texture_nullptr_;

	return iter->second;
}

void TextureManager::_Release()
{
}

bool TextureManager::_LoadTextureDataSheet(wstring const& _file_name, string const& _path_tag)
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

		string tag{};
		string texture_file_name{};

		getline(line_stream, tag, ',');
		getline(line_stream, texture_file_name, ',');

		wstring wstring_texture_file_name{ texture_file_name.begin(), texture_file_name.end() };

		if (!_LoadTexture(tag, wstring_texture_file_name))
			return false;
	}

	return true;
}

bool TextureManager::_LoadTexture(string const& _tag, wstring const& _file_name, string const& _path_tag)
{
	if (FindTexture(_tag))
		return false;

	auto texture = shared_ptr<Texture>{ new Texture, [](Texture* _p) {
		_p->_Release();
		delete _p;
	} };

	texture->set_tag(_tag);

	if (!texture->_LoadTexture(_file_name, _path_tag))
		return false;

	texture_map_.insert(make_pair(_tag, move(texture)));

	return true;
}