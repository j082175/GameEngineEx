#pragma once

#include "Buffers.h"
#include <vector>
#include <string>

struct MeshData {
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	std::string albedoFilename;
	std::string roughnessFilename;
	std::string metallicFilename;
	std::string normalFilename;
	std::string heightFilename;
	std::string emissionFilename;
};