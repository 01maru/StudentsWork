#include  "BlurHeader.hlsli"
Texture2D<float4> sceneTexture : register(t0); // �V�[���e�N�X�`��
sampler Sampler : register(s0);
float4 main(VSOutput input) : SV_TARGET
{
    float4 Color;

    // ��e�N�Z������v���X������8�e�N�Z���A�d�ݕt���ŃT���v�����O
    Color = weight[0].x * sceneTexture.Sample(Sampler, input.tex0.xy);
    Color += weight[0].y * sceneTexture.Sample(Sampler, input.tex1.xy);
    Color += weight[0].z * sceneTexture.Sample(Sampler, input.tex2.xy);
    Color += weight[0].w * sceneTexture.Sample(Sampler, input.tex3.xy);
    Color += weight[1].x * sceneTexture.Sample(Sampler, input.tex4.xy);
    Color += weight[1].y * sceneTexture.Sample(Sampler, input.tex5.xy);
    Color += weight[1].z * sceneTexture.Sample(Sampler, input.tex6.xy);
    Color += weight[1].w * sceneTexture.Sample(Sampler, input.tex7.xy);
    
    // ��e�N�Z���Ƀ}�C�i�X������8�e�N�Z���A�d�ݕt���ŃT���v�����O
    Color += weight[0].x * sceneTexture.Sample(Sampler, input.tex0.zw);
    Color += weight[0].y * sceneTexture.Sample(Sampler, input.tex1.zw);
    Color += weight[0].z * sceneTexture.Sample(Sampler, input.tex2.zw);
    Color += weight[0].w * sceneTexture.Sample(Sampler, input.tex3.zw);
    Color += weight[1].x * sceneTexture.Sample(Sampler, input.tex4.zw);
    Color += weight[1].y * sceneTexture.Sample(Sampler, input.tex5.zw);
    Color += weight[1].z * sceneTexture.Sample(Sampler, input.tex6.zw);
    Color += weight[1].w * sceneTexture.Sample(Sampler, input.tex7.zw);
    
    return float4(Color.xyz, 1.0f);
}