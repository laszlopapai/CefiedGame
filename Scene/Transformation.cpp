#include "Transformation.h"

using namespace DirectX;
using namespace Cefied::Game;

TypeSetup(Transformation)

bool Transformation::initialize()
{
	m_quaternio   = XMQuaternionIdentity();
	m_translation = XMVectorZero();
	m_scale       = XMVectorSet(1.0f, 1.0f, 1.0f, 0);

	return true;
}

XMMATRIX Transformation::getMatrix() const
{
	if (m_parentTransformation == nullptr)
		return XMMatrixRotationQuaternion(m_quaternio) * XMMatrixTranslationFromVector(m_translation) * XMMatrixScalingFromVector(m_scale);
	
	return m_parentTransformation->getMatrix() * XMMatrixRotationQuaternion(m_quaternio) * XMMatrixTranslationFromVector(m_translation) * XMMatrixScalingFromVector(m_scale);
}
