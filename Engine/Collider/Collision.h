#pragma once
#include "CollisionPrimitive.h"

/**
* @file Collision.h
* @brief 当たり判定の関数をまとめたのファイル
*/

namespace MNE
{

	class Collision
	{
	public:
		/**
		* @fn ClosestPtPoint2Triangle(const MyMath::Vector3D&, const Triangle&, MyMath::Vector3D*)
		* ある点に一番近い三角形の辺上の点を計算する関数
		* @param point 点の位置
		* @param triangle 三角形の情報
		* @param closest 一番近い三角形の辺上の点
		*/
		static void ClosestPtPoint2Triangle(const MyMath::Vector3D& point, const Triangle& triangle,
			MyMath::Vector3D* closest);

#pragma region Sphere

		/**
		* @fn CheckSphere2Sphere(const Sphere&, const Sphere&, MyMath::Vector3D*, MyMath::Vector3D*)
		* 球同士の当たり判定を計算する関数
		* @param sphereA 一つ目の球
		* @param sphereB 二つ目のの球
		* @param inter 中間地点
		* @param reject 押し出すベクトル
		* @return 当たっているかどうか
		*/
		static bool CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB,
			MyMath::Vector3D* inter = nullptr, MyMath::Vector3D* reject = nullptr);
		/**
		* @fn CheckSphere2Plane(const Sphere&, const Plane&, MyMath::Vector3D*)
		* 球と面の当たり判定を計算する関数
		* @param sphere 球
		* @param plane 面
		* @param inter 中間地点
		* @return 当たっているかどうか
		*/
		static bool CheckSphere2Plane(const Sphere& sphere, const Plane& plane,
			MyMath::Vector3D* inter = nullptr);
		/**
		* @fn CheckSphere2Triangle(const Sphere&, const Triangle&, MyMath::Vector3D*, MyMath::Vector3D*)
		* 球と面の当たり判定を計算する関数
		* @param sphere 球
		* @param triangle 面
		* @param inter 中間地点
		* @param reject 押し出すベクトル
		* @return 当たっているかどうか
		*/
		static bool CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle,
			MyMath::Vector3D* inter = nullptr, MyMath::Vector3D* reject = nullptr);

#pragma endregion

#pragma region Ray

		/**
		* @fn CheckRay2Plane(const Ray&, const Plane&, float*, MyMath::Vector3D*)
		* 面とレイの当たり判定を計算する関数
		* @param ray レイ
		* @param plane 面
		* @param distance 当たった位置までの距離
		* @param inter 当たった位置
		* @return 当たっているかどうか
		*/
		static bool CheckRay2Plane(const Ray& ray, const Plane& plane, float* distance = nullptr, MyMath::Vector3D* inter = nullptr);
		/**
		* @fn CheckRay2Sphere(const Ray&, const Sphere&, float*, MyMath::Vector3D*)
		* 球とレイの当たり判定を計算する関数
		* @param ray レイ
		* @param sphere 球
		* @param distance 当たった位置までの距離
		* @param inter 当たった位置
		* @return 当たっているかどうか
		*/
		static bool CheckRay2Sphere(const Ray& ray, const Sphere& sphere,
			float* distance = nullptr, MyMath::Vector3D* inter = nullptr);
		/**
		* @fn CheckRay2Triangle(const Ray&, const Triangle&, float*, MyMath::Vector3D*)
		* 球とレイの当たり判定を計算する関数
		* @param ray レイ
		* @param triangle 三角形
		* @param distance 当たった位置までの距離
		* @param inter 当たった位置
		* @return 当たっているかどうか
		*/
		static bool CheckRay2Triangle(const Ray& ray, const Triangle& triangle,
			float* distance = nullptr, MyMath::Vector3D* inter = nullptr);

#pragma endregion

	};

}
