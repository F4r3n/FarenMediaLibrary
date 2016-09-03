#pragma once
#include <string>
namespace fm {
class PostProcessor {
	public:
		PostProcessor();
		~PostProcessor();
		void setShader(const std::string &newFrag);
	private:
		std::string simple_vertex;
		std::string simple_fragement;

};
}