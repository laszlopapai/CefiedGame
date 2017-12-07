#pragma once

#include "Interfaces/IShaderVariable.h"
#include <d3dx11effect.h>

namespace Cefied
{
	namespace Game
	{
		class ShaderVariable : public IShaderVariable
		{
		public:
			explicit ShaderVariable(ID3DX11EffectVariable * variable);
			
			bool setVector(const XMVECTOR & vector) override;

			bool setMatrix(const XMMATRIX & vector) override;

			bool setScalar(const float vector) override;

			~ShaderVariable();

		private:
			ID3DX11EffectVariable *m_variable { nullptr };

			TypeInherit1(IShaderVariable)
		};		
	}
}
