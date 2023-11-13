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
		template <typename P = T>
		struct Node
		{
			Node(P inValue) : value(inValue) {}
			P value;
			std::list<std::unique_ptr<Node<P>>> nodes;
		};

	protected:
		std::unique_ptr<Node<T>> _head;

	};
}