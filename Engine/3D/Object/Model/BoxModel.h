﻿#pragma once
#include "IModel.h"

class BoxModel :public IModel
{
protected:
	void LoadModel(const std::string& modelname, bool smoothing) override;

	void AddIndex(Mesh* mesh);
	void AddVertex(Mesh* mesh);
public:
	BoxModel(const char* filename, bool smoothing = false);
	~BoxModel() override;
	void BoneTransform(float TimeInSeconds, std::vector<Matrix>& transforms) override;
};

