#include "ScreenHeader.hlsli"

Texture2D<float4> mainTex: register(t0);
SamplerState smp : register(s0);

PSOutput main(VSOutput input) : SV_TARGET
{
	PSOutput output;

	float4 texcolor = float4(mainTex.Sample(smp, input.uv));

	//	���]
	output.target0 = float4(1 - texcolor.rgb, 1);
	
	//	���ςڂ���
    float offsetU = 1.5f / 1280.0f;
    float offsetV = 1.5f / 720.0f;

    // �E�̃J���[
    texcolor += mainTex.Sample(smp, input.uv + float2(offsetU, 0.0f));

    // ���̃J���[
    texcolor += mainTex.Sample(smp, input.uv + float2(-offsetU, 0.0f));

    // ���̃J���[
    texcolor += mainTex.Sample(smp, input.uv + float2(0.0f, offsetV));

    // ��̃J���[
    texcolor += mainTex.Sample(smp, input.uv + float2(0.0f, -offsetV));

    // �E���̃J���[
    texcolor += mainTex.Sample(smp, input.uv + float2(offsetU, offsetV));

    // �E��̃J���[
    texcolor += mainTex.Sample(smp, input.uv + float2(offsetU, -offsetV));

    // �����̃J���[
    texcolor += mainTex.Sample(smp, input.uv + float2(-offsetU, offsetV));

    // ����̃J���[
    texcolor += mainTex.Sample(smp, input.uv + float2(-offsetU, -offsetV));

    texcolor /= 9.0f;
	output.target1 = texcolor;

	return output;
}