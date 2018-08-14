#pragma once

using namespace std;

template<typename T>
shared_ptr<Collider> Object::_AddCollider(string const& _tag)
{
	auto collider = shared_ptr<Collider>{ new T, [](Collider* _p) {
		p->_Release();
		delete p;
	} };

	collider->set_tag(_tag);
	collider->set_object(weak_from_this());

	if (!collider->_Initialize())
		return collider_nullptr_;

	collider_list_.push_back(collider);

	return collider;
}