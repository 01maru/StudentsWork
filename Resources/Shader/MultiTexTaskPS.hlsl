#include "ScreenHeader.hlsli"

Texture2D<float4> tex: register(t0);
Texture2D<float4> mainTex : register(t1);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	float4 color0 = tex.Sample(smp,input.uv);
	float4 color1 = mainTex.Sample(smp, input.uv);

	float4 color = color0;
	if (fmod(input.uv.y, 0.1f) < 0.05f) {
		color = color1;
	}

	return float4(color.rgb, 1);
}