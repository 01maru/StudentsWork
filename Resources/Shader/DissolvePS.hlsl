#include "DissolveHeader.hlsli"

Texture2D<float4> tex : register(t0);
Texture2D<float4> dissolve : register(t1);
sampler g_sampler : register(s0); 

float4 main(VSOutput input) : SV_TARGET
{
    //  texture‚ÌƒJƒ‰[
    float4 texcolor = tex.Sample(g_sampler, input.uv);
    float4 dissolveMap = dissolve.Sample(g_sampler, input.uv);

    float dissolve = smoothstep(dissolveValue, dissolveValue + 0.2f, dissolveMap.r);

    if (dissolve == 1) return texcolor;
    if (dissolve == 0) discard;

    return dissolveColor;
}