cbuffer ConstBufferDataTransform:register(b0)
{
	matrix matview;
	matrix matworld;
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
	float3 fogCenterPos;
	float3 fogColor;
	uint active;
	float fogStart;
	float fogEnd;
	float fogNear;
	float fogFar;
};

cbuffer ConstBufferLightData:register(b1)
{
	float3 ambientColor;
	DirLight dirLights[DIRLIGHT_NUM];
	DistanceFog distanceFog;
};

static const int MAX_BONES = 32;

cbuffer skinning:register(b2)
{
	matrix matSkinning[MAX_BONES];
}

cbuffer CBuffColorMaterial:register(b3)
{
	float4 materialColor;
}

cbuffer ConstBufferDataMaterial:register(b4)
{
	float3 m_ambient : packoffset(c0);
	float3 m_diffuse : packoffset(c1);
	float3 m_specular : packoffset(c2);
	float m_alpha : packoffset(c2.w);
};

struct VSOutput
{
	//	セマンティック(SV_はSystem Value)
	float4 svpos : SV_POSITION;	//	座標情報
	float4 worldpos:	POSITION;
	float3 normal :	NORMAL;
	float2 uv :TEXCOORD;
	float depth : TEXCOORD1;
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