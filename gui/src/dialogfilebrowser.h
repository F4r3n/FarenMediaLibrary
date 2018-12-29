#ifndef DIALOGFILEBROWSER_H
#define DIALOGFILEBROWSER_H
#include <string>
#include <vector>
#include "FilePath.h"

struct EntityFile
{
        FilePath path;
		FilePath parentDirectory;
		bool isDirectory;
};



std::vector<EntityFile> GetListFilesFromPath(const std::string &inPath);
int IsDirectory(const char *path);
bool CreateFolder(const char *path);
void RenameFile(const char* path, const char* newPath);

//Only one file dialog can be visible at once
class DialogFileBrowser
{

    struct InternalData
    {
         bool _isOpened = false;
         bool _isVisible = false;
         FilePath _currentPath;
		 FilePath _currentDirectory;
         std::vector<EntityFile> _listFiles;


         bool folderOnly = false;
};

    public:
        DialogFileBrowser();
        static DialogFileBrowser& Get()
        {
            static DialogFileBrowser instance;
            return instance;
        }
        void Import(const std::string &path, const std::string &browserName,bool *isOpened);
        void Run(const std::string &path, const std::string &browserName,bool *isOpened, bool folderOnly);
        const FilePath& GetResult() const {return _result.path;}
        void GetResult(std::string &outFileName, FilePath &outFilePath);
        inline bool IsValid() const {return _isValid;}

    private:
        void _UpdateData(const FilePath& path);
        bool _IsPathAFolder(const std::string &inPath);
        EntityFile _result;
        bool _isValid;

        InternalData _internaldata;
        size_t editedItem = -1;
        bool openContexttualMenu = false;
        bool renameWanted = false;
};

#endif // DIALOGFILEBROWSER_H
