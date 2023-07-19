#include "DissolveHeader.hlsli"

Texture2D<float4> tex : register(t0);
Texture2D<float4> dissolve : register(t1);
sampler g_sampler : register(s0); 

float4 main(VSOutput input) : SV_TARGET
{
    //  texture‚ÌƒJƒ‰[
    float4 texcolor = tex.Sample(g_sampler, input.uv);
    float4 dissolveMap = dissolve.Sample(g_sampler, input.uv);

    //  0.5ˆÈ‰º‚ğ•`‰æ”jŠü
    float dissolve = smoothstep(dissolveValue, dissolveValue + 0.1f, dissolveMap.r);

    if (dissolve == 1) return texcolor;
    if (dissolve == 0) discard;

    dissolve += 1.0f;
    return float4(dissolve, 0, 0, 1.0f);
}