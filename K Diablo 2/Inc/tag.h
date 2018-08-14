#pragma once

class Tag
{
public:
	std::string const& tag() const;
	bool enablement() const;
	bool activation() const;

	void set_tag(std::string const& _tag);
	void set_enablement(bool _state);
	void set_activation(bool _state);

protected:
	Tag() = default;
	Tag(Tag const& _other);
	Tag(Tag&& _other) noexcept;
	Tag& operator=(Tag const&) = default;
	Tag& operator=(Tag&&) noexcept = default;

	virtual void _Release() = 0;

	std::string tag_{};
	bool enablement_{ true };
	bool activation_{ true };
};