#include "stdafx.h"
#include "tag.h"

using namespace std;

string const& Tag::tag() const
{
	return tag_;
}

bool Tag::enablement() const
{
	return enablement_;
}

bool Tag::activation() const
{
	return activation_;
}

void Tag::set_tag(string const& _tag)
{
	tag_ = _tag;
}

void Tag::set_enablement(bool _state)
{
	enablement_ = _state;
}

void Tag::set_activation(bool _state)
{
	activation_ = _state;
}

Tag::Tag(Tag const& _other)
{
	*this = _other;
}

Tag::Tag(Tag&& _other) noexcept
{
	*this = move(_other);
}