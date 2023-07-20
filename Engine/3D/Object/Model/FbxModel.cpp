#include "FbxModel.h"
#include <cassert>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "LightManager.h"
#include "ConvertString.h"
#include "ConvertAiStruct.h"
#include "ImGuiManager.h"

using namespace std;

FbxModel::FbxModel(const char* filename, bool smoothing)
{
	Initialize(filename, smoothing);
}

void FbxModel::LoadModel(const std::string& modelname, bool smoothing)
{
	//	�p�X�̐ݒ�
	const string filename = modelname + ".fbx";
	const string directoryPath = "Resources/Model/" + modelname + "/";

	//	�V�[���̃��[�h
	Assimp::Importer importer;
	uint32_t aiProcess = aiProcess_Triangulate;
	if (smoothing) aiProcess |= aiProcess_GenSmoothNormals;

	const aiScene* modelScene = importer.ReadFile(directoryPath + filename, aiProcess);	//	�O�p�ʉ�

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

		LoadMaterial(mesh, pMaterial->GetName().C_Str());

		SetTextureFilePath(directoryPath + filename, *mesh, pMaterial);
	}

	LoadAnimation(modelScene);
	LoadNodes(modelScene->mRootNode);
}

void FbxModel::LoadNodeHeirarchy(const aiNode* pNode)
{
	Node node;

	//	NodeTransform
	Util::TransformMatToAiMat(node.transformation, pNode->mTransformation);

	//	ChildrenName
	for (size_t i = 0; i < pNode->mNumChildren; i++) {
		node.childrenName.emplace_back(pNode->mChildren[i]->mName.C_Str());
	}

	nodes_.emplace(pNode->mName.C_Str(), node);

	//	�ċA
	for (size_t i = 0; i < pNode->mNumChildren; i++) {
		LoadNodeHeirarchy(pNode->mChildren[i]);
	}
}

void FbxModel::LoadNodes(const aiNode* rootNode)
{
	rootNodeName_ = rootNode->mName.C_Str();

	LoadNodeHeirarchy(rootNode);
}

void FbxModel::LoadAnimation(const aiScene* modelScene)
{
	for (size_t i = 0; i < modelScene->mNumAnimations; i++)
	{
		AnimationData data;

		aiAnimation* animation = modelScene->mAnimations[i];

		data.ticksPerSecond = (float)animation->mTicksPerSecond;
		data.duration = (float)animation->mDuration;

		for (size_t channelIdx = 0; channelIdx < animation->mNumChannels; channelIdx++)
		{
			KeyChannels channel;
			channel.nodeName = animation->mChannels[channelIdx]->mNodeName.C_Str();

			for (size_t keyIdx = 0; keyIdx < animation->mChannels[channelIdx]->mNumPositionKeys; keyIdx++)
			{
				Key key;
				key.time = (float)animation->mChannels[channelIdx]->mPositionKeys[keyIdx].mTime;
				aiVector3D pos = animation->mChannels[channelIdx]->mPositionKeys[keyIdx].mValue;
				key.value = { pos.x,pos.y,pos.z };
				channel.positionKeys.emplace_back(key);
			}

			for (size_t scaleIdx = 0; scaleIdx < animation->mChannels[channelIdx]->mNumScalingKeys; scaleIdx++)
			{
				Key key;
				key.time = (float)animation->mChannels[channelIdx]->mScalingKeys[scaleIdx].mTime;
				aiVector3D scale = animation->mChannels[channelIdx]->mScalingKeys[scaleIdx].mValue;
				key.value = { scale.x,scale.y,scale.z };
				channel.scalingKeys.emplace_back(key);
			}

			for (size_t rotIdx = 0; rotIdx < animation->mChannels[channelIdx]->mNumRotationKeys; rotIdx++)
			{
				RotKey key;
				key.time = (float)animation->mChannels[channelIdx]->mRotationKeys[rotIdx].mTime;
				aiQuaternion rot = animation->mChannels[channelIdx]->mRotationKeys[rotIdx].mValue;
				key.value = { rot.w,rot.x,rot.y,rot.z };
				channel.rotationKeys.emplace_back(key);
			}

			data.channels.emplace_back(channel);
		}

		animations_.emplace_back(data);
	}
}

void FbxModel::LoadMaterial(Mesh* dst, const std::string& name)
{
	Material* material = Material::Create();

	material->name_ = name;

	//	Diffuse
	LightManager* light = LightManager::GetInstance();
	material->diffuse_ = light->GetMtlDiffuse();
	//	AMBIENT
	material->ambient_ = light->GetMtlAmbient();
	//	SPECULAR
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

void FbxModel::ImGuiUpdate()
{
	ImGuiManager* imgui = ImGuiManager::GetInstance();

	imgui->BeginWindow("FbxInfo");

	imgui->Text("Test");
	if (imgui->CollapsingHeader("Node")) {
		for (auto itr = nodes_.begin(); itr != nodes_.end(); ++itr)
		{
			if (imgui->TreeNode(itr->first)) {
				for (auto child = itr->second.childrenName.begin()
					; child != itr->second.childrenName.end(); ++child)
				{
					imgui->Text(child->c_str());
				}

				Matrix transformation = itr->second.transformation;
				imgui->Text("Transformaion\n(%.2f, %.2f, %.2f, %.2f,\n %.2f, %.2f, %.2f, %.2f,\n %.2f, %.2f, %.2f, %.2f,\n %.2f, %.2f, %.2f, %.2f)",
					transformation.m[0][0], transformation.m[0][1], transformation.m[0][2], transformation.m[0][3],
					transformation.m[1][0], transformation.m[1][1], transformation.m[1][2], transformation.m[1][3],
					transformation.m[2][0], transformation.m[2][1], transformation.m[2][2], transformation.m[2][3],
					transformation.m[3][0], transformation.m[3][1], transformation.m[3][2], transformation.m[3][3]);

				imgui->TreePop();
			}
		}
	}
	if (imgui->CollapsingHeader("BoneTransformation")) {

		for (auto itr = boneMapping_.begin(); itr != boneMapping_.end(); ++itr) {
			if (imgui->TreeNode(itr->first)) {
				Matrix transformation = boneInfo_[itr->second].finalTransformation;
				imgui->Text("Transformaion\n(%.2f, %.2f, %.2f, %.2f,\n %.2f, %.2f, %.2f, %.2f,\n %.2f, %.2f, %.2f, %.2f,\n %.2f, %.2f, %.2f, %.2f)",
					transformation.m[0][0], transformation.m[0][1], transformation.m[0][2], transformation.m[0][3],
					transformation.m[1][0], transformation.m[1][1], transformation.m[1][2], transformation.m[1][3],
					transformation.m[2][0], transformation.m[2][1], transformation.m[2][2], transformation.m[2][3],
					transformation.m[3][0], transformation.m[3][1], transformation.m[3][2], transformation.m[3][3]);

				imgui->TreePop();
			}
		}
	}

	imgui->EndWindow();
}

void FbxModel::BoneTransform(float TimeInSeconds, std::vector<Matrix>& transforms, int32_t animationIdx)
{
	Matrix Identity;

	if (animations_.size() <= animationIdx || animationIdx < 0) return;

	//double TicksPerSecond = animations_[index].ticksPerSecond != 0 ? animations_[index].ticksPerSecond : 25.0f;
	//float TimeInTicks = TimeInSeconds * (float)TicksPerSecond;	//	frame��
	float AnimationTime = (float)fmod(TimeInSeconds, animations_[animationIdx].duration);	//	���݂̃t���[����/1Loop�̃t���[�����@�̂��܂�

	ReadNodeHeirarchy(animations_[animationIdx], AnimationTime, Identity, rootNodeName_);

	transforms.resize(numBones_);

	for (size_t i = 0; i < numBones_; i++) {
		transforms[i] = boneInfo_[i].finalTransformation;
	}
}

void FbxModel::ReadNodeHeirarchy(const AnimationData& animData, float AnimationTime, const Matrix& ParentTransform, const std::string& nodeName)
{
	//	node�����邩
	if (nodes_.count(nodeName) == 0) assert(0);
	Matrix NodeTransformation = nodes_[nodeName].transformation;

	const KeyChannels* pNodeAnim = Util::FindNodeChannel(animData, nodeName);

	if (pNodeAnim) {
		MyMath::ObjMatrix mat;

		// �X�P�[�����O���Ԃ��A�X�P�[�����O�ϊ��s��𐶐�����
		Vector3D Scaling;
		Util::CalcInterpolatedScaling(Scaling, AnimationTime, pNodeAnim);
		mat.scale_ = Vector3D(Scaling.x, Scaling.y, Scaling.z);
		mat.SetMatScaling();

		// ��]���Ԃ��A��]�ϊ��s��𐶐�����
		Quaternion RotationQ;
		Util::CalcInterpolatedRotation(RotationQ, AnimationTime, pNodeAnim);
		Quaternion rotQ(RotationQ.w, RotationQ.x, RotationQ.y, RotationQ.z);
		mat.matRot_ = rotQ.GetRotMatrix();

		// �ړ����Ԃ��A�ړ��ϊ��s��𐶐�����
		Vector3D Translation;
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

	if (boneMapping_.find(nodeName) != boneMapping_.end()) {
		size_t BoneIndex = boneMapping_[nodeName];
		boneInfo_[BoneIndex].finalTransformation = globalInverseTransform_;
		boneInfo_[BoneIndex].finalTransformation *= boneInfo_[BoneIndex].boneOffset;
		boneInfo_[BoneIndex].finalTransformation *= GlobalTransformation;
	}

	for (size_t i = 0; i < nodes_[nodeName].childrenName.size(); i++) {
		ReadNodeHeirarchy(animData, AnimationTime, GlobalTransformation, nodes_[nodeName].childrenName[i]);
	}
}