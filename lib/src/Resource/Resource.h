#pragma once
#include <string>
#include "Core/FilePath.h"
#include <nlohmann/json_fwd.hpp>
#pragma warning(disable: 4100)
namespace fm
{
	class FilePath;
}


 namespace fm {   
	 enum RESOURCE_TYPE {
		TEXTURE,
		FONT,
		MESH,
		SHADER,
		MATERIAL,
		IMAGE,
		AUDIO,
        LAST_RESOURCE,
        NONE
    };
    
    
    class Resource
	{
    public:
		Resource(const fm::FilePath& inFilePath);
		Resource() = default;
        virtual ~Resource() {}

		virtual void Reload(bool force = false) {}
		virtual void Save() const {}
		virtual void Load() {}

		virtual void Load(const nlohmann::json& inJSON);
		virtual void Save(nlohmann::json& outJSON) const;
		virtual RESOURCE_TYPE GetType() const = 0;

    protected:
		fm::FilePath	_path;


    };
    

    
 }
