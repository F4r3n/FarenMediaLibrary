#pragma once
#include <vector>
#include <string>
#include <any>
namespace fm
{
	class EventObserver
	{
	public:
		template <typename T>
		EventObserver(size_t inKind, const T &v): eventKind(inKind), value(v) {}
		EventObserver(size_t inKind) : eventKind(inKind) {}

		size_t eventKind = -1;
		std::any value;
	};


	class Observable;
	class Observer
	{
	public:
		virtual void Notify(Observable*, const EventObserver& inEvent) = 0;
	};

	class Observable
	{
	public:
		Observable(const std::string inName) : _name(inName) {}
		void Subscribe(Observer* inObserver)
		{
			_observers.push_back(inObserver);
		}

		void Unsubscribe(Observer* inObserver)
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

		void NotifyAll(const EventObserver& inEvent)
		{
			for (auto&& o : _observers)
			{
				o->Notify(this, inEvent);
			}
		}
		bool HasObserver() { return !_observers.empty(); }
		const std::string& GetName() const { return _name; }
	private:
		std::string			   _name;
		std::vector<Observer*> _observers;

	};
}