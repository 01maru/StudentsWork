#include "FbxModel.h"
#include "Quaternion.h"
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "LightManager.h"

#include "ConvertString.h"
#include "ConvertAiStruct.h"

using namespace std;

FbxModel::FbxModel(const char* filename, bool smoothing)
{
	Initialize(filename, smoothing);
}

FbxModel::~FbxModel() {}

void FbxModel::LoadModel(const std::string& modelname, bool /*smoothing*/)
{
	//	パスの設定
	const string filename = modelname + ".fbx";
	const string directoryPath = "Resources/Model/" + modelname + "/";

	//	シーンのロード
	modelScene = importer.ReadFile(directoryPath + filename, aiProcess_Triangulate);	//	三角面化

	//	読み込み失敗したら
	if (modelScene == nullptr) { return; }

	//	GlobalInverseTransform設定
	Util::TransformMatToAiMat(globalInverseTransform_, modelScene->mRootNode->mTransformation);

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
		aiMaterial* pMaterial = modelScene->mMaterials[i];

		LoadMaterial(mesh, pMaterial, i);

		SetTextureFilePath(directoryPath + filename, *mesh, pMaterial);
	}

	LoadAnimation();
}

void FbxModel::LoadAnimation()
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

void FbxModel::LoadMaterial(Mesh* dst, const aiMaterial* /*src*/, size_t index)
{
	Material* material = Material::Create();

	material->name_ += to_string(index);

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

void FbxModel::LoadMesh(Mesh& dst, const aiMesh* src)
{
	aiVector3D zero3D(0.0f, 0.0f, 0.0f);

	//	頂点生成
	for (size_t i = 0; i < src->mNumVertices; i++)
	{
		auto position = &(src->mVertices[i]);
		auto normal = &(src->mNormals[i]);
		auto uv = (src->HasTextureCoords(0)) ? &(src->mTextureCoords[0][i]) : &zero3D;
		//	uv反転
		uv->y = 1 - uv->y;

		ModelVertex vertex_ = {};
		vertex_.pos = Vector3D(position->x, position->y, -position->z);
		vertex_.normal = Vector3D(normal->x, normal->y, normal->z);
		vertex_.uv = Vector2D(uv->x, uv->y);
		vertex_.boneIndex[0] = 31;				//	bone最大許容数-1
		vertex_.boneWeight[0] = 1.0f;			//	ボーンなし用

		dst.AddVertex(vertex_);
	}

	//	index設定
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

void FbxModel::BoneTransform(float TimeInSeconds, std::vector<Matrix>& transforms)
{
	Matrix Identity;

	double TicksPerSecond = modelScene->mAnimations[0]->mTicksPerSecond != 0 ? modelScene->mAnimations[0]->mTicksPerSecond : 25.0f;
	float TimeInTicks = TimeInSeconds * (float)TicksPerSecond;	//	frame数
	float AnimationTime = (float)fmod(TimeInTicks, modelScene->mAnimations[0]->mDuration);	//	現在のフレーム数/1Loopのフレーム数　のあまり

	ReadNodeHeirarchy(AnimationTime, modelScene->mRootNode, Identity);

	transforms.resize(numBones_);

	for (size_t i = 0; i < numBones_; i++) {
		transforms[i] = boneInfo_[i].finalTransformation;
	}
}

void FbxModel::ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const Matrix& ParentTransform)
{
	string NodeName(pNode->mName.data);

	//	複数アニメーションあるならここで変更
	const aiAnimation* pAnimation = modelScene->mAnimations[0];

	Matrix NodeTransformation;
	Util::TransformMatToAiMat(NodeTransformation, pNode->mTransformation);

	const aiNodeAnim* pNodeAnim = Util::FindNodeAnim(pAnimation, NodeName);

	if (pNodeAnim) {
		MyMath::ObjMatrix mat;

		// スケーリングを補間し、スケーリング変換行列を生成する
		aiVector3D Scaling;
		Util::CalcInterpolatedScaling(Scaling, AnimationTime, pNodeAnim);
		mat.scale_ = Vector3D(Scaling.x, Scaling.y, Scaling.z);
		mat.SetMatScaling();

		// 回転を補間し、回転変換行列を生成する
		aiQuaternion RotationQ;
		Util::CalcInterpolatedRotation(RotationQ, AnimationTime, pNodeAnim);
		Quaternion rotQ(RotationQ.w, RotationQ.x, RotationQ.y, RotationQ.z);
		mat.matRot_ = rotQ.GetRotMatrix();

		// 移動を補間し、移動変換行列を生成する
		aiVector3D Translation;
		Util::CalcInterpolatedPosition(Translation, AnimationTime, pNodeAnim);
		mat.trans_ = Vector3D(Translation.x, Translation.y, Translation.z);
		mat.SetMatTransform();

		// これら上記の変換を合成する
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
