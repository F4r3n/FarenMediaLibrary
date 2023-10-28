#include "Core/UUID.h"
#include <cassert>


#if _WIN32
#include <Rpc.h>
#include <guiddef.h>
#elif __linux__
#include <uuid/uuid.h>
#endif


std::string fm::UUID::Generate()
{
#if _WIN32
	GUID uuid = { 0 };
	std::string guid;

	// Create uuid or load from a string by UuidFromString() function
	::UuidCreate(&uuid);

	// If you want to convert uuid to string, use UuidToString() function
	RPC_CSTR szUuid = NULL;
	if (::UuidToStringA(&uuid, &szUuid) == RPC_S_OK)
	{
		guid = (char*)szUuid;
		::RpcStringFreeA(&szUuid);
	}

	return guid;
#elif __linux__
	uuid_t uuid;

	// generate
	uuid_generate_time_safe(uuid);

	// unparse (to string)
	char uuid_str[37];      // ex. "1b4e28ba-2fa1-11d2-883f-0016d3cca427" + "\0"
	uuid_unparse_lower(uuid, uuid_str);
	std::string s(uuid_str);

	return s;
#else
	return "";
#endif
	
}


