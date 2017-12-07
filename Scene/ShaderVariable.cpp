#include "ShaderVariable.h"
#include "../Tools/Macros.h"

using namespace Cefied::Game;

TypeSetup(ShaderVariable)
ShaderVariable::ShaderVariable(ID3DX11EffectVariable * variable) :
	m_variable(variable)
{ }

bool ShaderVariable::setVector(const XMVECTOR& vector)
{
	auto vec = m_variable->AsVector();
	if (vec->IsValid()) {
		vec->SetFloatVector((float*)&vector);
		return true;
	}

	return false;
}

bool ShaderVariable::setMatrix(const XMMATRIX& matrix)
{
	auto mtx = m_variable->AsMatrix();
	if (mtx->IsValid()) {
		mtx->SetMatrix((float*)&matrix);
		return false;
	}
	
	return false;
}

bool ShaderVariable::setScalar(const float scalar)
{
	auto scal = m_variable->AsScalar();
	if (scal->IsValid()) {
		scal->SetFloat(scalar);
		return true;
	}

	return false;
}

ShaderVariable::~ShaderVariable()
{
	SafeRelease(m_variable);
}
