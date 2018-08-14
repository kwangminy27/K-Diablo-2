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

	std::shared_ptr<Object> mouse() const;
	TYPE::Point const& mouse_client_position() const;
	TYPE::Point const& mouse_world_position() const;
	TYPE::Point const& mouse_displacement() const;

	void set_mouse(std::shared_ptr<Object> const& _mouse);

	void UpdateMouseCursor();
	void RenderMouseCursor(HDC _device_context, float _time);

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
	std::shared_ptr<Object> mouse_{};
	TYPE::Point mouse_client_position_{};
	TYPE::Point mouse_world_position_{};
	TYPE::Point mouse_displacement_{};
	bool mouse_cursor_show_state_{};
};

#include "input_manager.inl"