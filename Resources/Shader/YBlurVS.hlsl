#include  "BlurHeader.hlsli"
Texture2D<float4> sceneTexture : register(t0); // �V�[���e�N�X�`��
sampler Sampler : register(s0);
VSOutput main(float4 pos : POSITION, float2 uv : TEXCOORD0)
{
    // step-14 �c�u���[�p�̒��_�V�F�[�_�[������
    VSOutput output;

    // ���W�ϊ�
    output.pos = pos;

    // �e�N�X�`���T�C�Y���擾
    float2 texSize;
    float level;
    sceneTexture.GetDimensions(0, texSize.x, texSize.y, level);

    // ��e�N�Z����UV���W���L�^
    float2 tex = uv;

    // ��e�N�Z������V���W��+1�e�N�Z�����炷���߂̃I�t�Z�b�g���v�Z����
    output.tex0.xy = float2(0.0f, 1.0f / texSize.y);

    // ��e�N�Z������V���W��+3�e�N�Z�����炷���߂̃I�t�Z�b�g���v�Z����
    output.tex1.xy = float2(0.0f, 3.0f / texSize.y);

    // ��e�N�Z������V���W��+5�e�N�Z�����炷���߂̃I�t�Z�b�g���v�Z����
    output.tex2.xy = float2(0.0f, 5.0f / texSize.y);

    // ��e�N�Z������V���W��+7�e�N�Z�����炷���߂̃I�t�Z�b�g���v�Z����
    output.tex3.xy = float2(0.0f, 7.0f / texSize.y);

    // ��e�N�Z������V���W��+9�e�N�Z�����炷���߂̃I�t�Z�b�g���v�Z����
    output.tex4.xy = float2(0.0f, 9.0f / texSize.y);

    // ��e�N�Z������V���W��+11�e�N�Z�����炷���߂̃I�t�Z�b�g���v�Z����
    output.tex5.xy = float2(0.0f, 11.0f / texSize.y);

    // ��e�N�Z������V���W��+13�e�N�Z�����炷���߂̃I�t�Z�b�g���v�Z����
    output.tex6.xy = float2(0.0f, 13.0f / texSize.y);

    // ��e�N�Z������V���W��+15�e�N�Z�����炷���߂̃I�t�Z�b�g���v�Z����
    output.tex7.xy = float2(0.0f, 15.0f / texSize.y);

    // �I�t�Z�b�g��-1���|���ă}�C�i�X�����̃I�t�Z�b�g���v�Z����
    output.tex0.zw = output.tex0.xy * -1.0f;
    output.tex1.zw = output.tex1.xy * -1.0f;
    output.tex2.zw = output.tex2.xy * -1.0f;
    output.tex3.zw = output.tex3.xy * -1.0f;
    output.tex4.zw = output.tex4.xy * -1.0f;
    output.tex5.zw = output.tex5.xy * -1.0f;
    output.tex6.zw = output.tex6.xy * -1.0f;
    output.tex7.zw = output.tex7.xy * -1.0f;

    // �I�t�Z�b�g�Ɋ�e�N�Z����UV���W�𑫂��Z���āA
    // ���ۂɃT���v�����O����UV���W�ɕϊ�����
    output.tex0 += float4(tex, tex);
    output.tex1 += float4(tex, tex);
    output.tex2 += float4(tex, tex);
    output.tex3 += float4(tex, tex);
    output.tex4 += float4(tex, tex);
    output.tex5 += float4(tex, tex);
    output.tex6 += float4(tex, tex);
    output.tex7 += float4(tex, tex);
    return output;
}