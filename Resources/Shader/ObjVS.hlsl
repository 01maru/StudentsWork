#include "ObjHeader.hlsli"

SkinOutput ComputeSkin(float4 pos : POSITION, float3 normal : NORMAL, uint4 boneIndices : BONEINDICES, float4 boneWeights : BONEWEIGHTS)
{
	SkinOutput output = (SkinOutput)0;

	uint iBone;
	float weight;
	matrix m;
	
	if(boneWeights.x!=0||boneWeights.y!=0||boneWeights.z!=0||boneWeights.w!=0)
	{
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
	}
	else
	{
		output.pos = pos;
		output.normal = normal;
	}

	return output;
}


VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD, uint4 boneIndices : BONEINDICES, float4 boneWeights : BONEWEIGHTS)
{
	SkinOutput skinned = ComputeSkin(pos, normal, boneIndices, boneWeights);

	float4 wnormal = normalize(mul(matworld, float4(skinned.normal, 0)));
	VSOutput output;

	output.svpos = mul(mul(matview, matworld), skinned.pos);
	output.worldpos = mul(matworld, skinned.pos);
	output.normal = wnormal.xyz;
	output.uv = uv;

	output.depth = length(output.worldpos.xyz - cameraPos) / 1000.0;

	return output;
}