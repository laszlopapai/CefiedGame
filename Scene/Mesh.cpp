#include "Mesh.h"
#include <unordered_map>
#include <limits.h>

using namespace Cefied::Game;

void Mesh::buildFromVectorIndexArray(float * vectors, const uint32_t vertexCount, uint32_t * indices, const uint32_t triangleCount) {
	
	Vertex v;
	std::unordered_map<uint32_t, uint16_t> indexMap(UINT16_MAX);

	for (uint32_t i = 0; i < triangleCount * 3; i += 3)
	{
		indexMap.clear();
		auto geometry = new Geometry<Vertex>;
		auto &vertices = geometry->getVerticesData();

		for (; i < triangleCount * 3 && vertices.size() < UINT16_MAX - 3; i += 3)
		{

			uint32_t triangel[] = {
				indices[i + 0],
				indices[i + 1],
				indices[i + 2]
			};

			for (auto vComponent = 0; vComponent < 3; vComponent++)
			{
				const auto it = indexMap.find(triangel[vComponent]);
				if (it == indexMap.end())
				{
					std::pair<uint32_t, uint16_t> indexBind;
					indexBind.first = triangel[vComponent];
					triangel[vComponent] = indexBind.second = (uint16_t)vertices.size();
					indexMap.insert(indexBind);

					v.position = XMFLOAT3(
						vectors[indexBind.first * 3 + 0], 
						vectors[indexBind.first * 3 + 1], 
						vectors[indexBind.first * 3 + 2]
					);
					
					geometry->addVertex(v);
				}
				else
				{
					triangel[vComponent] = it->second;
				}
			}

			geometry->addIndex(triangel[0], triangel[1], triangel[2]);

			auto& vt0 = vertices[triangel[0]];
			auto& vt1 = vertices[triangel[1]];
			auto& vt2 = vertices[triangel[2]];

			calculateFaceNormal(vt0.position, vt1.position, vt2.position, vt0.normal);
			vt2.normal = vt1.normal = vt0.normal;
			
		}

		m_meshParts.push_back(geometry);
	}

}

void Mesh::calculateFaceNormal(XMFLOAT3 &v0, XMFLOAT3 &v1, XMFLOAT3 &v2, XMFLOAT3 &normal)
{
	const auto vf0 = XMLoadFloat3(&v0);
	auto vf1 = XMLoadFloat3(&v1);
	auto vf2 = XMLoadFloat3(&v2);

	vf1 = XMVector3Normalize(vf1 - vf0);
	vf2 = XMVector3Normalize(vf2 - vf0);

	XMStoreFloat3(&normal, XMVector3Cross(vf1, vf2));
}
