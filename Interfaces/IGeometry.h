#pragma once

#include "TypeSystem.h"
#include <cstdint>

enum Topology
{
	Triangle,
	Line
};

class IGeometry
{

public:
	virtual void * getVertices() = 0;

	virtual void * getIndices() = 0;

	virtual size_t getVerticesCount() = 0;

	virtual size_t getIndicesCount() = 0;

	virtual uint32_t getStride() const = 0;

	virtual Topology getTopology() const = 0;

	virtual ~IGeometry() = default;

	TypeInterface(IGeometry)
};
