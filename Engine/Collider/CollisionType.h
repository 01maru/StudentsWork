#pragma once
/**
* @file CollisionShapeType.h
* @brief 当たり判定の形をまとめたのファイル
*/

namespace MNE
{

	enum CollisionShapeType
	{
		SHAPE_UNKNOWN = -1,
		COLLISIONSHAPE_SPHERE,
		COLLISIONSHAPE_MESH,
		COLLISIONSHAPE_PLANE,
	};

}

