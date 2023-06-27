#include "ObjHeader.hlsli"

Texture2D<float4> tex: register(t0);
SamplerState smp : register(s0);


PSOutput main(VSOutput input)
{
	PSOutput output;
	float3 eyedir = normalize(cameraPos - input.worldpos.xyz);
	const float shininess = 4.0f;
	float4 shadercolor = { 0.0f,0.0f,0.0f,0.0f };
	shadercolor.a = m_alpha;
	//	方向ライト
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

	float4 texcolor = float4(tex.Sample(smp, input.uv));

	//	フォグ
	if (distanceFog.active) {
		float4 fogColor = float4(distanceFog.fogColor, 1.0f);		//フォグカラー
		const float linerDepth = 1.0 / (distanceFog.fogFar - distanceFog.fogNear);
		float linerPos = length(cameraPos - input.worldpos.xyz) * linerDepth;
		float fogFactor = saturate((distanceFog.fogEnd - linerPos) / (distanceFog.fogEnd - distanceFog.fogStart));

		output.target0 = lerp(fogColor, shadercolor * texcolor, fogFactor);
		output.target1 = float4(1 - output.target0.rgb, 1);
		return output;
	}

	output.target0 = shadercolor * texcolor * materialColor;
	output.target1 = float4(1 - output.target0.rgb, 1);
	return output;
}