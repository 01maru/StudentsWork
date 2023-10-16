#include "DissolveSpriteHeader.hlsli"

Texture2D<float4> tex : register(t0);
Texture2D<float4> dissolve : register(t1);
sampler g_sampler : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
    float4 texcolor = tex.Sample(g_sampler, input.uv) * color;
    float4 dissolveMap = dissolve.Sample(g_sampler, input.uv);

    float dissolve = smoothstep(dissolveValue + 0.1f, dissolveValue, 1.0f-dissolveMap.r);
	
    if (dissolve == 1) return texcolor;
    if (dissolve == 0) discard;
    if (dissolveValue == 0) discard;

    return texcolor * dissolve;
}