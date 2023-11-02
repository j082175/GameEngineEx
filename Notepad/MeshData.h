#pragma once

#include "Buffers.h"
#include <vector>

struct MeshData {
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
};