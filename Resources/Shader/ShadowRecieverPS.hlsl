#include "ShadowRecieverHeader.hlsli"

Texture2D<float4> g_albedo : register(t0);
Texture2D<float4> g_shadowMap : register(t1);  // �V���h�E�}�b�v
sampler g_sampler : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	float3 eyedir = normalize(cameraPos - input.worldpos.xyz);
	const float shininess = 4.0f;
	float4 shadercolor = { 0.0f,0.0f,0.0f,0.0f };
	shadercolor.a = m_alpha;
	//	�������C�g
	for (int i = 0; i < DIRLIGHT_NUM; i++) {
		if (dirLights[i].active) {
			float3 dolightnormal = dot(dirLights[i].lightv, input.normal);
			float3 reflect = normalize(-dirLights[i].lightv + 2 * dolightnormal * input.normal);

			float3 ambient = m_ambient;
			float3 diffuse = dolightnormal * m_diffuse;
			float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

			shadercolor.rgb += (ambient + diffuse + specular) * dirLights[i].lightcolor;
		}
	}

    //  texture�̃J���[
    float4 texcolor = g_albedo.Sample(g_sampler, input.uv);

    // ���C�g�r���[�X�N���[����Ԃ���UV��Ԃɍ��W�ϊ����V���h�E�}�b�v��UV�v�Z
    float2 shadowMapUV = input.posInLVP.xy / input.posInLVP.w;
    shadowMapUV *= float2(0.5f, -0.5f);
    shadowMapUV += 0.5f;

    // ���C�g�r���[�X�N���[����Ԃł�Z�l
    float zInLVP = input.posInLVP.z;


    if (shadowMapUV.x > 0.0f && shadowMapUV.x < 1.0f
        && shadowMapUV.y > 0.0f && shadowMapUV.y < 1.0f)
    {
        //  shadowMap�̒l�o��
        float2 shadowValue = g_shadowMap.Sample(g_sampler, shadowMapUV).xy;

        // ���̃s�N�Z�����Օ�����Ă��邩(Lpos��worldpos�̋�����1000�ȉ�)
        if (zInLVP - 0.0005f > shadowValue.x && zInLVP <= 1.0f)
        {
            float depth_sq = shadowValue.x * shadowValue.x;

            // ���U���傫���قǁAvariance�̐��l�͑傫���Ȃ�
            float variance = min(max(shadowValue.y - depth_sq, 0.0001f), 1.0f);

            // ���̃s�N�Z���̃��C�g���猩���[�x�l�ƃV���h�E�}�b�v�̕��ς̐[�x�l�̍������߂�
            float md = zInLVP - shadowValue.x;

            // �����͂��m�������߂�
            float lit_factor = variance / (variance + md * md);

            // �V���h�E�J���[�����߂�
            float3 shadowColor = texcolor.xyz * 0.5f;

            // ����������m�����g���Ēʏ�J���[�ƃV���h�E�J���[����`�⊮
            texcolor.xyz = lerp(shadowColor, texcolor.xyz, 0.04);
        }
    }

    //	�t�H�O
    if (distanceFog.active) {
        float4 fogColor = float4(distanceFog.fogColor, 1.0f);		//�t�H�O�J���[
        const float linerDepth = 1.0 / (distanceFog.fogFar - distanceFog.fogNear);
        float linerPos = length(cameraPos - input.worldpos.xyz) * linerDepth;
        float fogFactor = saturate((distanceFog.fogEnd - linerPos) / (distanceFog.fogEnd - distanceFog.fogStart));

        return lerp(fogColor, shadercolor * texcolor, fogFactor);
    }

    return shadercolor * texcolor;
}