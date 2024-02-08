#pragma once
#include "MyMath.h"

/**
* @file VertexStruct.h
* @brief 頂点バッファの構造体をまとめたファイル
*/

namespace MNE
{

	struct ParticleVertex
	{
		MyMath::Vector3D pos;
		float scale;
	};

	struct Vertex
	{
		MyMath::Vector3D pos;
		MyMath::Vector3D normal;
		MyMath::Vector2D uv;
	};

	struct ModelVertex
	{
		MyMath::Vector3D pos;
		MyMath::Vector3D normal;
		MyMath::Vector2D uv;
		unsigned int boneIndex[4];
		float boneWeight[4];
	};

	struct VertexObj
	{
		MyMath::Vector3D pos;
		MyMath::Vector3D normal;
		MyMath::Vector2D uv;
		MyMath::Vector4D color;
	};

	struct ScreenVertex
	{
		MyMath::Vector3D pos;
		MyMath::Vector2D uv;
	};

	struct SpriteVertex
	{
		MyMath::Vector2D pos;
		MyMath::Vector2D uv;
	};

}
