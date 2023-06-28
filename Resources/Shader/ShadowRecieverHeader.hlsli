static const int DIRLIGHT_NUM = 1;

static const int MAX_BONES = 32;

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

cbuffer ConstBufferDataMaterial:register(b0)
{
	float3 m_ambient : packoffset(c0);
	float3 m_diffuse : packoffset(c1);
	float3 m_specular : packoffset(c2);
	float m_alpha : packoffset(c2.w);
};

// モデル用の定数バッファー
cbuffer ModelCb : register(b1)
{
    matrix mViewProj;
    matrix mWorld;
	float3 cameraPos;
};

// ライトビュープロジェクション行列の定数バッファーを定義
cbuffer ShadowCb : register(b2)
{
    matrix mLVP;
    float3 lightPos;    // ライトの座標
};

cbuffer skinning:register(b3)
{
	matrix matSkinning[MAX_BONES];
}

cbuffer ConstBufferLightData : register(b4)
{
	float3 ambientColor;
	DirLight dirLights[DIRLIGHT_NUM];
	DistanceFog distanceFog;
};

struct VSOutput
{
	//	セマンティック(SV_はSystem Value)
	float4 pos : SV_POSITION;	//	座標情報
	float4 worldpos:	POSITION;
	float3 normal :	NORMAL;
	float2 uv :TEXCOORD;
    float4 posInLVP : TEXCOORD1;    // ライトビュースクリーン空間でのピクセルの座標
};

struct SkinOutput
{
	float4 pos;
	float3 normal;
};