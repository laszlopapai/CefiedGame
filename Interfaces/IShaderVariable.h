#pragma once

#include <DirectXMath.h>
#include "Interfaces/TypeSystem.h"

using namespace DirectX;


namespace Cefied
{
	namespace Game
	{
		class IShaderVariable
		{
		public:
			virtual bool setVector(const XMVECTOR & vector) = 0;

			virtual bool setMatrix(const XMMATRIX & vector) = 0;

			virtual bool setScalar(const float vector) = 0;

			virtual ~IShaderVariable() = default;

			TypeInterface()
		};		
	}
}