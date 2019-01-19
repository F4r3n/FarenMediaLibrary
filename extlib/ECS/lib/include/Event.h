
/*
 * Copyright (C) 2012 Alec Thomas <alec@swapoff.org>
 * All rights reserved.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.
 *
 * Author: Alec Thomas <alec@swapoff.org>
 */

#pragma once
#include <cstdint>
#include <list>
#include <System.h>
class BaseEvent
{
public:
	BaseEvent () {}
	virtual ~BaseEvent() {}
protected:
	static std::uint32_t _familyCounter;
};

class Event : public BaseEvent
{
public:
	Event() {}
	virtual ~Event() {}
	static std::uint32_t GetID()
	{
		static uint32_t id = _familyCounter++;
		return id;
	}
private:
	size_t receiver;
};


class EventManager
{
public:
	static EventManager& Get()
	{
		static EventManager e;
		return e;
	}

	

	template <typename ...Systems>
	void AddEvent(Event *e)
	{
		e->receiver = Getid<Systems...>();
		events.push_back(e);
	}
private:
	template <typename ...T>
	size_t GetId()
	{
		return GetId<T...>() & (1 << T::id());
	}

	std::list<Event*> events;
	EventManager() {}
};
