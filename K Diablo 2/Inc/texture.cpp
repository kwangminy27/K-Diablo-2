#include "stdafx.h"
#include "texture.h"

#include "core.h"
#include "path_manager.h"

using namespace std;
using namespace filesystem;

HDC Texture::memory_device_context() const
{
	return memory_device_context_;
}

int Texture::width() const
{
	return static_cast<int>(bitmap_structure_.bmWidth);
}

int Texture::height() const
{
	return static_cast<int>(bitmap_structure_.bmHeight);
}

Texture::Texture(Texture const& _other)
{
	*this = _other;
}

Texture::Texture(Texture&& _other) noexcept
{
	*this = move(_other);
}

void Texture::_Release()
{
	SelectObject(memory_device_context_, old_bitmap_);
	DeleteObject(bitmap_);
	DeleteDC(memory_device_context_);
}

bool Texture::_LoadTexture(wstring const& _file_name, string const& _path_tag)
{
	const auto& core = Core::GetSingleton();

	memory_device_context_ = CreateCompatibleDC(core->device_context());

	path path_buffer = PathManager::GetSingleton()->FindPath(_path_tag);

	if (path_buffer.empty())
		return false;

	wstring full_path = path_buffer.wstring() + _file_name;

	bitmap_ = static_cast<HBITMAP>(LoadImage(core->instance(), full_path.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
	old_bitmap_ = static_cast<HBITMAP>(SelectObject(memory_device_context_, bitmap_));

	GetObject(bitmap_, sizeof(bitmap_structure_), &bitmap_structure_);

	return true;
}