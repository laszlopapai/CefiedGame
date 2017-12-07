#pragma once

#include "IAny.h"
#include "Tools/GameTimer.h"

namespace Cefied
{
	namespace Game
	{
		class IObject : public IAny
		{
		public:
			virtual void update(const GameTime &gameTime) = 0;
			virtual ~IObject() = default;

			TypeInherit1(IAny)
		};
	}
}
