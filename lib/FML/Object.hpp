#pragma once
#include <functional>

namespace fm
{
	template <class T>
	class Object
	{
	public:
		Object()
		{
			_ID++;
			_currentID = _ID;
		}
		virtual ~Object()
		{
			Destroy();
		}

		void SetDestroyCallback(std::function<void(uint32_t)>&& inCallback)
		{
			_destroyCallback = inCallback;
		}

		virtual void Destroy()
		{
			if (_destroyCallback != nullptr)
			{
				_destroyCallback(_currentID);
			}
		}
	private:
		std::function<void(uint32_t)> _destroyCallback = {};

	public:
		uint32_t GetInstance() const { return _currentID; }

	private:
		inline static uint32_t _ID = 0;
		uint32_t	_currentID = 0;
	};
}
