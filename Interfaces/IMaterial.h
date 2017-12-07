#pragma once

#include "Interfaces//TypeSystem.h"
#include "IShaderVariable.h"
#include <string>

namespace Cefied
{
	namespace Game
	{
		class IMaterial
		{
		public:

			virtual IShaderVariable * getVariable(const std::string name) = 0;

			virtual bool setCurrentTecnique(const std::string name) = 0;

			virtual ~IMaterial() = default;

			TypeInterface()
		};
	}
}
