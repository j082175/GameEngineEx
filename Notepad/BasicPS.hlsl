#include "Common.hlsli"

Texture2D g_texture0 : register(t0);
SamplerState g_sampler : register(s0);

float4 main(PSInput input) : SV_TARGET
{

    // Use the interpolated vertex color
	//return float4(float3(1.f, 1.f, 1.f), 1.0);
	
	return g_texture0.Sample(g_sampler, input.texcoord);
}
