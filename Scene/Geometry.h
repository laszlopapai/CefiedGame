#pragma once

#include "Interfaces/IAny.h"
#include <DirectXMath.h>
#include <vector>
#include "Interfaces/IGeometry.h"

using namespace DirectX;

namespace Cefied
{
	namespace Game
	{
		struct ColorVertex
		{
			XMFLOAT3 position;
			XMFLOAT4 color;
		};

		struct Vertex {
			XMFLOAT3 position;
			XMFLOAT2 texture;
			XMFLOAT3 normal;
		};
		
		template<typename T>
		class Geometry : public IGeometry
		{
		public:
			void * getVertices() override {
				return m_vertices.data();
			}
			
			void * getIndices() override {
				return m_indices.data();
			}

			size_t getVerticesCount() override {
				return m_vertices.size();
			}

			size_t getIndicesCount() override {
				return m_indices.size();
			}

			uint32_t getStride() const override {
				return sizeof(T);
			}


			Topology getTopology() const override {
				return m_topology;
			}

			void setTopology(const Topology topology) {
				m_topology = topology;
			}

			Geometry() = default;

			~Geometry() = default;

			std::vector<T> & getVerticesData() {
				return m_vertices;
			}

			std::vector<uint16_t> & getIndicesData() {
				return m_indices;
			}

			void addVertex(T & vertex) {
				m_vertices.push_back(vertex);
			}

			void addIndex(uint16_t i1, uint16_t i2, uint16_t i3) {
				m_indices.push_back(i1);
				m_indices.push_back(i2);
				m_indices.push_back(i3);
			}

			void addIndex(uint16_t i1, uint16_t i2) {
				m_indices.push_back(i1);
				m_indices.push_back(i2);
			}

			static void generateSphere(std::vector<T> & vertices, std::vector<uint16_t> & indices, 
				const float radius = .5f, const int tessellation = 32);
			static void generateCube(std::vector<T> & vertices, std::vector<uint16_t> & indices);
			
		private:
			std::vector<T>   m_vertices;
			std::vector<uint16_t> m_indices;
			Topology m_topology { Triangle };

			TypeInherit1(IGeometry)
		};
	}
}