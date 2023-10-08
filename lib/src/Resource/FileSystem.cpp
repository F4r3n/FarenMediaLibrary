#include "FileSystem.h"
#include "Core/FilePath.h"
#include "Resource/ResourcesManager.h"
using namespace fm;

bool fm::IsInternal(LOCATION inLocation) {
	return inLocation == LOCATION::INTERNAL_LUA_LOCATION
		|| inLocation == LOCATION::INTERNAL_RESOURCES_LOCATION
		|| inLocation == LOCATION::INTERNAL_FONT_LOCATION
		|| inLocation == LOCATION::INTERNAL_SHADERS_LOCATION
		|| inLocation == LOCATION::INTERNAL_MATERIALS_LOCATION;
}