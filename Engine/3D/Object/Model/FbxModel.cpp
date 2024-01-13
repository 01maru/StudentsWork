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
using namespace MNE;

MNE::FbxModel::FbxModel(const char* filename, bool smoothing)
{
	IModel::Initialize(filename, smoothing);
}

void MNE::FbxModel::LoadModel(const std::string& modelname, bool smoothing)
{
	//	パスの設定
	const string filename = modelname + ".fbx";
	const string directoryPath = "Resources/Model/" + modelname + "/";

	//	シーンのロード
	Assimp::Importer importer;
	uint32_t aiProcess = aiProcess_Triangulate | aiProcess_ConvertToLeftHanded;
	if (smoothing) aiProcess |= aiProcess_GenSmoothNormals;

	const aiScene* modelScene = importer.ReadFile(directoryPath + filename, aiProcess);	//	三角面化

	//	読み込み失敗したら
	if (modelScene == nullptr) { return; }

	//	GlobalInverseTransform設定
	MNE::Util::TransformMatToAiMat(globalInverseTransform_, modelScene->mRootNode->mTransformation);

	//	mesh情報設定
	meshes_.reserve(modelScene->mNumMeshes);
	for (size_t i = 0; i < modelScene->mNumMeshes; i++)
	{
		//	aiMesh型の情報取得
		const auto pMesh = modelScene->mMeshes[i];

		//	型変換
		meshes_.emplace_back();
		Mesh* mesh = &meshes_.back();
		LoadMesh(*mesh, pMesh);
		//	ボーン取得
		LoadBone(i, pMesh);

		//	material取得
		aiMaterial* pMaterial = modelScene->mMaterials[0];

		LoadMaterial(mesh, pMaterial->GetName().C_Str());

		SetTextureFilePath(directoryPath + filename, *mesh, pMaterial);
	}

	LoadAnimation(modelScene);
	LoadNodes(modelScene->mRootNode);
}

void MNE::FbxModel::LoadNodeHeirarchy(const aiNode* pNode)
{
	Node node;

	//	NodeTransform
	MNE::Util::TransformMatToAiMat(node.transformation, pNode->mTransformation);

	//	ChildrenName
	for (size_t i = 0; i < pNode->mNumChildren; i++) {
		node.childrenName.emplace_back(pNode->mChildren[i]->mName.C_Str());
	}

	nodes_.emplace(pNode->mName.C_Str(), node);

	//	再帰
	for (size_t i = 0; i < pNode->mNumChildren; i++) {
		LoadNodeHeirarchy(pNode->mChildren[i]);
	}
}

void MNE::FbxModel::LoadNodes(const aiNode* rootNode)
{
	rootNodeName_ = rootNode->mName.C_Str();

	LoadNodeHeirarchy(rootNode);
}

void MNE::FbxModel::LoadAnimation(const aiScene* modelScene)
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

void MNE::FbxModel::LoadMaterial(Mesh* dst, const std::string& name)
{
	if (SerchMaterial(name) == TRUE) {
		dst->SetMaterial(GetMaterial(name));
		return;
	}

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
		// マテリアルを登録
		AddMaterial(material);
	}

	dst->SetMaterial(material);
}

void MNE::FbxModel::LoadMesh(Mesh& dst, const aiMesh* src)
{
	aiVector3D zero3D(0.0f, 0.0f, 0.0f);

	//	頂点生成
	for (size_t i = 0; i < src->mNumVertices; i++)
	{
		auto position = &(src->mVertices[i]);
		auto normal = &(src->mNormals[i]);
		auto uv = (src->HasTextureCoords(0)) ? &(src->mTextureCoords[0][i]) : &zero3D;

		ModelVertex vertex_ = {};
		vertex_.pos = Vector3D(position->x, position->y, position->z);
		vertex_.normal = Vector3D(normal->x, normal->y, normal->z);
		vertex_.uv = Vector2D(uv->x, uv->y);
		//vertex_.boneIndex[0] = 31;				//	bone最大許容数-1
		//vertex_.boneWeight[0] = 1.0f;			//	ボーンなし用

		dst.AddVertex(vertex_);
	}

	//	index設定
	for (size_t i = 0; i < src->mNumFaces; i++)
	{
		const auto& face = src->mFaces[i];

		dst.AddIndex((unsigned short)face.mIndices[0]);
		dst.AddIndex((unsigned short)face.mIndices[1]);
		dst.AddIndex((unsigned short)face.mIndices[2]);
	}
}

void MNE::FbxModel::LoadBone(size_t meshIndex, const aiMesh* src)
{
	//	bone情報設定
	for (size_t i = 0; i < src->mNumBones; i++) {
		uint16_t BoneIndex = 0;
		string BoneName(src->mBones[i]->mName.data);

		if (boneMapping_.find(BoneName) == boneMapping_.end()) {
			BoneIndex = (uint16_t)numBones_;
			numBones_++;
			BoneInfo bi;
			boneInfo_.push_back(bi);
			//	型変換
			MNE::Util::TransformMatToAiMat(boneInfo_[BoneIndex].boneOffset, src->mBones[i]->mOffsetMatrix);
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

void MNE::FbxModel::SetTextureFilePath(const std::string& filename, Mesh& dst, const aiMaterial* src)
{
	aiString path;
	if (src->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), path) == AI_SUCCESS)
	{
		auto dir = MNE::Util::GetDirectoryPath(filename);
		auto file = std::string(path.C_Str());
		dst.SetTextureFilePath(dir + file);
		dst.GetMaterial()->name_ = file;
	}
}

void MNE::FbxModel::ImGuiUpdate()
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

void MNE::FbxModel::BoneTransform(float TimeInSeconds, std::vector<Matrix>& transforms, int32_t animationIdx)
{
	Matrix Identity;

	if (animations_.size() <= animationIdx || animationIdx < 0) return;

	//double TicksPerSecond = animations_[index].ticksPerSecond != 0 ? animations_[index].ticksPerSecond : 25.0f;
	//float TimeInTicks = TimeInSeconds * (float)TicksPerSecond;	//	frame数
	float AnimationTime = (float)fmod(TimeInSeconds, animations_[animationIdx].duration);	//	現在のフレーム数/1Loopのフレーム数　のあまり

	ReadNodeHeirarchy(animations_[animationIdx], AnimationTime, Identity, rootNodeName_);

	transforms.resize(numBones_);

	for (size_t i = 0; i < numBones_; i++) {
		transforms[i] = boneInfo_[i].finalTransformation;
	}
}

void MNE::FbxModel::ReadNodeHeirarchy(const AnimationData& animData, float AnimationTime, const Matrix& ParentTransform, const std::string& nodeName)
{
	//	nodeがあるか
	if (nodes_.count(nodeName) == 0) assert(0);
	Matrix NodeTransformation = nodes_[nodeName].transformation;

	const KeyChannels* pNodeAnim = MNE::Util::FindNodeChannel(animData, nodeName);

	if (pNodeAnim) {
		MyMath::ObjMatrix mat;

		// スケーリングを補間し、スケーリング変換行列を生成する
		Vector3D Scaling;
		MNE::Util::CalcInterpolatedScaling(Scaling, AnimationTime, pNodeAnim);
		mat.scale_ = Vector3D(Scaling.x, Scaling.y, Scaling.z);
		mat.SetMatScaling();

		// 回転を補間し、回転変換行列を生成する
		Quaternion RotationQ;
		MNE::Util::CalcInterpolatedRotation(RotationQ, AnimationTime, pNodeAnim);
		Quaternion rotQ(RotationQ.w, RotationQ.x, RotationQ.y, RotationQ.z);
		mat.matRot_ = rotQ.GetRotMatrix();

		// 移動を補間し、移動変換行列を生成する
		Vector3D Translation;
		MNE::Util::CalcInterpolatedPosition(Translation, AnimationTime, pNodeAnim);
		mat.trans_ = Vector3D(Translation.x, Translation.y, Translation.z);
		mat.SetMatTransform();

		// これら上記の変換を合成する
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