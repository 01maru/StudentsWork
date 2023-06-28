#include "ShadowRecieverHeader.hlsli"

Texture2D<float4> g_albedo : register(t0);
Texture2D<float4> g_shadowMap : register(t1);  // シャドウマップ
sampler g_sampler : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
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

    //  textureのカラー
    float4 texcolor = g_albedo.Sample(g_sampler, input.uv);

    // ライトビュースクリーン空間からUV空間に座標変換しシャドウマップのUV計算
    float2 shadowMapUV = input.posInLVP.xy / input.posInLVP.w;
    shadowMapUV *= float2(0.5f, -0.5f);
    shadowMapUV += 0.5f;

    // ライトビュースクリーン空間でのZ値
    float zInLVP = input.posInLVP.z;


    if (shadowMapUV.x > 0.0f && shadowMapUV.x < 1.0f
        && shadowMapUV.y > 0.0f && shadowMapUV.y < 1.0f)
    {
        //  shadowMapの値出力
        float2 shadowValue = g_shadowMap.Sample(g_sampler, shadowMapUV).xy;

        // このピクセルが遮蔽されているか(Lposとworldposの距離が1000以下)
        if (zInLVP - 0.0005f > shadowValue.x && zInLVP <= 1.0f)
        {
            float depth_sq = shadowValue.x * shadowValue.x;

            // 分散が大きいほど、varianceの数値は大きくなる
            float variance = min(max(shadowValue.y - depth_sq, 0.0001f), 1.0f);

            // このピクセルのライトから見た深度値とシャドウマップの平均の深度値の差を求める
            float md = zInLVP - shadowValue.x;

            // 光が届く確率を求める
            float lit_factor = variance / (variance + md * md);

            // シャドウカラーを求める
            float3 shadowColor = texcolor.xyz * 0.5f;

            // 光が当たる確率を使って通常カラーとシャドウカラーを線形補完
            texcolor.xyz = lerp(shadowColor, texcolor.xyz, 0.04);
        }
    }

    //	フォグ
    if (distanceFog.active) {
        float4 fogColor = float4(distanceFog.fogColor, 1.0f);		//フォグカラー
        const float linerDepth = 1.0 / (distanceFog.fogFar - distanceFog.fogNear);
        float linerPos = length(cameraPos - input.worldpos.xyz) * linerDepth;
        float fogFactor = saturate((distanceFog.fogEnd - linerPos) / (distanceFog.fogEnd - distanceFog.fogStart));

        return lerp(fogColor, shadercolor * texcolor, fogFactor);
    }

    return shadercolor * texcolor;
}