#pragma once

#include <string>
#include "ICamera.h"
#include "IAny.h"
#include "Tools/GameTimer.h"

namespace Cefied
{
	namespace Game
	{
		class IScene : public IAny
		{
		public:
			virtual ~IScene() = default;

			virtual bool addCamera(std::string cameraName, ICamera * camera) = 0;
			virtual ICamera * getCamera(std::string cameraName) = 0;

			virtual bool setMainCamera(std::string cameraName) = 0;
			virtual bool setChosenCamera(std::string cameraName) = 0;

			virtual const ICamera * getMainCamera() = 0;
			virtual const ICamera * getChosenCamera() = 0;

			virtual void update(const GameTime& gameTime) = 0;
			virtual void draw(const GameTime& gameTime) = 0;
			
			TypeInherit1(IAny)
		};
	}
}
