#pragma once
#include <string>
#include "Core/FilePath.h"
#include "FileSystem.h"
#include <nlohmann/json_fwd.hpp>
#if defined(_MSC_VER) && _MSC_VER >= 1900
#pragma warning(disable: 4100)
#endif
namespace fm
{
	class FilePath;
}


 namespace fm {   
	 enum class RESOURCE_TYPE {
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

		virtual void Reload([[maybe_unused]] bool force = false) {}
		virtual void Save() const {}
		virtual void Load() {}

		virtual void Load(const nlohmann::json& inJSON);
		virtual void Save(nlohmann::json& outJSON) const;
		virtual RESOURCE_TYPE GetType() const = 0;

		const fm::FilePath& GetPath() const { return _path; }
		bool  IsInternal() {
			const auto id = _path.GetFileSystemID();
			return fm::IsInternal(id);
		}

		void Touch() { _stamp++; }
		size_t GetStamp() const { return _stamp; }
    protected:
		fm::FilePath	_path;
		size_t			_stamp = 0;


    };
    

    
 }
