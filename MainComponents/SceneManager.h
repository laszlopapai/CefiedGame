#pragma once

#include <string>
#include "Interfaces/IScene.h"
#include <Tools/GameTimer.h>
#include <unordered_map>

namespace Cefied
{
	namespace Game
	{
		class SceneManager : public IAny
		{
		public:
			~SceneManager();

			bool initialize() override;

			bool addScene(const std::string sceneName, IScene * scene);

			bool loadScene(const std::string sceneName);

			IScene *getCurrentScene() const;

		private:
			IScene                                  *m_currentScene { nullptr } ;
			std::unordered_map<std::string, IScene*> m_scenes;
			
			TypeInherit1(IAny)
		};
	}
}