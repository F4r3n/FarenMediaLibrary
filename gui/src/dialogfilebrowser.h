#ifndef DIALOGFILEBROWSER_H
#define DIALOGFILEBROWSER_H
#include <string>
#include <vector>

struct EntityFile
{
        std::string name;
        int type;
};



std::vector<std::string> GetListFilesFromPattern(const std::string & inPattern);
std::vector<EntityFile> GetListFilesFromPath(const std::string &inPath);
int IsDirectory(const char *path);


//Only one file dialog can be visible at once
class DialogFileBrowser
{

        struct InternalData
        {
                std::string _selectedFilePath;
                bool _isOpened = false;
                bool _isVisible = false;
                std::string _currentFolderPath;
                std::vector<EntityFile> _listFiles;
        };

    public:
        DialogFileBrowser();
        static DialogFileBrowser& Get()
        {
            static DialogFileBrowser instance;
            return instance;
        }
        void Import(const std::string &path, const std::string &browserName,bool *isOpened);
    private:

        InternalData _internaldata;


};

#endif // DIALOGFILEBROWSER_H
