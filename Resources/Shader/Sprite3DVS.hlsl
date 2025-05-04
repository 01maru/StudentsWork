#include "Sprite3DHeader.hlsli"

VSOutput main(float4 pos : POSITION, float2 uv : TEXCOORD)
{
	VSOutput output;

	output.svpos = mul(mul(matview, matworld), pos);
	output.uv = uv;

	return output;
}