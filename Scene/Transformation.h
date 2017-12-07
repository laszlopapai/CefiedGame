#pragma once

#include <DirectXMath.h>
#include "Interfaces/IAny.h"

using namespace DirectX;

namespace Cefied
{
	namespace Game
	{
		class Transformation : public IAny
		{
		public:
			Transformation()
			{
				Transformation::initialize();
			}

			bool initialize() override;

			void setParent(Transformation *parent)
			{
				m_parentTransformation = parent;
			}

			Transformation * getParent() const
			{
				return m_parentTransformation;
			}

			void move(const XMVECTOR step)
			{
				m_translation += step;
			}

			void move(const float x, const float y, const float z)
			{
				m_translation.m128_f32[0] += x;
				m_translation.m128_f32[1] += y;
				m_translation.m128_f32[2] += z;
			}

			void setTranslation(const XMVECTOR translation)
			{
				m_translation = translation;
			}

			XMVECTOR & getTranslation()
			{
				return m_translation;
			}

			const XMVECTOR & getTranslation() const
			{
				return m_translation;
			}

			void rotate(const XMVECTOR quaternion)
			{
				m_quaternio = XMQuaternionMultiply(quaternion, m_quaternio);
			}

			void rotateRollPitchYaw(const float roll, const float pitch, const float yaw)
			{
				rotate(XMQuaternionRotationRollPitchYaw(roll, pitch, yaw));
			}
			
			void setRotation(const XMVECTOR quaternion)
			{
				m_quaternio = quaternion;
			}

			XMVECTOR & getRotation()
			{
				return m_quaternio;
			}

			void scale(const XMVECTOR scale)
			{
				m_scale += scale;
			}

			void setScale(const XMVECTOR scale)
			{
				m_scale = scale;
			}

			XMVECTOR & getScale()
			{
				return m_scale;
			}

			XMMATRIX getMatrix() const;

		private:
			XMVECTOR        m_scale;
			XMVECTOR        m_quaternio;
			XMVECTOR        m_translation;
			Transformation *m_parentTransformation { nullptr } ;

			TypeInherit1(IAny)
		};
	}
}
