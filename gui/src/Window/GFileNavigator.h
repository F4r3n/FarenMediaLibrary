#pragma once
#include <memory>
#include "Window/GWindow.h"
#include "Core/FilePath.h"
#include <queue>

namespace fm
{
	class GameObject;
}


struct Node
{
	std::string name;
	std::vector<Node> nodes;
	bool refresh;
	Node(const fm::FilePath &inFilePath)
	{
		name = inFilePath.GetName(false);
		refresh = false;
	}

	Node()
	{
		name = "";
		refresh = false;
	}

};

class PathStorage
{
public:
	PathStorage();
	~PathStorage();
	Node* AddPath(const fm::FilePath &inPath);
	void SetRoot(const fm::FilePath& inRoot);
	const fm::FilePath& GetRoot() const { return _rootpath; }
	Node* GetRootNode() { return &_root; }

	void Clear(const fm::FilePath& inRoot);
	bool HasRoot() const;
	Node* GetNode(const std::string &inRelativePath);
	Node* GetNode(const fm::FilePath &inPath);
	void RefreshPath(const fm::Folder &inPath);
private:
	Node* _GetNode(const std::string &inRelativePath, Node* inCurrentNode);
	void _GetRelative(const fm::FilePath &inPath, std::string &outRelativePath) const;


	void _Clear(const std::string &inRelativePath, Node* inCurrentNode);
	Node* _AddPath(const std::string &inRelativePath, Node* inCurrentNode);
	Node _root;
	fm::FilePath _rootpath;

};



namespace gui
{
	class GFileNavigator : public GWindow
	{
	public:
		GFileNavigator();
		~GFileNavigator();
		void CustomDraw();

		void DrawHierarchy(const fm::FilePath&, Node* currentNode);
		void SetPathSelected(const fm::FilePath& inFilePath);

	private:
		void _Update(float dt, Context &inContext);
		fm::Folder _root;
		fm::Folder _currentFolderSelected;
		bool		_hasChanged;
		PathStorage _cache;
		std::queue<fm::FilePath> _listToRefresh;
		std::vector<fm::FilePath> _listFiles;

		std::optional<fm::FilePath> _pathSelected;

		//fm::FilePath
	};
}

