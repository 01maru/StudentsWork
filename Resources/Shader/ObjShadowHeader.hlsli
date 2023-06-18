cbuffer ConstBufferDataTransform:register(b1)
{
	matrix matview;
	matrix matworld;
	float3 cameraPos;
};

cbuffer CBuffLight:register(b2)
{
	matrix mLVP;
	float3 lightPos;
}

//cbuffer skinning:register(b2)
//{
//	matrix matSkinning[MAX_BONES];
//}

struct VSOutput
{
	//	�Z�}���e�B�b�N(SV_��System Value)
	float4 svpos : SV_POSITION;	//	���W���
	float2 depth : TEXCOORD1;   // ���C�g��Ԃł̍��W
};