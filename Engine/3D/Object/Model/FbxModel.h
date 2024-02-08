#pragma once
#define NOMINMAX
#include "IModel.h"
#include "Quaternion.h"
#include "AnimationStruct.h"

#pragma region Assimp
struct aiMesh;
struct aiMaterial;
struct aiNode;
struct aiScene;
#pragma endregion

namespace MNE
{

	class FbxModel :public IModel
	{
	private:
		std::map<std::string, AnimationData> animations_;
		//	nodedata
		std::map<std::string, Node, std::less<>> nodes_;
		std::string rootNodeName_;

	protected:
		void LoadModel(const std::string& modelname, bool smoothing) override;

	private:
		void LoadAnimation(const aiScene* modelScene);
		void LoadNodeHeirarchy(const aiNode* pNode);
		void LoadNodes(const aiNode* rootNode);
		void LoadMaterial(Mesh* dst, const std::string& name);
		void LoadMesh(Mesh& dst, const aiMesh* src);
		void LoadBone(size_t meshIndex, const aiMesh* src);
		void SetTextureFilePath(const std::string& filename, Mesh& dst, const aiMaterial* src);

		void ReadNodeHeirarchy(const AnimationData& animData, float AnimationTime, const MyMath::Matrix& ParentTransform, const std::string& nodeName);

	public:
		FbxModel(const char* filename, bool smoothing = false);
		~FbxModel() override {};

		void ImGuiUpdate() override;

		void BoneTransform(float timer, std::vector<MyMath::Matrix>& transforms, const std::string& animeName, bool isLoop) override;
	};

}
