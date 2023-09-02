#pragma once
#include <string>
#include "Core/FilePath.h"
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
        LAST_RESOURCE,
        NONE
    };
    
    
    class Resource
	{
    public:
		Resource(const fm::FilePath& inFilePath);
        virtual ~Resource() {}

		virtual void Reload(bool force = false) {}
		virtual void Save() const {}
		virtual void Load() {}

    protected:
		fm::FilePath	_path;


    };
    

    
 }
