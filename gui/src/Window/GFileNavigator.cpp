#include "GFileNavigator.h"
#include "Core/application.h"
#include "Rendering/material.hpp"
#include "Resource/ResourcesManager.h"
#include "Rendering/Shader.h"
#include "nlohmann/json.hpp"
#include <fstream>
#include "imgui/imgui.h"
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
	_root = fm::Folder(fm::FilePath(fm::LOCATION::USER_LOCATION, ""));
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
		if (_pathSelected.value().GetExtension() == ".material")
		{
			inContext.currentWindowToDisplay = gui::WINDOWS::WIN_MATERIAL_EDITOR;
			inContext.currentPathSelected = _pathSelected;
		}
		_pathSelected = std::nullopt;
	}
}

void GFileNavigator::DrawHierarchy(const fm::FilePath& inRoot, Node* currentNode)
{

	for (auto& n : currentNode->nodes)
	{
		fm::FilePath p(inRoot);
		p.ToSub(n.name);


		bool opened = ImGui::TreeNodeEx(n.name.c_str());
		
		if (ImGui::IsItemClicked())
		{
			_listToRefresh.push(p);
			SetPathSelected(p);
		}
		if (opened)
		{
			DrawHierarchy(p, &n);

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
	if (_splitter_1 == -1)
		_splitter_1 = GetSize().x * 0.5f;

	if (_splitter_2 == -1)
		_splitter_2 = GetSize().x * 0.5f;
	float h = GetSize().y;

	Splitter(true, 8.0f, &_splitter_1, &_splitter_2, 8, 8, h);
	if (ImGui::BeginChild("##folderNavigator", ImVec2(_splitter_1, 0), false, ImGuiWindowFlags_NoTitleBar))
	{
		DrawHierarchy(_cache.GetRoot(), _cache.GetRootNode());
	}
	ImGui::EndChild();

	ImGui::SameLine();
	
	if (_currentFolderSelected.GetPath().IsValid())
	{

		if(ImGui::BeginChild("##files", ImVec2(_splitter_2, 0), false, ImGuiWindowFlags_NoTitleBar))
		{
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			for (auto && f : _listFiles)
			{
				if (ImGui::Button(f.GetName(false).c_str()))
				{
					_pathSelected = f;
				}
				//ImGui::Text(f.GetName(false).c_str());
			}
			ImGui::PopStyleColor();
		}


		if (ImGui::IsWindowFocused() && ImGui::IsMouseClicked(1))
		{
			ImGui::OpenPopup("popup from button");
		}

		if (ImGui::BeginPopup("popup from button"))
		{
			if (ImGui::MenuItem("Create Material"))
			{
				_listToRefresh.push(_currentFolderSelected.GetPath());
				
				fm::FilePath path = fm::FilePath(_currentFolderSelected.GetPath()).ToSub("newMaterial.material");
				fm::FilePath newFilePath = fm::File(path).CreateUniqueFile().GetPath();
				std::shared_ptr<fm::Material> material = std::make_shared<fm::Material>(newFilePath);
			
				nlohmann::json j;
				material->Save(j);
				std::ofstream o(newFilePath.GetPath(), std::ofstream::out);
				o << j << std::endl;
				o.close();
			
				fm::ResourcesManager::get().load<fm::Material>(newFilePath, material);

				_pathSelected = newFilePath;
			}
			ImGui::EndPopup();
		}
		

		ImGui::EndChild();
	}       
}


GFileNavigator::~GFileNavigator()
{
}
