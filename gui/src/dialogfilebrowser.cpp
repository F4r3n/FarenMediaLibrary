#include "dialogfilebrowser.h"
#include <sstream>
#include <imgui/imgui.h>
#include <iostream>
#include <cstdio>
#include <filesystem>

namespace fs = std::filesystem;

int IsDirectory(const char *inPath)
{
	fs::path path(inPath);
	return fs::is_directory(path);
}


void RenameFile(const char* path, const char* newPath)
{
	std::filesystem::rename(path, newPath);
}


bool CreateFolder(const char *path)
{
	if (!fs::is_directory(path))
	{
		return fs::create_directory(path);
	}
	return false;
}

std::vector<EntityFile> GetListFilesFromPath(const std::string &inPath)
{
	std::vector<EntityFile> listFiles;

	if (fs::is_directory(inPath))
	{
		for (auto& file : fs::directory_iterator(inPath))
		{
			EntityFile f;
			f.name = file.path().filename().u8string();
			f.isDirectory = file.is_directory();
			f.parentDirectory = inPath;
			f.fullPath = file.path().u8string();
			
			listFiles.push_back(f);
		}
	}

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

void DialogFileBrowser::_UpdateData( const std::string& path)
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
            _internaldata._listFiles = GetListFilesFromPath(path);
			_internaldata._currentDirectory = path;
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
                        if(_internaldata._listFiles[i].isDirectory)//If folder update path
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
                std::string newPath = _internaldata._listFiles[editedItem].parentDirectory + std::string(bufferFile);
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

