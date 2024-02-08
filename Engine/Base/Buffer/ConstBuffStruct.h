#pragma once
#include "Vector2D.h"
#include "Vector3D.h"
#include "Vector4D.h"
#include "Matrix.h"

/**
* @file ConstBuffStruct.h
* @brief 定数バッファの構造体をまとめたファイル
*/

namespace MNE
{

	namespace CBuff
	{
		const int MAX_BONES = 32;

		struct CBuffSpriteTransform {
			MyMath::Matrix mat;
		};

		struct CBufferParticleTransform {
			MyMath::Matrix mat;
			MyMath::Matrix matBillboard;
			MyMath::Vector3D cameraPos;
			float scale;
		};

		struct CBuffColorMaterial {
			MyMath::Vector4D color;	//	RGBA
		};

		struct CBuffDissolveData {
			MyMath::Vector4D color;
			float value;
		};

		struct CBuffGlayScale {
			bool active;
		};

		struct CBuffObj3DTransform {
			MyMath::Matrix matViewProj;
			MyMath::Matrix matWorld;
			MyMath::Vector3D cameraPos;
		};

		struct CBuffLightMaterial {
			MyMath::Matrix mLVP;
			MyMath::Vector3D cameraPos;
		};

		struct CBuffSkinData
		{
			MyMath::Matrix bones[MAX_BONES];
		};

		struct CBufferBlurWeight {
			MyMath::Vector4D weight[2];
		};

		struct CBufferMaterialData {
			MyMath::Vector3D ambient;
			float pad1;
			MyMath::Vector3D diffuse;
			float pad2;
			MyMath::Vector3D specular;
			float alpha;
		};

	#pragma region Light
		const size_t DIRLIGHT_NUM = 1;

		struct CBuffPointLight
		{
			MyMath::Vector3D pos;
			float pad1;
			MyMath::Vector3D color;
			float pad2;
			MyMath::Vector3D lightatten;
			bool active;
		};

		struct CBuffDirLightData
		{
			MyMath::Vector3D lightv;
			float pad1;
			MyMath::Vector3D lightcolor;
			bool active;
		};

		struct CBuffDisFogData
		{
			MyMath::Vector3D centerPos;
			float pad1;
			MyMath::Vector3D color;
			bool active;
			float start;
			float end;
			float fogNear;
			float fogFar;
		};

		struct CBuffSpotLightData
		{
			MyMath::Vector3D lightv;
			float pad1;
			MyMath::Vector3D lightpos;
			float pad2;
			MyMath::Vector3D lightcolor;
			float pad3;
			MyMath::Vector3D lightatten;
			float pad4;
			MyMath::Vector2D lightfactoranglecos;
			bool active;
			float pad5;
		};

		struct CBuffCircleShadowData
		{
			MyMath::Vector3D dir;
			float pad1;
			MyMath::Vector3D centerPos;
			float distanceCasterLight;
			MyMath::Vector3D atten;
			float pad2;
			MyMath::Vector2D factorAngleCos;
			bool active;
			float pad3;
		};

		struct CBuffLightData
		{
			MyMath::Vector3D ambientColor;
			float pad1;
			CBuff::CBuffDirLightData dirLights[DIRLIGHT_NUM];
			CBuff::CBuffDisFogData distanceFog;
		};

	#pragma endregion
	}

}
