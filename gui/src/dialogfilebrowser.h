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



class DialogFileBrowser
{
    public:
        DialogFileBrowser();
        void Run(const std::string &path, const std::string &browserName,bool *isOpened);
    private:
        void _Open();
        void _Close();
        std::string _selectedFilePath;
        static bool _isOpened;
        static bool _isVisible;
        std::string _currentFolderPath;
        std::vector<EntityFile> _listFiles;

};

#endif // DIALOGFILEBROWSER_H
