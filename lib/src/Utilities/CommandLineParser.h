#pragma once
#include <map>
#include <string>
#include <vector>
namespace fm
{
	class CommandLineParser
	{
	public:

		struct ArgInfo
		{
			std::string shortName;
			std::string	longName;

			bool operator<(const ArgInfo& rhs) const{
				return longName < rhs.longName;
			}
		};



		bool Process(char** argv, int argc);
		CommandLineParser(const std::vector<ArgInfo>& inArgInfo);
		std::string GetArg(const std::string& inValue) const;
	private:
		std::map<ArgInfo, std::string> _args;
	};
}