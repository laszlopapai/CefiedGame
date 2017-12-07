#include "Geometry.h"
#include "Tools/Macros.h"

using namespace Cefied::Game;

TypeSetup(Geometry<Vertex>)
TypeSetup(Geometry<ColorVertex>)

template <typename T>
void Geometry<T>::generateSphere(std::vector<T>& vertices, std::vector<uint16_t>& indices, 
	const float radius, const int tessellation)
{
	//TODO: Implement geometry creation for vertex types
}

template <typename T>
void Geometry<T>::generateCube(std::vector<T>& vertices, std::vector<uint16_t>& indices)
{
	//TODO: Implement geometry creation for vertex types
}


void Geometry<Vertex>::generateSphere(std::vector<Vertex>& vertices, std::vector<uint16_t>& indices,
	const float radius, const int tessellation)
{
	const auto verticalSegments = tessellation;
	const auto horizontalSegments = tessellation * 2;
	
	Vertex v;

	// Start with a single vertex at the bottom of the sphere.
	v.normal = XMFLOAT3(0, -1, 0);
	v.position = XMFLOAT3(0, -radius, 0);
	v.texture = XMFLOAT2(.5f, 0);
	vertices.push_back(v);

	// Create rings of vertices at progressively higher latitudes.
	for (auto i = 0; i < verticalSegments - 1; i++)
	{
		const auto latitude = ((i + 1) * DirectX::XM_PI / verticalSegments) - DirectX::XM_PIDIV2;

		const auto dy = sin(latitude);
		const auto dxz = cos(latitude);

		// Create a single ring of vertices at this latitude.
		for (auto j = 0; j < horizontalSegments; j++)
		{
			const auto longitude = j * DirectX::XM_2PI / horizontalSegments;

			const auto dx = cos(longitude) * dxz;
			const auto dz = sin(longitude) * dxz;

			v.normal = XMFLOAT3(dx, dy, dz);
			v.position = XMFLOAT3(dx * radius, dy * radius, dz * radius);
			v.texture = XMFLOAT2(j / (float)horizontalSegments, (i + 1) / (float)verticalSegments);

			vertices.push_back(v);
		}
	}

	// Finish with a single vertex at the top of the sphere.
	v.normal = XMFLOAT3(0, 1, 0);
	v.position = XMFLOAT3(0, radius, 0);
	v.texture = XMFLOAT2(.5f, 0);
	vertices.push_back(v);


	// Create a fan connecting the bottom vertex to the bottom latitude ring.
	for (auto i = 0; i < horizontalSegments; i++)
	{
		indices.push_back(1 + i);
		indices.push_back(1 + (i + 1) % horizontalSegments);
		indices.push_back(0);
	}

	// Fill the sphere body with triangles joining each pair of latitude rings.
	for (int i = 0; i < verticalSegments - 2; i++)
	{
		for (int j = 0; j < horizontalSegments; j++)
		{
			const auto nextI = i + 1;
			const auto nextJ = (j + 1) % horizontalSegments;

			indices.push_back(1 + nextI * horizontalSegments + j);
			indices.push_back(1 + i * horizontalSegments + nextJ);
			indices.push_back(1 + i * horizontalSegments + j);

			indices.push_back(1 + nextI * horizontalSegments + j);
			indices.push_back(1 + nextI * horizontalSegments + nextJ);
			indices.push_back(1 + i * horizontalSegments + nextJ);
		}
	}

	// Create a fan connecting the top vertex to the top latitude ring.
	for (int i = 0; i < horizontalSegments; i++)
	{
		indices.push_back((uint16_t)vertices.size() - 2 - i);
		indices.push_back((uint16_t)vertices.size() - 2 - (i + 1) % horizontalSegments);
		indices.push_back((uint16_t)vertices.size() - 1);
	}
}

void Geometry<Vertex>::generateCube(std::vector<Vertex>& vertices, std::vector<uint16_t>& indices)
{
	vertices.resize(24);
	indices.resize(36);

	XMVECTOR normal[] =
	{
		{ 0, 0, 1, 0 },
		{ 0, 0, -1, 0 },
		{ 1, 0, 0, 0 },
		{ -1, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, -1, 0, 0 },
	};


	for (int i = 0, v = 0; i < 6; i++)
	{
		XMVECTOR side1 = XMVectorSet(XMVectorGetY(normal[i]), XMVectorGetZ(normal[i]), XMVectorGetX(normal[i]), 0);
		XMVECTOR side2 = XMVector3Cross(normal[i], side1);

		indices[i * 6 + 0] = v + 2;
		indices[i * 6 + 1] = v + 1;
		indices[i * 6 + 2] = v + 0;

		indices[i * 6 + 3] = v + 3;
		indices[i * 6 + 4] = v + 2;
		indices[i * 6 + 5] = v + 0;


		XMStoreFloat3(&vertices[v].position, (normal[i] - side1 - side2) * .5f);
		XMStoreFloat3(&vertices[v].normal, normal[i]);
		vertices[v++].texture = XMFLOAT2(i / 6.0f, 0.0f);

		XMStoreFloat3(&vertices[v].position, (normal[i] - side1 + side2) * .5f);
		XMStoreFloat3(&vertices[v].normal, normal[i]);
		vertices[v++].texture = XMFLOAT2(i / 6.0f, 1.0f);

		XMStoreFloat3(&vertices[v].position, (normal[i] + side1 + side2) * .5f);
		XMStoreFloat3(&vertices[v].normal, normal[i]);
		vertices[v++].texture = XMFLOAT2((i + 1) / 6.0f, 1.0f);

		XMStoreFloat3(&vertices[v].position, (normal[i] + side1 - side2) * .5f);
		XMStoreFloat3(&vertices[v].normal, normal[i]);
		vertices[v++].texture = XMFLOAT2((i + 1) / 6.0f, 0.0f);
	}
}
