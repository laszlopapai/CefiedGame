#pragma once

#include "Geometry.h"
#include <vector>

namespace Cefied
{
	namespace Game
	{

		class Mesh {

		public:
			void buildFromVectorIndexArray(float * vectors, uint32_t vertexCount, uint32_t * indices, uint32_t triangleCount);
			IGeometry * getMeshPart(uint32_t i) {
				return m_meshParts[i];
			}

		private:
			static void calculateFaceNormal(XMFLOAT3 &v0, XMFLOAT3 &v1, XMFLOAT3 &v2, XMFLOAT3 &normal);

		private:
			std::vector<IGeometry*> m_meshParts;
		};
	}
}