#pragma once
#define NOMINMAX
#include "IModel.h"
#include "Quaternion.h"
#include <assimp/Importer.hpp>

#pragma region Assimp
struct aiMesh;
struct aiMaterial;
struct aiNode;
struct aiScene;
#pragma endregion

struct Key
{
	float time;
	Vector3D value;
};

struct RotKey
{
	float time;
	Quaternion value;
};

struct KeyChannels
{
	//	�Ή�����m�[�h�̖��O
	std::string nodeName;

	std::vector<Key> positionKeys;
	std::vector<Key> scalingKeys;
	std::vector<RotKey> rotationKeys;
};

struct AnimationData
{
	//	frame/1sec
	float ticksPerSecond;
	//	�A�j���[�V����1Loop��frame��
	float duration;

	std::vector<KeyChannels> channels;
};

class FbxModel :public IModel
{
private:
	//	fbx�̏��擾�p�ϐ�
	const aiScene* modelScene;
	Assimp::Importer importer;

	std::vector<AnimationData> animations_;

protected:
	void LoadModel(const std::string& modelname, bool smoothing) override;

private:
	void LoadAnimation();
	void LoadMaterial(Mesh* dst, const aiMaterial* src, size_t index);
	void LoadMesh(Mesh& dst, const aiMesh* src);
	void LoadBone(size_t meshIndex, const aiMesh* src);
	void SetTextureFilePath(const std::string& filename, Mesh& dst, const aiMaterial* src);

	void ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const Matrix& ParentTransform);

public:
	FbxModel(const char* filename, bool smoothing = false);
	~FbxModel() override;
	void BoneTransform(float TimeInSeconds, std::vector<Matrix>& transforms) override;
};

