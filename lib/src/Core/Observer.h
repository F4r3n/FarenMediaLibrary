#pragma once
#include <vector>
#include <string>
namespace fm
{
	class EventObserver
	{
	public:
		EventObserver(size_t inKind): eventKind(inKind) {}
		size_t eventKind;
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
		void Subscribe(std::shared_ptr<Observer> inObserver)
		{
			_observers.push_back(inObserver);
		}

		void Unsubscribe(std::shared_ptr<Observer> inObserver)
		{
			_observers.erase(std::remove(_observers.begin(), _observers.end(), inObserver), _observers.end());
		}

		void NotifyAll(const EventObserver& inEvent)
		{
			for (auto&& o : _observers)
			{
				o->Notify(this, inEvent);
			}
		}
		const std::string& GetName() const { return _name; }
	private:
		std::string							   _name;
		std::vector<std::shared_ptr<Observer>> _observers;

	};
}