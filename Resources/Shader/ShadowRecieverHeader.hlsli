// ���C�g�r���[�v���W�F�N�V�����s��̒萔�o�b�t�@�[���`
cbuffer ShadowCb : register(b0)
{
    matrix mLVP;
    float3 lightPos;    // ���C�g�̍��W
};

// ���f���p�̒萔�o�b�t�@�[
cbuffer ModelCb : register(b1)
{
    matrix mViewProj;
    matrix mWorld;
	float3 cameraPos;
};

static const int DIRLIGHT_NUM = 1;

struct DirLight
{
	float3 lightv;
	float3 lightcolor;
	uint active;
};

struct DistanceFog
{
	float3 fogColor;
	uint active;
	float fogStart;
	float fogEnd;
	float fogNear;
	float fogFar;
};

cbuffer ConstBufferLightData : register(b2)
{
	float3 ambientColor;
	DirLight dirLights[DIRLIGHT_NUM];
	DistanceFog distanceFog;
};

static const int MAX_BONES = 32;

cbuffer skinning:register(b3)
{
	matrix matSkinning[MAX_BONES];
}

cbuffer CBuffColorMaterial:register(b4)
{
	float4 materialColor;
}

cbuffer ConstBufferDataMaterial:register(b5)
{
	float3 m_ambient : packoffset(c0);
	float3 m_diffuse : packoffset(c1);
	float3 m_specular : packoffset(c2);
	float m_alpha : packoffset(c2.w);
};


struct VSOutput
{
	//	�Z�}���e�B�b�N(SV_��System Value)
	float4 pos : SV_POSITION;	//	���W���
	float4 worldpos:	POSITION;
	float3 normal :	NORMAL;
	float2 uv :TEXCOORD;
    float4 posInLVP : TEXCOORD1;    // ���C�g�r���[�X�N���[����Ԃł̃s�N�Z���̍��W
	float depth : TEXCOORD2;
};

struct PSOutput
{
	float4 target0 : SV_TARGET0;
	float4 target1 : SV_TARGET1;
	//float4 target2 : SV_TARGET2;
};

struct SkinOutput
{
	float4 pos;
	float3 normal;
};