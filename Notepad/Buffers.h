#pragma once

#include <directxtk/SimpleMath.h>


using DirectX::SimpleMath::Matrix;
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Vector2;

struct Vertex {
	Vector3 position;
	Vector3 normalModel;
	Vector2 texcoord;
	Vector3 tangentModel;
};

struct MVP {
	Matrix model;
	Matrix view;
	Matrix proj;
};