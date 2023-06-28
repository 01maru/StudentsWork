#include "ShadowRecieverHeader.hlsli"

SkinOutput ComputeSkin(float4 pos : POSITION, float3 normal : NORMAL, uint4 boneIndices : BONEINDICES, float4 boneWeights : BONEWEIGHTS)
{
	SkinOutput output = (SkinOutput)0;

	uint iBone;
	float weight;
	matrix m;

	iBone = boneIndices.x;
	weight = boneWeights.x;
	m = matSkinning[iBone];
	output.pos += mul(m, pos) * weight;
	output.normal += mul((float3x3)m, normal) * weight;

	iBone = boneIndices.y;
	weight = boneWeights.y;
	m = matSkinning[iBone];
	output.pos += mul(m, pos) * weight;
	output.normal += mul((float3x3)m, normal) * weight;

	iBone = boneIndices.z;
	weight = boneWeights.z;
	m = matSkinning[iBone];
	output.pos += mul(m, pos) * weight;
	output.normal += mul((float3x3)m, normal) * weight;

	iBone = boneIndices.w;
	weight = boneWeights.w;
	m = matSkinning[iBone];
	output.pos += mul(m, pos) * weight;
	output.normal += mul((float3x3)m, normal) * weight;

	return output;
}

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD, uint4 boneIndices : BONEINDICES, float4 boneWeights : BONEWEIGHTS)
{
	SkinOutput skinned = ComputeSkin(pos, normal, boneIndices, boneWeights);

	float4 wnormal = normalize(mul(mWorld, float4(skinned.normal, 0)));
	VSOutput output;

	output.pos = mul(mul(mViewProj, mWorld), skinned.pos);
	output.worldpos = mul(mWorld, skinned.pos);
	output.normal = wnormal.xyz;
	output.uv = uv;

    output.posInLVP = mul(mLVP, output.worldpos);

    //  0~1000‚ð0~1.0‚É•ÏŠ·
    output.posInLVP.z = length(output.worldpos.xyz - lightPos) / 100.0f;
    return output;
}