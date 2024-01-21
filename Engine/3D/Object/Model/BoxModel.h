#pragma once
#include "IModel.h"

namespace MNE
{

	class BoxModel :public IModel
	{
	protected:
		void LoadModel(const std::string& modelname, bool smoothing) override;

		void AddIndex(Mesh* mesh);
		void AddVertex(Mesh* mesh);
	public:
		BoxModel(const char* filename, bool smoothing = false);
		~BoxModel() override;
		void ImGuiUpdate() override {};
		void BoneTransform(float /*timer*/, std::vector<Matrix>& /*transforms*/, const std::string& /*animeName*/, bool /*isLoop*/) override {};
	};

}
