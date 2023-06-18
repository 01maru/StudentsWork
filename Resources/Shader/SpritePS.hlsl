#include "SpriteHeader.hlsli"

Texture2D<float4> tex: register(t0);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	float4 output = float4(tex.Sample(smp,input.uv)) * color;

	//	���ߒl0�Ȃ�`�悵�Ȃ�
	if (output.a == 0.0f) {
		discard;
	}

	return output;
}