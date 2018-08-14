#pragma once

#include "tag.h"

class Texture final : public Tag
{
	friend class TextureManager;
public:
	HDC memory_device_context() const;
	int width() const;
	int height() const;

protected:
	Texture() = default;
	Texture(Texture const& _other);
	Texture(Texture&& _other) noexcept;
	Texture& operator=(Texture const&) = default;
	Texture& operator=(Texture&&) noexcept = default;

	virtual void _Release() override;

	bool _LoadTexture(std::wstring const& _file_name, std::string const& _path_tag);

	HDC memory_device_context_{};
	HBITMAP bitmap_{};
	HBITMAP old_bitmap_{};
	BITMAP bitmap_structure_{};
};