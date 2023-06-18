#include "ScreenHeader.hlsli"

Texture2D<float4>tex : register(t0);
Texture2D<float4> g_bokeTexture_1 : register(t1);
Texture2D<float4> g_bokeTexture_2 : register(t2);
Texture2D<float4> g_bokeTexture_3 : register(t3);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	float4 output = float4(tex.Sample(smp, input.uv));
	output += g_bokeTexture_1.Sample(smp, input.uv);
	//output += g_bokeTexture_2.Sample(smp, input.uv);
	//output += g_bokeTexture_3.Sample(smp, input.uv);
	//output /= 2.0f;
	output.a = 1.0f;

	//// step-1 ��e�N�Z��+�ߖT8�e�N�Z���̕��ς��v�Z����
	//// 2.5�e�N�Z�������炷���߂�UV�l�����߂�
	//float offsetU = 1.5f / 1280.0f;
	//float offsetV = 1.5f / 720.0f;

	//// ��e�N�Z������E�̃e�N�Z���̃J���[���T���v�����O����
	//output += tex.Sample(smp, input.uv + float2(offsetU, 0.0f));

	//// ��e�N�Z�����獶�̃e�N�Z���̃J���[���T���v�����O����
	//output += tex.Sample(smp, input.uv + float2(-offsetU, 0.0f));

	//// ��e�N�Z�����牺�̃e�N�Z���̃J���[���T���v�����O����
	//output += tex.Sample(smp, input.uv + float2(0.0f, offsetV));

	//// ��e�N�Z�������̃e�N�Z���̃J���[���T���v�����O����
	//output += tex.Sample(smp, input.uv + float2(0.0f, -offsetV));

	//// ��e�N�Z������E���̃e�N�Z���̃J���[���T���v�����O����
	//output += tex.Sample(smp, input.uv + float2(offsetU, offsetV));

	//// ��e�N�Z������E��̃e�N�Z���̃J���[���T���v�����O����
	//output += tex.Sample(smp, input.uv + float2(offsetU, -offsetV));

	//// ��e�N�Z�����獶���̃e�N�Z���̃J���[���T���v�����O����
	//output += tex.Sample(smp, input.uv + float2(-offsetU, offsetV));

	//// ��e�N�Z�����獶��̃e�N�Z���̃J���[���T���v�����O����
	//output += tex.Sample(smp, input.uv + float2(-offsetU, -offsetV));

	//// ��e�N�Z���ƋߖT8�e�N�Z���̕��ςȂ̂�9�ŏ��Z����
	//output /= 9.0f;
	output *= color;
	//if (output.a == 0.0f) {
	//	discard;
	//}

	return output;
}