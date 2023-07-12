#pragma once
#include "Object3D.h"

class Enemy :public Object3D
{
public:
	void Initialize(IModel* model);
	void Update();
};

