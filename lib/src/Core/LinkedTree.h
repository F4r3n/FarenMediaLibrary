#pragma once
#include <list>
#include <limits>
#include <memory>
namespace fm
{
	template <typename T>
	class LinkedTree
	{
	public:
		LinkedTree() {}
		template <typename T>
		struct Node
		{
			Node(T inValue) : value(inValue) {}
			T value;
			std::list<std::unique_ptr<Node<T>>> nodes;
		};

	protected:
		std::unique_ptr<Node<T>> _head;

	};
}