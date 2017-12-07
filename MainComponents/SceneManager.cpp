#include "SceneManager.h"
#include "Tools/Macros.h"

using namespace Cefied::Game;

TypeSetup(SceneManager)

bool SceneManager::initialize()
{
	return true;
}

SceneManager::~SceneManager()
{
	for (auto it = m_scenes.begin(); it != m_scenes.end(); ++it)
		SafeDelete(it->second);
}

bool SceneManager::addScene(const std::string sceneName, IScene * scene)
{
	InitCheck(m_scenes.find(sceneName) == m_scenes.end(),
		"Already exists a scene with the same name.");

	m_scenes[sceneName] = scene;
	return true;
}

bool SceneManager::loadScene(const std::string sceneName)
{
	InitCheck(m_scenes.find(sceneName) != m_scenes.end(),
		"There isn't any scene with this name: " + sceneName);

	m_currentScene = m_scenes[sceneName];
	return true;
}

IScene* SceneManager::getCurrentScene() const
{
	return m_currentScene;
}
