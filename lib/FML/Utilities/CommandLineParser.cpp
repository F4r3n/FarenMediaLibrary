#include "CommandLineParser.h"
#include <span>
#include <ranges>

using namespace fm;
bool CommandLineParser::Process(char** argv, int argc)
{
	if (argc % 2 == 0) return false;

	for (int i = 0; i < argc; i++)
	{
		if (i == 0) continue;


		std::string arg(argv[i++]);
		size_t subLength = 0;
		if (arg.starts_with("--"))
		{
			subLength = 2;
		}
		else if (arg.starts_with("-"))
		{
			subLength = 1;
		}
		
		if (subLength == 0)
			continue;

		ArgInfo info;
		info.longName = arg.substr(subLength);
		auto it = _args.find(info);
		if (it != _args.end())
		{
			it->second = std::string(argv[i]);
		}
	}
	return true;
}

CommandLineParser::CommandLineParser(const std::vector<ArgInfo>& inArgInfo)
{
	for (const auto& arg : inArgInfo)
	{
		_args.emplace(arg, "");
	}
}


std::string CommandLineParser::GetArg(const std::string& inValue) const
{
	ArgInfo info;
	info.longName = inValue;

	auto it = _args.find(info);
	if (it != _args.end())
	{
		return it->second;
	}

	return "";
}