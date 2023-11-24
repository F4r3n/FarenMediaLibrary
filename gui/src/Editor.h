#pragma once
#include <FML/Core/Observer.h>
#include <memory>
namespace fm
{
	class FilePath;
	class Scene;
	class Folder;
}

class Editor : public fm::Observable
{
public:

	enum class Event
	{
		ON_PRE_START,
		ON_AFTER_START,
		ON_PRE_STOP,
		ON_AFTER_STOP,
		ON_PRE_LOAD,
		ON_AFTER_LOAD,
		ON_PRE_SCENE_LOAD,
		ON_AFTER_SCENE_LOAD
	};
	Editor();
	inline static Editor& Get()
	{
		static Editor editor;
		return editor;
	}
	std::shared_ptr<fm::Scene>	CreateNewScene(const fm::FilePath& inScenePath);
	std::shared_ptr<fm::Scene>	CreateEditorScene();
	std::shared_ptr<fm::Scene>	RenameScene(std::shared_ptr<fm::Scene> inCurrentScene, const fm::FilePath& inPath);
	std::shared_ptr<fm::Scene>	LoadScene(const fm::FilePath& inPath);
	void						SetCurrentScene(const std::string& inName);
	void						NewProject(const fm::Folder& inPath);

	void						GetLastProjectsOpened(std::vector<fm::FilePath>& outPath) const;
	void						Start();
	void						Stop();
	void						Update();
	std::shared_ptr<fm::Scene>	GetCurrentScene() const;
	std::shared_ptr<fm::Scene>	GetEditorScene() const;
	const std::string&			GetCurrentSceneName() const;
	std::shared_ptr<fm::Scene>	GetScene(const std::string& inName) const;
	void						SerializeCurrentScene();

private:

	void						_SetUserDirectory(const fm::Folder& inPath);
	void						_SaveLastProjectOpened(const fm::FilePath& inFilePath);
	std::string					_nameLastScene;
	std::shared_ptr<fm::Scene>	_editorScene = nullptr;
};