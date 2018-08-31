#pragma once

#include "object.h"

class Text : public Object
{
	friend class ObjectManager;
public:
	void set_string(std::string const& _string);
	void set_font_size(FONT_SIZE _font_size);
	void set_ui_flag(bool _flag);

private:
	Text() = default;
	Text(Text const& _other);
	Text(Text&& _other) noexcept;
	Text& operator=(Text const&) = default;
	Text& operator=(Text&&) noexcept = default;

	virtual void _Release() override;

	virtual bool _Initialize() override;
	virtual void _Input(float _time) override;
	virtual void _Update(float _time) override;
	virtual void _LateUpdate(float _time) override;
	virtual void _Collision(float _time) override;
	virtual void _Render(HDC _device_context, float _time) override;

	virtual std::unique_ptr<Object, std::function<void(Object*)>> _Clone() const override;

	std::string string_{};
	FONT_SIZE font_size_{};
	bool ui_flag_{ true };
};