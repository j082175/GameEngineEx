#include "Common.hlsli"

PSInput main(VSInput input)
{

	PSInput output;
	float4 pos = float4(input.position, 1.0f);
	pos = mul(pos, world);
	pos = mul(pos, viewProj);

	output.posProj = pos;
	output.normal = input.normalModel;
	
	output.texcoord = input.texcoord;

	return output;
}
