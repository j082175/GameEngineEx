#pragma once

#include "MeshData.h"
#include <directxtk/SimpleMath.h>

using DirectX::SimpleMath::Vector2;

class GeometryGenerator
{
public:
	static MeshData MakeSquare(const float scale, const Vector2 texScale);
	static MeshData MakeBox();
};

