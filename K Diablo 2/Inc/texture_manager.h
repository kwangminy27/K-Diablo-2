#pragma once

#include "singleton.h"

class Texture;

class TextureManager final : public Singleton<TextureManager>
{
	friend class Singleton<TextureManager>;
public:
	bool Initialize();

	std::shared_ptr<Texture> const& FindTexture(std::string const& _tag) const;

private:
	TextureManager() = default;
	TextureManager(TextureManager const&) = delete;
	TextureManager& operator=(TextureManager const&) = delete;

	virtual void _Release() override;

	bool _LoadTextureDataSheet(std::wstring const& _file_name, std::string const& _path_tag = "DataPath");
	bool _LoadTexture(std::string const& _tag, std::wstring const& _file_name, std::string const& _path_tag = "TexturePath");

	std::shared_ptr<Texture> texture_nullptr_{};

	std::unordered_map<std::string, std::shared_ptr<Texture>> texture_map_{};
};