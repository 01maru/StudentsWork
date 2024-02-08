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

namespace MyMath
{
	class Quaternion;
	class Vector3D;
}

namespace MNE
{

	struct Key;
	struct RotKey;
	struct Node;
	struct KeyChannels;
	struct AnimationData;

	namespace Util {

		//	転置して変換
		void TransformMatToAiMat(MyMath::Matrix& mat, const aiMatrix4x4& aiMat);

		const KeyChannels* FindNodeChannel(const AnimationData& animData, const std::string& name);

		size_t FindRotation(float AnimationTime, const KeyChannels* pNodeAnim);
		size_t FindScaling(float AnimationTime, const KeyChannels* pNodeAnim);
		size_t FindPosition(float AnimationTime, const KeyChannels* pNodeAnim);

		void CalcInterpolatedRotation(MyMath::Quaternion& Out, float AnimationTime, const KeyChannels* pNodeAnim);
		void CalcInterpolatedScaling(MyMath::Vector3D& Out, float AnimationTime, const KeyChannels* pNodeAnim);
		void CalcInterpolatedPosition(MyMath::Vector3D& Out, float AnimationTime, const KeyChannels* pNodeAnim);
	}

}
