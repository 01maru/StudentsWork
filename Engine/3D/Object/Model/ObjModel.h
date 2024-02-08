#pragma once
#include "IModel.h"

namespace MNE
{

	class ObjModel :public IModel
	{
	protected:
		void LoadModel(const std::string& modelname, bool smoothing) override;

	private:
		void LoadMaterial(const std::string& directoryPath, const std::string& filename);

		void AddIndices(const std::vector<uint16_t>& indices, Mesh* mesh);
	public:
		ObjModel(const char* filename, bool smoothing = false);
		~ObjModel() override;
		void ImGuiUpdate() override {};
		void BoneTransform(float /*timer*/, std::vector<MyMath::Matrix>& /*transforms*/, const std::string& /*animeName*/, bool /*isLoop*/) override {};
	};

}
