#include "dialogfilebrowser.h"
#include <sstream>
#include <imgui/imgui.h>
#include <iostream>

//For linux and mac
#include <sys/types.h>
#include <sys/stat.h>
#if __linux__
#include <unistd.h>
#include <glob.h>
#include <dirent.h>
#endif
#include <cstdio>


int IsDirectory(const char *path)
{
#if __linux__
    struct stat statbuf;
    if (stat(path, &statbuf) != 0)
        return 0;
    return S_ISDIR(statbuf.st_mode);
#else
	return 0;
#endif
}

std::vector<std::string> GetListFilesFromPattern(const std::string & inPattern)
{
	std::vector<std::string> filenames;
#if __linux__
    using namespace std;
    glob_t glob_result;
    memset(&glob_result, 0, sizeof(glob_result));

    // do the glob operation
    int return_value = glob(inPattern.c_str(), GLOB_TILDE, NULL, &glob_result);
    if(return_value != 0)
    {
        globfree(&glob_result);
        stringstream ss;
        ss << "glob() failed with return_value " << return_value << endl;
        throw std::runtime_error(ss.str());
    }

    // collect all the filenames into a std::list<std::string>
   
    for(size_t i = 0; i < glob_result.gl_pathc; ++i)
    {
        filenames.push_back(string(glob_result.gl_pathv[i]));
    }

    // cleanup
    globfree(&glob_result);
#else

#endif
    // done
    return filenames;
}

int RenameFile(const char* path, const char* newPath)
{
    return rename(path, newPath);
}


int CreateFolder(const char *path)
{
#if __linux__
    struct stat            st;
    int             status = 0;

    if (stat(path, &st) != 0)
    {
        /* Directory does not exist. EEXIST for race condition */
        if (mkdir(path, mode) != 0 && errno != EEXIST)
            status = -1;
    }
    else if (!S_ISDIR(st.st_mode))
    {
        errno = ENOTDIR;
        status = -1;
    }
	return(status);

#else
	return 0;

#endif
}

std::vector<EntityFile> GetListFilesFromPath(const std::string &inPath, std::string *outPath)
{
	std::vector<EntityFile> listFiles;
#if __linux__
    DIR* dir;
    dirent *ent;
    
    char pathDir[PATH_MAX];
    realpath(inPath.c_str(), pathDir);
    std::string s(pathDir);
    if(s[s.size() -1] != '/')
        s.push_back('/');
    if(outPath != nullptr)
        *outPath = s;

    if((dir = opendir(s.c_str())) != nullptr)
    {
        while((ent = readdir(dir)) != nullptr)
        {

            EntityFile f;
            f.name = ent->d_name;
            f.type = ent->d_type;
            f.directory = s;
            f.fullPath = std::string(s + f.name);
            if(f.type == 4 && f.fullPath[f.fullPath.size() - 1] != '/')
                f.fullPath.push_back('/');

            listFiles.push_back(f);
        }
        closedir(dir);
    }
#else

#endif
    return listFiles;
}

DialogFileBrowser::DialogFileBrowser()
{

}
void DialogFileBrowser::Run(const std::string &path, const std::string &browserName,bool *isOpened, bool folderOnly)
{
    _internaldata.folderOnly = folderOnly;

    Import(path, browserName, isOpened);
}

void DialogFileBrowser::_UpdateData( const std::string& path , bool previous)
{
    _internaldata._currentDirectory = path;
    if(_IsPathAFolder( _internaldata._currentDirectory))
         _internaldata._listFiles = GetListFilesFromPath(_internaldata._currentDirectory);
}

bool DialogFileBrowser::_IsPathAFolder(const std::string &inPath)
{
    return (inPath.size() > 0 && inPath[inPath.size() - 1] == '.')
            ||(inPath.size() > 1 && inPath[inPath.size() - 1] == '/');
}


void DialogFileBrowser::Import(const std::string &path, const std::string &browserName, bool *isOpened)
{
    if(!_internaldata._isVisible)
    {
        ImGui::OpenPopup(browserName.c_str());
        _internaldata._isVisible = true;
        _isValid = false;
        if(_IsPathAFolder(path))
        {
            _internaldata._listFiles = GetListFilesFromPath(path, &_internaldata._currentDirectory);
        }
    }

    if(ImGui::BeginPopupModal(browserName.c_str(), isOpened,ImGuiWindowFlags_AlwaysAutoResize ))
    {


        if(ImGui::Button("<"))
        {
            if(_IsPathAFolder(_internaldata._currentDirectory))
                _internaldata._currentDirectory += "../";

            _UpdateData(_internaldata._currentDirectory);

        }
        ImGui::SameLine();

        if(ImGui::Button("Create folder"))
        {
            std::string newFolder = _internaldata._currentDirectory + "/New Folder";
            CreateFolder(newFolder.c_str());
            _UpdateData(_internaldata._currentDirectory);

        }



        ImGui::BeginChild("Files", ImVec2(300, 200), true, ImGuiWindowFlags_AlwaysAutoResize);

        if(ImGui::ListBoxHeader("##Files_ListBox", _internaldata._listFiles.size()))
        {
            for(size_t i = 0; i < _internaldata._listFiles.size(); ++i)
            {
                 if(ImGui::Selectable(_internaldata._listFiles[i].name.c_str(), editedItem == i, ImGuiSelectableFlags_AllowDoubleClick))
                {
                     _internaldata._currentPath = _internaldata._listFiles[i].fullPath;
                    if (ImGui::IsMouseDoubleClicked(0))
                    {
                        if(_internaldata._listFiles[i].type == 4)//If folder update path
                        {
                            std::cout << _internaldata._listFiles[i].fullPath << std::endl;
                            _UpdateData(_internaldata._listFiles[i].fullPath);
                        }
                    }

                    if(ImGui::IsMouseClicked(1))
                    {
                        std::cout << "Right click" << std::endl;
                    }
                     editedItem = i;
                    _result = _internaldata._listFiles[i];
                }


            }
            ImGui::ListBoxFooter();

        }

        if(ImGui::IsMouseClicked(1))
        {

            ImGui::OpenPopup("popup from button");
        }

        if(ImGui::BeginPopup("popup from button"))
        {
            if(ImGui::MenuItem("Rename"))
            {
                renameWanted = true;
            }
            ImGui::EndPopup();
        }

        static char bufferFile[256];
        if(renameWanted && editedItem != -1)
        {
            strcpy(bufferFile, _internaldata._listFiles[editedItem].name.c_str());
            if(ImGui::InputText("tata", bufferFile, 256, ImGuiInputTextFlags_EnterReturnsTrue))
            {
                std::string newPath = _internaldata._listFiles[editedItem].directory + std::string(bufferFile);
                RenameFile(_internaldata._listFiles[editedItem].fullPath.c_str(),newPath.c_str());
                _UpdateData(_internaldata._currentDirectory);
                renameWanted = false;
                editedItem = -1;
            }
        }

        ImGui::EndChild();

        if(ImGui::Button("Select"))
        {
            ImGui::CloseCurrentPopup();
            _isValid = true;
            *isOpened = false;
        }
        ImGui::SameLine();
        if(ImGui::Button("Cancel"))
        {
            ImGui::CloseCurrentPopup();
            _isValid = false;
            *isOpened = false;
        }

        ImGui::EndPopup();
    }
    _internaldata._isOpened = *isOpened;
    _internaldata._isVisible = *isOpened;
}

void DialogFileBrowser::GetResult(std::string &outFileName, std::string &outFilePath)
{
    outFileName = _result.name;
    outFilePath = _result.fullPath;
}

