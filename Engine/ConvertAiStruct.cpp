#include "ConvertAiStruct.h"
#include <assimp/anim.h>
#include <cassert>
#include "AnimationStruct.h"

using namespace MyMath;

void MNE::Util::TransformMatToAiMat(MyMath::Matrix& mat, const aiMatrix4x4& aiMat)
{
	mat.m[0][0] = aiMat.a1;
	mat.m[1][0] = aiMat.a2;
	mat.m[2][0] = aiMat.a3;
	mat.m[3][0] = aiMat.a4;

	mat.m[0][1] = aiMat.b1;
	mat.m[1][1] = aiMat.b2;
	mat.m[2][1] = aiMat.b3;
	mat.m[3][1] = aiMat.b4;

	mat.m[0][2] = aiMat.c1;
	mat.m[1][2] = aiMat.c2;
	mat.m[2][2] = aiMat.c3;
	mat.m[3][2] = aiMat.c4;

	mat.m[0][3] = aiMat.d1;
	mat.m[1][3] = aiMat.d2;
	mat.m[2][3] = aiMat.d3;
	mat.m[3][3] = aiMat.d4;
}

const MNE::KeyChannels* MNE::Util::FindNodeChannel(const AnimationData& animData, const std::string& name)
{
	for (size_t i = 0; i < animData.channels.size(); i++) {
		if (animData.channels[i].nodeName == name) {
			return &animData.channels[i];
		}
	}

	return nullptr;
}

size_t MNE::Util::FindRotation(float AnimationTime, const KeyChannels* pNodeAnim)
{
	size_t keysSize = pNodeAnim->rotationKeys.size();
	assert(keysSize > 0);

	for (size_t i = 0; i < keysSize - 1; i++) {
		if (AnimationTime < (float)pNodeAnim->rotationKeys[i + 1].time) {
			return i;
		}
	}

	assert(0);
	return 0;
}

size_t MNE::Util::FindScaling(float AnimationTime, const KeyChannels* pNodeAnim)
{
	size_t keysSize = pNodeAnim->scalingKeys.size();
	assert(keysSize > 0);

	for (size_t i = 0; i < keysSize - 1; i++) {
		if (AnimationTime < (float)pNodeAnim->scalingKeys[i + 1].time) {
			return i;
		}
	}

	assert(0);
	return 0;
}

size_t MNE::Util::FindPosition(float AnimationTime, const KeyChannels* pNodeAnim)
{
	size_t keysSize = pNodeAnim->positionKeys.size();
	assert(keysSize > 0);

	for (size_t i = 0; i < keysSize - 1; i++) {
		if (AnimationTime < (float)pNodeAnim->positionKeys[i + 1].time) {
			return i;
		}
	}

	assert(0);

	return 0;
}

void MNE::Util::CalcInterpolatedRotation(MyMath::Quaternion& Out, float AnimationTime, const KeyChannels* pNodeAnim)
{
	// 補間には最低でも２つの値が必要
	if (pNodeAnim->rotationKeys.size() == 1) {
		Out = pNodeAnim->rotationKeys[0].value;
		return;
	}

	size_t RotationIndex = FindRotation(AnimationTime, pNodeAnim);
	size_t NextRotationIndex = (RotationIndex + 1);
	assert(NextRotationIndex < pNodeAnim->rotationKeys.size());
	float DeltaTime = (float)(pNodeAnim->rotationKeys[NextRotationIndex].time - pNodeAnim->rotationKeys[RotationIndex].time);
	float Factor = (AnimationTime - (float)pNodeAnim->rotationKeys[RotationIndex].time) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiQuaternion& StartRotationQ = aiQuaternion(pNodeAnim->rotationKeys[RotationIndex].value.w,
		pNodeAnim->rotationKeys[RotationIndex].value.x,
		pNodeAnim->rotationKeys[RotationIndex].value.y,
		pNodeAnim->rotationKeys[RotationIndex].value.z);
	const aiQuaternion& EndRotationQ = aiQuaternion(pNodeAnim->rotationKeys[NextRotationIndex].value.w,
		pNodeAnim->rotationKeys[NextRotationIndex].value.x,
		pNodeAnim->rotationKeys[NextRotationIndex].value.y,
		pNodeAnim->rotationKeys[NextRotationIndex].value.z);
	aiQuaternion ans;
	aiQuaternion::Interpolate(ans, StartRotationQ, EndRotationQ, Factor);
	ans = ans.Normalize();

	Out.x = ans.x;
	Out.y = ans.y;
	Out.z = ans.z;
	Out.w = ans.w;
}

void MNE::Util::CalcInterpolatedScaling(MyMath::Vector3D& Out, float AnimationTime, const KeyChannels* pNodeAnim)
{
	if (pNodeAnim->scalingKeys.size() == 1) {
		Out = pNodeAnim->scalingKeys[0].value;
		return;
	}

	size_t ScalingIndex = FindScaling(AnimationTime, pNodeAnim);
	size_t NextScalingIndex = (ScalingIndex + 1);
	assert(NextScalingIndex < pNodeAnim->scalingKeys.size());
	float DeltaTime = (float)(pNodeAnim->scalingKeys[NextScalingIndex].time - pNodeAnim->scalingKeys[ScalingIndex].time);
	float Factor = (AnimationTime - (float)pNodeAnim->scalingKeys[ScalingIndex].time) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const Vector3D& Start = pNodeAnim->scalingKeys[ScalingIndex].value;
	const Vector3D& End = pNodeAnim->scalingKeys[NextScalingIndex].value;
	Vector3D Delta = End - Start;
	Out = Start + Factor * Delta;
}

void MNE::Util::CalcInterpolatedPosition(Vector3D& Out, float AnimationTime, const KeyChannels* pNodeAnim)
{
	if (pNodeAnim->positionKeys.size() == 1) {
		Out = pNodeAnim->positionKeys[0].value;
		return;
	}

	size_t PositionIndex = FindPosition(AnimationTime, pNodeAnim);
	size_t NextPositionIndex = (PositionIndex + 1);
	assert(NextPositionIndex < pNodeAnim->positionKeys.size());
	float DeltaTime = (float)(pNodeAnim->positionKeys[NextPositionIndex].time - pNodeAnim->positionKeys[PositionIndex].time);
	float Factor = (AnimationTime - (float)pNodeAnim->positionKeys[PositionIndex].time) / DeltaTime;
	assert(Factor >= 0.0f && Factor <= 1.0f);
	const Vector3D& Start = pNodeAnim->positionKeys[PositionIndex].value;
	const Vector3D& End = pNodeAnim->positionKeys[NextPositionIndex].value;
	Vector3D Delta = End - Start;
	Out = Start + Factor * Delta;
}
