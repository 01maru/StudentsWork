#pragma once
#include "Object3D.h"

namespace MNE
{

	class Object3DSilhouette :public Object3D
	{
	private:
		static GPipeline* sPipeline_;

	public:
		void Initialize() override;
		void Draw() override;
	};

}
