#pragma once

#include "IObject.h"
#include "Tools/GameTimer.h"

namespace Cefied
{
	namespace Game
	{
		class IDrawable : public IObject
		{
		public:
			virtual void draw(const GameTime &gameTime) = 0;
			virtual ~IDrawable() = default;

			TypeInherit1(IObject)
		};
	}
}
