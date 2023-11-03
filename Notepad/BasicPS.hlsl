#include "Common.hlsli"

//struct PixelShaderInput
//{
//	float4 pos : SV_POSITION;
//	float3 color : COLOR;
//};

float4 main(PSInput input) : SV_TARGET
{

    // Use the interpolated vertex color
	return float4(float3(1.f, 1.f, 1.f), 1.0);
}
