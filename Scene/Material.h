#pragma once

#include "Interfaces/IMaterial.h"
#include <d3dx11effect.h>
#include <unordered_map>

namespace Cefied
{
	namespace Game
	{
		class Material : public IMaterial
		{
		public:
			explicit Material(ID3DX11Effect * effect);

			bool setCurrentTecnique(const std::string name) override;
			
			IShaderVariable* getVariable(const std::string name) override;

			ID3DX11EffectTechnique * getCurrentTechnique() const;

			ID3DX11Effect * getEffect() const;

			~Material();

		private:
			ID3DX11Effect               *m_effect{ nullptr };
			ID3DX11EffectTechnique      *m_currentTechnique{ nullptr };

			std::unordered_map<const char*, ID3DX11EffectTechnique*> m_loadedTechniques;

			TypeInherit1(IMaterial)
		};		
	}
}
