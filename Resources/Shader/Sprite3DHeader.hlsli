cbuffer CBuffColorMaterial:register(b0)
{
	float4 materialColor;
	uint bloomActive;
};

cbuffer ConstBufferDataTransform:register(b1)
{
	matrix matview;
	matrix matworld;
	float3 cameraPos;
};

struct VSOutput
{
	//	セマンティック(SV_はSystem Value)
	float4 svpos : SV_POSITION;	//	座標情報
	float2 uv :TEXCOORD;
};