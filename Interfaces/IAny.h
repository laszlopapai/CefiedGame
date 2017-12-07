#pragma once
#include "TypeSystem.h"

namespace Cefied
{
	namespace Game
	{
		class IAny {
		public:
			virtual bool initialize() = 0;
			virtual ~IAny() = default;

			TypeInterface();
		};
	}
}
