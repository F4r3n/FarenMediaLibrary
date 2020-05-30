#include "Resource/Resource.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include "Core/FilePath.h"
#include "Core/UUID.h"

using namespace fm;


Resource::Resource(const fm::FilePath& inFilePath)
	:_path(inFilePath)
{
}


