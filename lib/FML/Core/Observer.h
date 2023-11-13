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
		EventObserver() = delete;

		size_t eventKind = 0;
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
		Observable(const std::string inName);
		void Subscribe(Observer* inObserver);
		

		void Unsubscribe(Observer* inObserver);
		

		void NotifyAll(const EventObserver& inEvent);
		
		bool HasObserver() const;
		const std::string& GetName() const;
	private:
		std::string			   _name;
		std::vector<Observer*> _observers;

	};
}