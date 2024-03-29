#include "GFileNavigator.h"
#include "Core/application.h"
#include "Rendering/material.hpp"
#include "Resource/ResourcesManager.h"
#include "Rendering/Shader.h"
#include "nlohmann/json.hpp"
#include <fstream>
#include "imgui/imgui.h"
#include "GMaterialEditor.h"
#include "imgui/misc/cpp/imgui_stdlib.h"
#include "Resource/ResourceLoader.h"
#include "Resources/IconManager.hpp"
#include "imgui/imgui_user.h"
#include "Rendering/Texture2D.hpp"
using namespace gui;


PathStorage::PathStorage()
{

}
PathStorage::~PathStorage()
{
	
}
Node* PathStorage::AddPath(const fm::FilePath &inPath)
{
	std::string relative;
	_GetRelative(inPath, relative);

	return _AddPath(relative, &_root);
}

void PathStorage::SetRoot(const fm::FilePath& inRoot)
{
	_rootpath = inRoot;
	_root.name = inRoot.GetName(false);
}

void PathStorage::Clear(const fm::FilePath& inRoot)
{

}

void PathStorage::_Clear(const std::string &inRelativePath, Node* inCurrentNode)
{
	Node *n = _GetNode(inRelativePath, inCurrentNode);


}




Node* PathStorage::GetNode(const std::string &inRelativePath)
{
	return _GetNode(inRelativePath, &_root);
}

Node* PathStorage::GetNode(const fm::FilePath &inPath)
{
	std::string relative;
	_GetRelative(inPath, relative);
	if(!relative.empty())
		return _GetNode(relative, &_root);

	return &_root;
}

void PathStorage::_GetRelative(const fm::FilePath &inPath, std::string &outRelativePath) const
{
	fm::FilePath::GetRelativeFromRoot(_rootpath, inPath, outRelativePath);
}




Node* PathStorage::_GetNode(const std::string &inRelativePath, Node* inCurrentNode)
{

	size_t pos = inRelativePath.find_first_of(fm::FilePath::GetFolderSeparator());
	std::string name;
	if (pos != std::string::npos)
	{
		name = inRelativePath.substr(0, pos);
	}
	if (!name.empty())
	{
		for (auto && n : inCurrentNode->nodes)
		{
			if (n.name == name)
			{
				std::string p = inRelativePath.substr(pos + 1, inRelativePath.size() - pos - 1);
				if (!p.empty())
					return _GetNode(p, &n);
				else
					return &n;
			}
		}
	}
	else
	{
		name = inRelativePath;

		for (auto && n : inCurrentNode->nodes)
		{
			if (n.name == name)
			{
				return &n;
			}
		}
	}
	
	return nullptr;
}

bool PathStorage::HasRoot() const
{
	return !_root.name.empty();
}

void PathStorage::RefreshPath(const fm::Folder &inPath)
{

	Node* n = GetNode(inPath.GetPath());
	if (n == nullptr)
	{
		n = AddPath(inPath.GetPath());
	}
	n->nodes.clear();

	if (!inPath.Exist())
	{
		return;
	}

	inPath.Iterate(false, [&n](const fm::Folder* inFolder, const fm::File* inFile)
	{
		
		if (inFolder != nullptr)
		{
			std::string name = inFolder->GetPath().GetName(false);

			Node newNode;
			newNode.name = name;
			n->nodes.emplace_back(newNode);
		}

	});
}



Node* PathStorage::_AddPath(const std::string &inRelativePath, Node* inCurrentNode)
{
	size_t pos = inRelativePath.find_first_of(fm::FilePath::GetFolderSeparator(), 0);
	if (pos != std::string::npos)
	{

		std::string name = inRelativePath.substr(0, pos);
		if (!name.empty())
		{
			for (auto && n : inCurrentNode->nodes)
			{
				if (n.name == name)
				{
					std::string p = inRelativePath.substr(pos + 1, inRelativePath.size() - pos - 1);
					return _AddPath(p, &n);
				}
			}

			Node n;
			n.name = name;
			inCurrentNode->nodes.emplace_back(n);

		}
		else
		{
			std::string name = inRelativePath.substr(pos);
			Node n;
			n.name = name;
			bool found = false;
			for (auto && n : inCurrentNode->nodes)
			{
				if (n.name == name)
				{
					found = true;
					break;
				}
			}

			if (!found)
			{
				inCurrentNode->nodes.emplace_back(n);
			}
			return &inCurrentNode->nodes.back();
		}
	}

	return inCurrentNode;
}



GFileNavigator::GFileNavigator() : GWindow("File Navigator", true)
{
	_enabled = true;
	_kind = gui::WINDOWS::WIN_FILE_NAVIGATOR;
	_hasChanged = false;
}


void GFileNavigator::_Update(float dt, Context &inContext)
{
	if (_fileIcon == nullptr)
	{
		_fileIcon = gui::LoadIconEditor("FileIcon.png");
	}

	if (_folderIcon == nullptr)
	{
		_folderIcon = gui::LoadIconEditor("DirectoryIcon.png");
	}

	_root = fm::Folder(fm::FilePath(fm::LOCATION::USER_LOCATION));
	if ((_root.GetPath().GetPath() != _currentFolderSelected.GetPath().GetPath()) && !_currentFolderSelected.Exist())
	{
		_currentFolderSelected = _root;
		if (!_cache.HasRoot() || _cache.GetRoot().GetPath() != _root.GetPath().GetPath())
		{
			_cache.SetRoot(_root.GetPath());
			_cache.RefreshPath(_root);
		}
	}

	if (!_listToRefresh.empty())
	{
		_listFiles.clear();
		_currentFolderSelected.Iterate(false, [this](const fm::Folder* inFolder, const fm::File* inFile)
		{
			if (inFile)
			{
				if(inFile->GetPath().GetExtension() != ".import")
					_listFiles.emplace_back(inFile->GetPath());
			}
		});
	}

	while (!_listToRefresh.empty())
	{
		fm::FilePath path = _listToRefresh.front();
		_listToRefresh.pop();

		_cache.RefreshPath(path);
	}

	if (_pathSelected.has_value())
	{
		inContext.currentPathSelected = _pathSelected;

		if (_pathSelected.value().GetExtension() == ".material")
		{
			inContext.currentWindowToDisplay = gui::WINDOWS::WIN_MATERIAL_EDITOR;
		}
		else if (_pathSelected.value().GetExtension() == ".png" || _pathSelected.value().GetExtension() == ".jpg")
		{
			inContext.currentWindowToDisplay = gui::WINDOWS::WIN_TEXTURE_EDITOR;
		}

		_pathSelected = std::nullopt;
		
	}
}

void GFileNavigator::DrawHierarchy(const fm::FilePath& inRoot, Node* currentNode, bool &popupopened)
{
	for (auto& n : currentNode->nodes)
	{
		fm::FilePath p(inRoot);
		p.ToSub(n.name);

		
		bool opened = TreeNodeWithIcon(n.name.c_str(), (ImTextureID)intptr_t(_folderIcon->GetID()));
		
		if (ImGui::IsItemClicked())
		{
			_listToRefresh.push(p);
			SetPathSelected(p);
		}

		ImGui::OpenPopupOnItemClick("context", ImGuiPopupFlags_MouseButtonRight);
		if (!popupopened && ImGui::BeginPopup("context"))
		{
			popupopened = true;
			if (ImGui::MenuItem("Delete"))
			{
				fm::Folder(p).Delete(true);
				_listToRefresh.push(p);
			}
			else if (ImGui::MenuItem("Create Folder"))
			{
				_listToRefresh.push(_currentFolderSelected.GetPath());
				_currentFolderSelected = fm::Folder(fm::FilePath(_currentFolderSelected.GetPath()).ToSub("New Folder")).CreateUniqueFolder();
			}

			ImGui::EndPopup();
		}
		
		if (opened)
		{
			DrawHierarchy(p, &n, popupopened);

			ImGui::TreePop();
		}
	}
}

void GFileNavigator::SetPathSelected(const fm::FilePath& inFilePath)
{
	_currentFolderSelected = inFilePath;
	_hasChanged = true;
}



void GFileNavigator::CustomDraw()
{
	if (_splitter_1 <= 0)
		_splitter_1 = GetSize().x * 0.5f;

	_splitter_2 = GetSize().x - _splitter_1 - 20;
	float h = GetSize().y - 60;

	Splitter(true, 2.0f, &_splitter_1, &_splitter_2, 2, 2, h);
	if (ImGui::BeginChild("##folderNavigator", ImVec2(_splitter_1, h), false, ImGuiWindowFlags_NoTitleBar))
	{
		bool hasPopupOpened = false;
		DrawHierarchy(_cache.GetRoot(), _cache.GetRootNode(), hasPopupOpened);
		if (!hasPopupOpened && ImGui::BeginPopupContextWindow())
		{
			if (ImGui::MenuItem("Create Folder"))
			{
				_listToRefresh.push(_currentFolderSelected.GetPath());
				_currentFolderSelected = fm::Folder(fm::FilePath(_currentFolderSelected.GetPath()).ToSub("New Folder")).CreateUniqueFolder();
			}
			ImGui::EndPopup();
		}

	}
	ImGui::EndChild();

	ImGui::SameLine();
	
	if (_currentFolderSelected.GetPath().IsValid())
	{

		if(ImGui::BeginChild("##files", ImVec2(_splitter_2, h), false, ImGuiWindowFlags_NoTitleBar))
		{
			bool hasSelectedAnItem = false;
			for (auto && f : _listFiles)
			{
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
				if (f == _pathBeingRenamed)
				{
					if (ImGui::InputText("##test", &_currentRename, ImGuiInputTextFlags_EnterReturnsTrue) || ImGui::IsItemDeactivatedAfterEdit())
					{

						AddEvent([f, this](gui::GWindow* inWindow, std::optional<gui::Context> Context) {
							fm::ResourcesManager::get().Purge(f);
							_listToRefresh.push(_currentFolderSelected.GetPath());
							fm::File newFile = fm::File(f).Rename(_currentRename);
							//TODO: reload
							fm::ResourceLoader loader;
							loader.Init();
							loader.Load(newFile.GetPath(), true);
							loader.SaveImport(newFile.GetPath(), false);

							_pathBeingRenamed.Clear();
							_currentRename.clear();
						});
					}
				}
				else
				{

					if (ImGui::Button(f.GetName(false).c_str()))
					{
						_pathSelected = f;
					}

					if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
					{
						std::string moveData = fm::FileSystem::ConvertPathToFileSystem(f);
						ImGui::SetDragDropPayload("FileNavigator", moveData.c_str(), moveData.length() + 1);

						ImGui::Text("Move %s", f.GetName(false).c_str());
						if (f.GetExtension() == ".png" || f.GetExtension() == ".jpg")
						{
							if (_textureDraging == nullptr)
							{
								_textureDraging = fm::Texture2D::CreateTexture2D(GRAPHIC_API::OPENGL, f);
							}
							if (_textureDraging)
							{
								ImGui::Image(ImTextureID(intptr_t(_textureDraging->GetID())), ImVec2(128, 128));
							}
						}
						ImGui::EndDragDropSource();
					}
				}

				ImGui::PopStyleColor();
				if (ImGui::BeginPopupContextItem())
				{
					hasSelectedAnItem = true;
					if (ImGui::MenuItem("Delete"))
					{
						fm::ResourcesManager::get().Purge(f);
						fm::File(f).Delete();
						_listToRefresh.push(_currentFolderSelected.GetPath());
					}
					else if (ImGui::MenuItem("Rename"))
					{
						_pathBeingRenamed = f;
						_currentRename = f.GetName(false);
					}

					ImGui::EndPopup();
				}
			}


			if (!hasSelectedAnItem && ImGui::BeginPopupContextWindow())
			{
				if (ImGui::MenuItem("Create Material"))
				{
					_listToRefresh.push(_currentFolderSelected.GetPath());
					_pathSelected = GMaterialEditor::CreateNewMaterial(_currentFolderSelected.GetPath());
				}
				ImGui::EndPopup();
			}
		}
		ImGui::EndChild();
	}       
}


GFileNavigator::~GFileNavigator()
{
}
