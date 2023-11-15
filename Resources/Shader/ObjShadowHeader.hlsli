static const int MAX_BONES = 32;

cbuffer ConstBufferDataTransform:register(b0)
{
	matrix matview;
	matrix matworld;
	float3 cameraPos;
};

cbuffer CBuffLight:register(b1)
{
	matrix mLVP;
	float3 lightPos;
}

cbuffer ConstBufferDataMaterial:register(b2)
{
	float3 m_ambient : packoffset(c0);
	float3 m_diffuse : packoffset(c1);
	float3 m_specular : packoffset(c2);
	float m_alpha : packoffset(c2.w);
};

//cbuffer skinning:register(b3)
//{
//	matrix matSkinning[MAX_BONES];
//}

struct VSOutput
{
	//	セマンティック(SV_はSystem Value)
	float4 svpos : SV_POSITION;	//	座標情報
	float2 depth : TEXCOORD1;   // ライト空間での座標
};