#include "Common.hlsli"

//cbuffer ModelViewProjectionConstantBuffer : register(b0)
//{
//	matrix model;
//	matrix view;
//	matrix projection;
//};

//// Semantics
//// https://learn.microsoft.com/en-us/windows/win32/direct3dhlsl/dx-graphics-hlsl-semantics

//struct VertexShaderInput
//{
//	float3 pos : POSITION;
//	float3 color : COLOR0;
//};

//struct PixelShaderInput
//{
//	float4 pos : SV_POSITION;
//	float3 color : COLOR;
//};

PSInput main(VSInput input)
{

	PSInput output;
	float4 pos = float4(input.position, 1.0f);
	pos = mul(pos, world);
	pos = mul(pos, viewProj);

	output.posProj = pos;
	output.normal = input.normalModel;

	return output;
}
