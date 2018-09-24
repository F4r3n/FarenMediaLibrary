#ifndef DIALOGFILEBROWSER_H
#define DIALOGFILEBROWSER_H
#include <string>
#include <vector>

struct EntityFile
{
        std::string name;
        std::string fullPath;
        std::string directory;
        int type;
};



std::vector<std::string> GetListFilesFromPattern(const std::string & inPattern);
std::vector<EntityFile> GetListFilesFromPath(const std::string &inPath, std::string *outPath = nullptr);
int IsDirectory(const char *path);
int CreateFolder(const char *path, mode_t mode = 0777);
int RenameFile(const char* path, const char* newPath);

//Only one file dialog can be visible at once
class DialogFileBrowser
{

    struct InternalData
    {
         bool _isOpened = false;
         bool _isVisible = false;
         std::string _currentPath;
         std::string _currentDirectory;
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
        const std::string& GetResult() const {return _result.fullPath;}
        void GetResult(std::string &outFileName, std::string &outFilePath);
        inline bool IsValid() const {return _isValid;}

    private:
        void _UpdateData(const std::string& path, bool previous = false);
        bool _IsPathAFolder(const std::string &inPath);
        EntityFile _result;
        bool _isValid;

        InternalData _internaldata;
        size_t editedItem = -1;
        bool openContexttualMenu = false;
        bool renameWanted = false;
};

#endif // DIALOGFILEBROWSER_H
