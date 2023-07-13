#include "FbxModel.h"
#include "Quaternion.h"
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <cassert>
#include "LightManager.h"

#include "ConvertString.h"
#include "ConvertAiStruct.h"

using namespace std;

FbxModel::FbxModel(const char* filename, bool smoothing)
{
	Initialize(filename, smoothing);
}

FbxModel::~FbxModel()
{
	meshes_.clear();

	materials_.clear();
}

struct AnimationData
{
	float mTicksPerSecond;
	float mDuration;
	std::vector<std::string> nodeName;
};

void FbxModel::BoneTransform(float TimeInSeconds, std::vector<Matrix>& transforms)
{
	Matrix Identity;

	double TicksPerSecond = modelScene->mAnimations[0]->mTicksPerSecond != 0 ? modelScene->mAnimations[0]->mTicksPerSecond : 25.0f;
	float TimeInTicks = TimeInSeconds * (float)TicksPerSecond;
	float AnimationTime = (float)fmod(TimeInTicks, modelScene->mAnimations[0]->mDuration);

	ReadNodeHeirarchy(AnimationTime, modelScene->mRootNode, Identity);

	transforms.resize(numBones_);

	for (size_t i = 0; i < numBones_; i++) {
		transforms[i] = boneInfo_[i].finalTransformation;
	}
}

void FbxModel::LoadModel(const std::string& modelname, bool /*smoothing*/)
{
	//	�p�X�̐ݒ�
	const string filename = modelname + ".fbx";
	const string directoryPath = "Resources/Model/" + modelname + "/";

	//	�V�[���̃��[�h
	modelScene = importer.ReadFile(directoryPath + filename, aiProcess_Triangulate);	//	�O�p�ʉ�

	//	�ǂݍ��ݎ��s������
	if (modelScene == nullptr) { return; }

	//	GlobalInverseTransform�ݒ�
	Util::TransformMatToAiMat(globalInverseTransform_, modelScene->mRootNode->mTransformation);

	//	mesh���ݒ�
	meshes_.reserve(modelScene->mNumMeshes);
	for (size_t i = 0; i < modelScene->mNumMeshes; i++)
	{
		//	aiMesh�^�̏��擾
		const auto pMesh = modelScene->mMeshes[i];

		//	�^�ϊ�
		meshes_.emplace_back();
		Mesh* mesh = &meshes_.back();
		LoadMesh(*mesh, pMesh);
		//	�{�[���擾
		LoadBone(i, pMesh);

		//	material�擾
		aiMaterial* pMaterial = modelScene->mMaterials[i];

		LoadMaterial(mesh, pMaterial, i);

		SetTextureFilePath(directoryPath + filename, *mesh, pMaterial);
	}
}

void FbxModel::LoadMaterial(Mesh* dst, const aiMaterial* /*src*/, size_t index)
{
	Material* material = Material::Create();

	material->name_ += to_string(index);

	////	Diffuse
	//aiColor3D difcolor(0.f, 0.f, 0.f);
	//src->Get(AI_MATKEY_COLOR_DIFFUSE, difcolor);
	LightManager* light = LightManager::GetInstance();
	material->diffuse_ = light->GetMtlDiffuse();
	////	AMBIENT
	//aiColor3D amcolor(0.3f, 0.3f, 0.3f);
	//src->Get(AI_MATKEY_COLOR_AMBIENT, amcolor);
	material->ambient_ = light->GetMtlAmbient();
	////	SPECULAR
	//aiColor3D specolor(0.3f, 0.3f, 0.3f);
	//src->Get(AI_MATKEY_COLOR_SPECULAR, specolor);
	material->specular_ = light->GetMtlSpecular();

	if (material) {
		// �}�e���A����o�^
		AddMaterial(material);
	}

	dst->SetMaterial(material);
}

void FbxModel::LoadMesh(Mesh& dst, const aiMesh* src)
{
	aiVector3D zero3D(0.0f, 0.0f, 0.0f);

	//	���_����
	for (size_t i = 0; i < src->mNumVertices; i++)
	{
		auto position = &(src->mVertices[i]);
		auto normal = &(src->mNormals[i]);
		auto uv = (src->HasTextureCoords(0)) ? &(src->mTextureCoords[0][i]) : &zero3D;
		//	uv���]
		uv->y = 1 - uv->y;

		ModelVertex vertex_ = {};
		vertex_.pos = Vector3D(position->x, position->y, -position->z);
		vertex_.normal = Vector3D(normal->x, normal->y, normal->z);
		vertex_.uv = Vector2D(uv->x, uv->y);
		vertex_.boneIndex[0] = 31;				//	bone�ő勖�e��-1
		vertex_.boneWeight[0] = 1.0f;			//	�{�[���Ȃ��p

		dst.AddVertex(vertex_);
	}

	//	index�ݒ�
	for (size_t i = 0; i < src->mNumFaces; i++)
	{
		const auto& face = src->mFaces[i];

		dst.AddIndex((unsigned short)face.mIndices[0]);
		dst.AddIndex((unsigned short)face.mIndices[2]);
		dst.AddIndex((unsigned short)face.mIndices[1]);
	}
}

void FbxModel::LoadBone(size_t meshIndex, const aiMesh* src)
{
	//	bone���ݒ�
	for (size_t i = 0; i < src->mNumBones; i++) {
		uint16_t BoneIndex = 0;
		string BoneName(src->mBones[i]->mName.data);

		if (boneMapping_.find(BoneName) == boneMapping_.end()) {
			BoneIndex = (uint16_t)numBones_;
			numBones_++;
			BoneInfo bi;
			boneInfo_.push_back(bi);
			//	�^�ϊ�
			Util::TransformMatToAiMat(boneInfo_[BoneIndex].boneOffset, src->mBones[i]->mOffsetMatrix);
			boneMapping_[BoneName] = BoneIndex;
		}
		else {
			BoneIndex = (uint16_t)boneMapping_[BoneName];
		}

		for (size_t j = 0; j < src->mBones[i]->mNumWeights; j++) {
			size_t VertexID = src->mBones[i]->mWeights[j].mVertexId;
			float Weight = src->mBones[i]->mWeights[j].mWeight;
			meshes_[meshIndex].SetBone(VertexID, BoneIndex, Weight);
		}
	}
}

void FbxModel::SetTextureFilePath(const std::string& filename, Mesh& dst, const aiMaterial* src)
{
	aiString path;
	if (src->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), path) == AI_SUCCESS)
	{
		auto dir = Util::GetDirectoryPath(filename);
		auto file = std::string(path.C_Str());
		dst.SetTextureFilePath(dir + file);
		dst.GetMaterial()->name_ = file;
	}
}

void FbxModel::ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const Matrix& ParentTransform)
{
	string NodeName(pNode->mName.data);

	//	�����A�j���[�V��������Ȃ炱���ŕύX
	const aiAnimation* pAnimation = modelScene->mAnimations[0];

	Matrix NodeTransformation;
	Util::TransformMatToAiMat(NodeTransformation, pNode->mTransformation);

	const aiNodeAnim* pNodeAnim = Util::FindNodeAnim(pAnimation, NodeName);

	if (pNodeAnim) {
		// �X�P�[�����O���Ԃ��A�X�P�[�����O�ϊ��s��𐶐�����
		aiVector3D Scaling;
		Util::CalcInterpolatedScaling(Scaling, AnimationTime, pNodeAnim);
		MyMath::ObjMatrix mat;
		mat.scale_ = Vector3D(Scaling.x, Scaling.y, Scaling.z);
		mat.SetMatScaling();

		// ��]���Ԃ��A��]�ϊ��s��𐶐�����
		aiQuaternion RotationQ;
		Util::CalcInterpolatedRotation(RotationQ, AnimationTime, pNodeAnim);
		Quaternion rotQ(RotationQ.w, RotationQ.x, RotationQ.y, RotationQ.z);
		mat.matRot_ = rotQ.GetRotMatrix();

		// �ړ����Ԃ��A�ړ��ϊ��s��𐶐�����
		aiVector3D Translation;
		Util::CalcInterpolatedPosition(Translation, AnimationTime, pNodeAnim);
		mat.trans_ = Vector3D(Translation.x, Translation.y, Translation.z);
		mat.SetMatTransform();

		// ������L�̕ϊ�����������
		NodeTransformation = mat.matScale_;
		NodeTransformation *= mat.matRot_;
		NodeTransformation *= mat.matTrans_;
	}

	Matrix GlobalTransformation = NodeTransformation;
	GlobalTransformation *= ParentTransform;

	if (boneMapping_.find(NodeName) != boneMapping_.end()) {
		size_t BoneIndex = boneMapping_[NodeName];
		boneInfo_[BoneIndex].finalTransformation = globalInverseTransform_;
		boneInfo_[BoneIndex].finalTransformation *= boneInfo_[BoneIndex].boneOffset;
		boneInfo_[BoneIndex].finalTransformation *= GlobalTransformation;
	}

	for (size_t i = 0; i < pNode->mNumChildren; i++) {
		ReadNodeHeirarchy(AnimationTime, pNode->mChildren[i], GlobalTransformation);
	}
}
