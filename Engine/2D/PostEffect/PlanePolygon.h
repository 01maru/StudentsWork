#pragma once
#include "VertIdxBuff.h"

class PlanePolygon :public VertIdxBuff
{
private:
	std::vector<ScreenVertex> vertices_;
	std::vector<uint16_t> indices_;

private:
	void SetVertices() override;

public:
	void Initialize();
	void DrawIndexedInstanced();
};

