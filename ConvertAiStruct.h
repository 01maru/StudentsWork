#pragma once
#include <string>
#include "Matrix.h"

struct aiNodeAnim;
struct aiAnimation;

typedef float ai_real;

template<typename T>
class aiMatrix4x4t;
typedef aiMatrix4x4t<ai_real> aiMatrix4x4;

template<typename T>
class aiQuaterniont;
typedef aiQuaterniont<ai_real> aiQuaternion;

template<typename T>
class aiVector3t;
typedef aiVector3t<ai_real> aiVector3D;

namespace Util {

	//	転置して変換
	void TransformMatToAiMat(Matrix& mat, const aiMatrix4x4& aiMat);

	const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const std::string& name);
	size_t FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
	size_t FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
	size_t FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);

	void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
	void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
	void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
}

