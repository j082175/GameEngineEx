//cbuffer MVP : register(b0)
//{
//	matrix model;
//	matrix view;
//	matrix proj;
//}

cbuffer MeshConstants : register(b1)
{
	Matrix world;

	int useHeightMap = 0;
	float heightScale = 0.f;
};

cbuffer MaterialConstants : register(b2)
{
	float3 albedoFactor = float3(1.f, 1.f, 1.f);
	float roughnessFactor = 1.f;
	float metallicFactor = 1.f;
	float3 emissionFactor = float3(0.f, 0.f, 0.f);

	int useAlbedoMap = 0;
	int useNormalMap = 0;
	int useroughnessMap = 0;
	int useMetallicMap = 0;
	int useAOMap = 0;
	int useEmissiveMap = 0;


};

cbuffer GlobalConstants : register(b3)
{
	Matrix viewProj;

};

struct VSInput
{
	float3 position : POSITION;
	float3 normalModel : NORMAL;
	float2 texcoord : TEXCOORD;
	float3 tangentModel : TANGENT;
};

struct PSInput
{
	float4 posProj : SV_POSITION;
	float3 normal : NORMAL;
};