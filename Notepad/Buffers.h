#pragma once

#include <directxtk/SimpleMath.h>


using DirectX::SimpleMath::Matrix;
using DirectX::SimpleMath::Vector3;

struct Vertex {
	Vector3 position;
	Vector3 color;
};

struct MVP {
	Matrix model;
	Matrix view;
	Matrix proj;
};