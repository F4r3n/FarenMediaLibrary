#include "dialogfilebrowser.h"
#include <sstream>
#include <imgui.h>
#include <iostream>

//For linux and mac
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <glob.h>
#include <dirent.h>

bool DialogFileBrowser::_isOpened = false;
bool DialogFileBrowser::_isVisible = false;



int IsDirectory(const char *path)
{
   struct stat statbuf;
   if (stat(path, &statbuf) != 0)
       return 0;
   return S_ISDIR(statbuf.st_mode);
}

std::vector<std::string> GetListFilesFromPattern(const std::string & inPattern)
{
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
    vector<string> filenames;
    for(size_t i = 0; i < glob_result.gl_pathc; ++i)
    {
        filenames.push_back(string(glob_result.gl_pathv[i]));
    }

        // cleanup
    globfree(&glob_result);

        // done
    return filenames;
}

std::vector<EntityFile> GetListFilesFromPath(const std::string &inPath)
{
    DIR* dir;
    dirent *ent;
    std::vector<EntityFile> listFiles;
    if((dir = opendir(inPath.c_str())) != nullptr)
    {

        while((ent = readdir(dir)) != nullptr)
        {
            listFiles.push_back({ent->d_name, ent->d_type});
        }
        closedir(dir);
    }
    return listFiles;
}

DialogFileBrowser::DialogFileBrowser()
{

}


void DialogFileBrowser::Run(const std::string &path, const std::string &browserName, bool *isOpened)
{
    if(!_isVisible)
    {
        ImGui::OpenPopup(browserName.c_str());
        _isVisible = true;
        _currentFolderPath = path;
        _listFiles = GetListFilesFromPath(_currentFolderPath);
    }
    //ImGui::SetNextWindowPos(ImVec2(200, 200));
    //ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(400,200));
    //ImGui::SetNextWindowSizeConstraints(ImVec2(400,200), ImVec2(800,600));
    if(ImGui::BeginPopupModal(browserName.c_str(), isOpened,ImGuiWindowFlags_AlwaysAutoResize ))
    {

        ImGui::BeginChild("Folders", ImVec2(200, 200), true, ImGuiWindowFlags_HorizontalScrollbar);

        for(size_t i = 0; i < _listFiles.size(); ++i)
        {
            if (_listFiles[i].type == 4 && ImGui::Selectable(_listFiles[i].name.c_str()))
            {
                _currentFolderPath += "/" +  _listFiles[i].name;
                std::cout << _currentFolderPath << std::endl;
                _listFiles = GetListFilesFromPath(_currentFolderPath);

            }

        }
        ImGui::EndChild();
        ImGui::SameLine();
        ImGui::BeginChild("Files", ImVec2(200, 200), true, ImGuiWindowFlags_HorizontalScrollbar);

         for(size_t i = 0; i < _listFiles.size(); ++i)
         {
            if(_listFiles[i].type == 8)
                ImGui::BulletText(_listFiles[i].name.c_str());

         }

        ImGui::EndChild();


      ImGui::EndPopup();
    }
    //ImGui::PopStyleVar(ImGuiStyleVar_WindowMinSize);

    _isOpened = *isOpened;
    _isVisible = *isOpened;
}

void DialogFileBrowser::_Open()
{

}

void DialogFileBrowser::_Close()
{


}
