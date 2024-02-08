#pragma once
#include "Quaternion.h"
#include "Vector3D.h"
#include "Matrix.h"
#include <vector>
#include <string>

namespace MNE
{

	struct Key
	{
		float time;
		MyMath::Vector3D value;
	};

	struct RotKey
	{
		float time;
		MyMath::Quaternion value;
	};

	struct KeyChannels
	{
		//	対応するノードの名前
		std::string nodeName;

		std::vector<Key> positionKeys;
		std::vector<Key> scalingKeys;
		std::vector<RotKey> rotationKeys;
	};

	struct AnimationData
	{
		//	frame/1sec
		float ticksPerSecond;
		//	アニメーション1Loopのframe数
		float duration;

		std::vector<KeyChannels> channels;
	};

	struct Node
	{
		MyMath::Matrix transformation;
		std::vector<std::string> childrenName;
	};

}
