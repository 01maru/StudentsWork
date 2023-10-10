cbuffer CBuffMaterial:register(b0)
{
	float4 color;	//	RGBA
};

cbuffer CBuffTransform:register(b1)
{
	matrix mat;
};

cbuffer CBuffDissolve:register(b2)
{
	float4 dissolveColor;
	float dissolveValue;
};

struct VSOutput
{
	//	�Z�}���e�B�b�N(SV_��System Value)
	float4 svpos : SV_POSITION;	//	���W���
	float2 uv :TEXCOORD;
};