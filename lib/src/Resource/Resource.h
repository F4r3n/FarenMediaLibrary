    #pragma once
#include <string>
#include "Core/FilePath.h"
#include "Core/serializer.hpp"
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
    
    
    class Resource : public Serializer
	{
    public:
		Resource(const fm::FilePath& inFilePath);
        virtual ~Resource() {}
		void CreateMeta(const fm::FilePath &inFilePath);
		void ReadMeta(const fm::FilePath& inFilePath);

		bool Serialize(nlohmann::json& ioJson) const;
		bool Read(const nlohmann::json& inJSON);

		bool Save();
		bool Load();
    protected:
		fm::FilePath _path;
	private:
		void GetMetaPath(const fm::FilePath&, fm::FilePath& outFilePath) const;
    };
    

    
 }
