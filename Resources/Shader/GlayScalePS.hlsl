#include "GlayScaleHeader.hlsli"

Texture2D<float4>tex : register(t0);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	float4 output = float4(tex.Sample(smp, input.uv));

	if (activeGlayScale) {
		float p = (0.29891 * output.x + 0.58661 * output.y + 0.11448 * output.z);

		output = float4(p, p, p, 1.0f);
	}

	return output;
}