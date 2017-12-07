#include "Material.h"
#include "ShaderVariable.h"
#include "../Tools/Macros.h"

using namespace Cefied::Game;

TypeSetup(Material)
Material::Material(ID3DX11Effect * effect) :
	m_effect(effect)
{ }

bool Material::setCurrentTecnique(const std::string name)
{
	auto it = m_loadedTechniques.find(name.c_str());
	if (it != m_loadedTechniques.end()) {
		m_currentTechnique = it->second;
		return true;
	}

	auto tecnique = m_effect->GetTechniqueByName(name.c_str());
	if (tecnique->IsValid()) {
		m_currentTechnique = tecnique;
		m_loadedTechniques[name.c_str()] = tecnique;
		return true;
	}

	return false;
}

IShaderVariable* Material::getVariable(const std::string name)
{
	const auto effectVariable = m_effect->GetVariableByName(name.c_str());
	return new ShaderVariable(effectVariable);
}

ID3DX11EffectTechnique * Material::getCurrentTechnique() const
{
	return m_currentTechnique;
}

ID3DX11Effect* Material::getEffect() const
{
	return  m_effect;
}

Material::~Material()
{
	for (auto it = m_loadedTechniques.begin(); it != m_loadedTechniques.end(); ++it) {
		SafeRelease(it->second);
	}
	SafeRelease(m_effect);
}
