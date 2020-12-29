#pragma once
#include "Core/Observer.h"
#include <algorithm>
using namespace fm;
Observable::Observable(const std::string inName) : _name(inName)
{
}

void Observable::Subscribe(Observer* inObserver)
{
	_observers.push_back(inObserver);
}

void Observable::Unsubscribe(Observer* inObserver)
{
	if (!_observers.empty())
	{
		auto&& it = std::find(_observers.begin(), _observers.end(), inObserver);
		if (it != _observers.end())
		{
			_observers.erase(it, _observers.end());
		}
	}
}

void Observable::NotifyAll(const EventObserver& inEvent)
{
	for (auto&& o : _observers)
	{
		o->Notify(this, inEvent);
	}
}

bool Observable::HasObserver() const
{
	return !_observers.empty();
}

const std::string& Observable::GetName() const
{
	return _name;
}