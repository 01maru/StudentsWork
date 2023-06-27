#include "ScreenHeader.hlsli"

Texture2D<float4> tex: register(t0);
Texture2D<float4> mainTex : register(t1);
SamplerState smp : register(s0); 

float Gaussian(float2 drawUV, float2 pickUV, float sigma)
{
    float d = distance(drawUV, pickUV);
    return exp(-(d * d) / (2 * sigma * sigma));
}

float4 GaussianBlur(float2 uv, float sigma, float stepWidth)
{
    float totalWeight = 0;
    float4 col = float4(0, 0, 0, 0);

    for (float py = -sigma * 2; py <= sigma * 2; py += stepWidth)
    {
        for (float px = -sigma * 2; px <= sigma * 2; px += stepWidth)
        {
            float2 pickUV = uv + float2(px, py);
            float weight = Gaussian(uv, pickUV, sigma);
            col += mainTex.Sample(smp, pickUV) * weight;
            totalWeight += weight;
        }
    }

    col.rgb = col.rgb / totalWeight;
    return col;
}

float4 main(VSOutput input) : SV_TARGET
{
	float depth = tex.Sample(smp, input.uv).r;
	float4 inFocusColor = mainTex.Sample(smp, input.uv);
    float4 outFocusColor = GaussianBlur(input.uv, 0.03, 0.005);
	float focus = 1 - smoothstep(0, 0.05, abs(depth - 0.05));
	return focus * inFocusColor + (1 - focus) * outFocusColor;
}