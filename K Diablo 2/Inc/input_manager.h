#pragma once

#include "singleton.h"

class Object;

struct KeyInfo
{
	std::vector<__int8> element{};
	bool push{};
	bool pressed{};
	bool up{};
};

class InputManager final : public Singleton<InputManager>
{
	friend class Singleton<InputManager>;
public:
	bool Initialize();
	void Update();

	bool KeyPush(std::string const& _tag) const;
	bool KeyPressed(std::string const& _tag) const;
	bool KeyUp(std::string const& _tag) const;

private:
	InputManager() = default;
	InputManager(InputManager const&) = delete;
	InputManager& operator=(InputManager const&) = delete;

	virtual void _Release() override;

	template <typename T, typename... Types> void _AddKey(T const& _element, Types... _Args);
	template <typename... Types> void _AddKey(std::string const& _tag, Types... _Args);
	void _AddKey();

	std::unique_ptr<KeyInfo> const& _FindKey(string const& _tag) const;

	std::unique_ptr<KeyInfo> key_nullptr_{};

	std::unordered_map<std::string, std::unique_ptr<KeyInfo>> key_map_{};
	std::unique_ptr<KeyInfo> key_buffer_{};
	std::string key_tag_buffer_{};
};

#include "input_manager.inl"