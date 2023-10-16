#pragma once
#include <string>
#include "Matrix.h"

/**
* @file ConvertAiStruct.h
* @brief assimp用の変換関数をまとめたファイル
*/

typedef float ai_real;

template<typename T>
class aiMatrix4x4t;
typedef aiMatrix4x4t<ai_real> aiMatrix4x4;

struct Key;
struct RotKey;
struct KeyChannels;
struct AnimationData;
struct Node;
class Quaternion;
class Vector3D;

namespace Util {

	//	転置して変換
	void TransformMatToAiMat(Matrix& mat, const aiMatrix4x4& aiMat);

	const KeyChannels* FindNodeChannel(const AnimationData& animData, const std::string& name);

	size_t FindRotation(float AnimationTime, const KeyChannels* pNodeAnim);
	size_t FindScaling(float AnimationTime, const KeyChannels* pNodeAnim);
	size_t FindPosition(float AnimationTime, const KeyChannels* pNodeAnim);

	void CalcInterpolatedRotation(Quaternion& Out, float AnimationTime, const KeyChannels* pNodeAnim);
	void CalcInterpolatedScaling(Vector3D& Out, float AnimationTime, const KeyChannels* pNodeAnim);
	void CalcInterpolatedPosition(Vector3D& Out, float AnimationTime, const KeyChannels* pNodeAnim);
}

