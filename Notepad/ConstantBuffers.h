#pragma once

#include <directxtk/SimpleMath.h>

using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Matrix;

#define MAX_LIGHTS 3
#define LIGHT_OFF 0x00
#define LIGHT_DIRECTIONAL 0x01
#define LIGHT_POINT 0x02
#define LIGHT_SPOT 0x04
#define LIGHT_SHADOW 0x10

__declspec(align(256))
struct MeshConstants {
	Matrix world;

	int useHeightMap = 0;
	float heightScale = 0.f;
};

static_assert(sizeof(MeshConstants) % 16 == 0);

__declspec(align(256))
struct MaterialConstants {
	Vector3 albedoFactor = Vector3(1.f);
	float roughnessFactor = 1.f;
	float metallicFactor = 1.f;
	Vector3 emissionFactor = Vector3(0.f);

	int useAlbedoMap = 0;
	int useNormalMap = 0;
	int useroughnessMap = 0;
	int useMetallicMap = 0;
	int useAOMap = 0;
	int useEmissiveMap = 0;


};

static_assert(sizeof(MaterialConstants) % 16 == 0);

__declspec(align(256))
struct GlobalConstants {
	Matrix viewProj;

};

static_assert(sizeof(GlobalConstants) % 16 == 0);

__declspec(align(256))
struct Light {
	Vector3 radiance = Vector3(5.f);
	float fallOffStart = 0.f;
	Vector3 direction = Vector3(0.f, 0.f, 1.f);
	float fallOffEnd = 20.f;
	Vector3 position = Vector3(0.f, 0.f, -2.f);
	float spotPower = 6.f;
	float radius = 0.035f;
	float haloRadius = 0.f;
	float haloStrength = 0.f;

	Matrix viewProjForShadowRendering;
	Matrix invProjForShadowDebugging;

	uint32_t type = LIGHT_OFF;
};

static_assert(sizeof(Light) % 16 == 0);