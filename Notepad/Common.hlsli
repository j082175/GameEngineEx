cbuffer MVP : register(b0)
{
	matrix model;
	matrix view;
	matrix proj;
}

struct VSInput
{
	float3 position : POSITION;
	float3 color : COLOR;
};

struct PSInput
{
	float4 position : SV_POSITION;
	float3 color : COLOR;
};