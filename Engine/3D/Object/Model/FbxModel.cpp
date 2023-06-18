//#include "FbxModel.h"
//#include "Quaternion.h"
//#include <assimp/scene.h>
//#include <assimp/postprocess.h>
//
//using namespace std;
//
//#pragma region string�ϊ�
//std::string GetDirectoryPath(const std::string& origin)
//{
//	int ind = (int)origin.find_last_of('/');
//	ind++;
//	return origin.substr(0, ind);
//}
//
//std::wstring ToWideString(const std::string& str)
//{
//	auto num1 = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str.c_str(), -1, nullptr, 0);
//
//	std::wstring wstr;
//	wstr.resize(num1);
//
//	auto num2 = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, str.c_str(), -1, &wstr[0], num1);
//
//	assert(num1 == num2);
//	return wstr;
//}
//#pragma endregion
//
//#pragma region assimp����ϊ�
////	�]�u���ĕϊ�
//void TransformMatToAiMat(Matrix& mat, const aiMatrix4x4 aiMat)
//{
//	mat.m[0][0] = aiMat.a1;
//	mat.m[1][0] = aiMat.a2;
//	mat.m[2][0] = aiMat.a3;
//	mat.m[3][0] = aiMat.a4;
//	mat.m[0][1] = aiMat.b1;
//	mat.m[1][1] = aiMat.b2;
//	mat.m[2][1] = aiMat.b3;
//	mat.m[3][1] = aiMat.b4;
//	mat.m[0][2] = aiMat.c1;
//	mat.m[1][2] = aiMat.c2;
//	mat.m[2][2] = aiMat.c3;
//	mat.m[3][2] = aiMat.c4;
//	mat.m[0][3] = aiMat.d1;
//	mat.m[1][3] = aiMat.d2;
//	mat.m[2][3] = aiMat.d3;
//	mat.m[3][3] = aiMat.d4;
//}
//
//const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, std::string name_)
//{
//	for (UINT i = 0; i < pAnimation->mNumChannels; i++) {
//		const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];
//
//		if (string(pNodeAnim->mNodeName.data) == name_) {
//			return pNodeAnim;
//		}
//	}
//
//	return NULL;
//}
//
//UINT FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim)
//{
//	assert(pNodeAnim->mNumRotationKeys > 0);
//
//	for (UINT i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++) {
//		if (AnimationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime) {
//			return i;
//		}
//	}
//
//	assert(0);
//	return 0;
//}
//
//UINT FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim)
//{
//	assert(pNodeAnim->mNumScalingKeys > 0);
//
//	for (UINT i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++) {
//		if (AnimationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime) {
//			return i;
//		}
//	}
//
//	assert(0);
//	return 0;
//}
//
//void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
//{
//	// ��Ԃɂ͍Œ�ł��Q�̒l���K�v�c
//	if (pNodeAnim->mNumRotationKeys == 1) {
//		Out = pNodeAnim->mRotationKeys[0].mValue;
//		return;
//	}
//
//	UINT RotationIndex = FindRotation(AnimationTime, pNodeAnim);
//	UINT NextRotationIndex = (RotationIndex + 1);
//	assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
//	float DeltaTime = (float)(pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime);
//	float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
//	assert(Factor >= 0.0f && Factor <= 1.0f);
//	const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
//	const aiQuaternion& EndRotationQ = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
//	aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
//	Out = Out.Normalize();
//}
//
//void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
//{
//	if (pNodeAnim->mNumScalingKeys == 1) {
//		Out = pNodeAnim->mScalingKeys[0].mValue;
//		return;
//	}
//
//	UINT ScalingIndex = FindScaling(AnimationTime, pNodeAnim);
//	UINT NextScalingIndex = (ScalingIndex + 1);
//	assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);
//	float DeltaTime = (float)(pNodeAnim->mScalingKeys[NextScalingIndex].mTime - pNodeAnim->mScalingKeys[ScalingIndex].mTime);
//	float Factor = (AnimationTime - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
//	assert(Factor >= 0.0f && Factor <= 1.0f);
//	const aiVector3D& Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
//	const aiVector3D& End = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
//	aiVector3D Delta = End - Start;
//	Out = Start + Factor * Delta;
//}
//
//UINT FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
//{
//	for (UINT i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++) {
//		if (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime) {
//			return i;
//		}
//	}
//
//	assert(0);
//
//	return 0;
//}
//
//void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
//{
//	if (pNodeAnim->mNumPositionKeys == 1) {
//		Out = pNodeAnim->mPositionKeys[0].mValue;
//		return;
//	}
//
//	UINT PositionIndex = FindPosition(AnimationTime, pNodeAnim);
//	UINT NextPositionIndex = (PositionIndex + 1);
//	assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);
//	float DeltaTime = (float)(pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime);
//	float Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
//	assert(Factor >= 0.0f && Factor <= 1.0f);
//	const aiVector3D& Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
//	const aiVector3D& End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
//	aiVector3D Delta = End - Start;
//	Out = Start + Factor * Delta;
//}
//#pragma endregion
//
//FbxModel::FbxModel(const char* filename, bool smoothing)
//{
//	Initialize(filename, smoothing);
//}
//
//FbxModel::~FbxModel()
//{
//	for (auto m : meshes_) {
//		delete m;
//	}
//	meshes_.clear();
//
//	for (auto m : materials_) {
//		delete m.second;
//	}
//	materials_.clear();
//}
//
//void FbxModel::BoneTransform(float TimeInSeconds, std::vector<Matrix>& transforms)
//{
//	Matrix Identity;
//
//	double TicksPerSecond = modelScene->mAnimations[0]->mTicksPerSecond != 0 ? modelScene->mAnimations[0]->mTicksPerSecond : 25.0f;
//	float TimeInTicks = TimeInSeconds * (float)TicksPerSecond;
//	float AnimationTime = (float)fmod(TimeInTicks, modelScene->mAnimations[0]->mDuration);
//
//	ReadNodeHeirarchy(AnimationTime, modelScene->mRootNode, Identity);
//
//	transforms.resize(numBones);
//
//	for (UINT i = 0; i < numBones; i++) {
//		transforms[i] = boneInfo[i].finalTransformation;
//	}
//}
//
//void FbxModel::LoadModel(const std::string& modelname, bool /*smoothing*/)
//{
//	//	�p�X�̐ݒ�
//	const string filename = modelname + ".fbx";
//	const string directoryPath = "Resources/Model/" + modelname + "/";
//
//	//	�V�[���̃��[�h
//	modelScene = importer.ReadFile(directoryPath + filename, aiProcess_Triangulate);	//	�O�p�ʉ�
//
//	//	�ǂݍ��ݎ��s������
//	if (modelScene == nullptr) { return; }
//
//	//	GlobalInverseTransform�ݒ�
//	TransformMatToAiMat(globalInverseTransform_, modelScene->mRootNode->mTransformation);
//
//	//	mesh���ݒ�
//	meshes_.reserve(modelScene->mNumMeshes);
//	for (UINT i = 0; i < modelScene->mNumMeshes; i++)
//	{
//		//	aiMesh�^�̏��擾
//		const auto pMesh = modelScene->mMeshes[i];
//
//		//	�^�ϊ�
//		meshes_.emplace_back(new Mesh);
//		Mesh* mesh = meshes_.back();
//		LoadMesh(*mesh, pMesh);
//		//	�{�[���擾
//		LoadBone(i, pMesh);
//
//		//	material�擾
//		aiMaterial* pMaterial = modelScene->mMaterials[i];
//
//		LoadMaterial(mesh, pMaterial, i);
//
//		SetTextureFilePath(directoryPath + filename, *mesh, pMaterial);
//	}
//}
//
//void FbxModel::LoadMaterial(Mesh* dst, const aiMaterial* src, int index)
//{
//	Material* material = Material::Create();
//
//	material->name_ += to_string(index);
//
//	//	Diffuse
//	aiColor3D difcolor(0.f, 0.f, 0.f);
//	src->Get(AI_MATKEY_COLOR_DIFFUSE, difcolor);
//	material->diffuse_.x = difcolor.r;
//	material->diffuse_.y = difcolor.g;
//	material->diffuse_.z = difcolor.b;
//	//	AMBIENT
//	aiColor3D amcolor(0.3f, 0.3f, 0.3f);
//	src->Get(AI_MATKEY_COLOR_AMBIENT, amcolor);
//	material->ambient_.x = amcolor.r;
//	material->ambient_.y = amcolor.g;
//	material->ambient_.z = amcolor.b;
//	//	SPECULAR
//	aiColor3D specolor(0.3f, 0.3f, 0.3f);
//	src->Get(AI_MATKEY_COLOR_SPECULAR, specolor);
//	material->specular_.x = specolor.r;
//	material->specular_.y = specolor.g;
//	material->specular_.z = specolor.b;
//
//	if (material) {
//		// �}�e���A����o�^
//		AddMaterial(material);
//	}
//
//	dst->SetMaterial(material);
//}
//
//void FbxModel::LoadMesh(Mesh& dst, const aiMesh* src)
//{
//	aiVector3D zero3D(0.0f, 0.0f, 0.0f);
//
//	//	���_����
//	for (UINT i = 0; i < src->mNumVertices; i++)
//	{
//		auto position = &(src->mVertices[i]);
//		auto normal = &(src->mNormals[i]);
//		auto uv = (src->HasTextureCoords(0)) ? &(src->mTextureCoords[0][i]) : &zero3D;
//		//	uv���]
//		uv->y = 1 - uv->y;
//
//		ModelVertex vertex_ = {};
//		vertex_.pos = Vector3D(position->x, position->y, -position->z);
//		vertex_.normal = Vector3D(normal->x, normal->y, normal->z);
//		vertex_.uv = Vector2D(uv->x, uv->y);
//		vertex_.boneIndex[0] = 31;				//	bone�ő勖�e��-1
//		vertex_.boneWeight[0] = 1.0f;			//	�{�[���Ȃ��p
//
//		dst.AddVertex(vertex_);
//	}
//
//	//	index�ݒ�
//	for (UINT i = 0; i < src->mNumFaces; i++)
//	{
//		const auto& face = src->mFaces[i];
//
//		dst.AddIndex((unsigned short)face.mIndices[0]);
//		dst.AddIndex((unsigned short)face.mIndices[2]);
//		dst.AddIndex((unsigned short)face.mIndices[1]);
//	}
//}
//
//void FbxModel::LoadBone(UINT meshIndex, const aiMesh* src)
//{
//	//	bone���ݒ�
//	for (UINT i = 0; i < src->mNumBones; i++) {
//		UINT BoneIndex = 0;
//		string BoneName(src->mBones[i]->mName.data);
//
//		if (boneMapping.find(BoneName) == boneMapping.end()) {
//			BoneIndex = numBones;
//			numBones++;
//			BoneInfo bi;
//			boneInfo.push_back(bi);
//			//	�^�ϊ�
//			TransformMatToAiMat(boneInfo[BoneIndex].boneOffset, src->mBones[i]->mOffsetMatrix);
//			boneMapping[BoneName] = BoneIndex;
//		}
//		else {
//			BoneIndex = boneMapping[BoneName];
//		}
//
//		for (UINT j = 0; j < src->mBones[i]->mNumWeights; j++) {
//			UINT VertexID = src->mBones[i]->mWeights[j].mVertexId;
//			float Weight = src->mBones[i]->mWeights[j].mWeight;
//			meshes_[meshIndex]->SetBone(VertexID, BoneIndex, Weight);
//		}
//	}
//}
//
//void FbxModel::SetTextureFilePath(const std::string& filename, Mesh& dst, const aiMaterial* src)
//{
//	aiString path;
//	if (src->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), path) == AI_SUCCESS)
//	{
//		auto dir = GetDirectoryPath(filename);
//		auto file = std::string(path.C_Str());
//		dst.SetTextureFilePath(dir + file);
//		dst.GetMaterial()->name_ = file;
//	}
//}
//
//void FbxModel::ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const Matrix& ParentTransform)
//{
//	string NodeName(pNode->mName.data);
//
//	//	�����A�j���[�V��������Ȃ炱���ŕύX
//	const aiAnimation* pAnimation = modelScene->mAnimations[0];
//
//	Matrix NodeTransformation;
//	TransformMatToAiMat(NodeTransformation, pNode->mTransformation);
//
//	const aiNodeAnim* pNodeAnim = FindNodeAnim(pAnimation, NodeName);
//
//	if (pNodeAnim) {
//		// �X�P�[�����O���Ԃ��A�X�P�[�����O�ϊ��s��𐶐�����
//		aiVector3D Scaling;
//		CalcInterpolatedScaling(Scaling, AnimationTime, pNodeAnim);
//		MyMath::ObjMatrix mat;
//		mat.scale = Vector3D(Scaling.x, Scaling.y, Scaling.z);
//		mat.SetMatScaling();
//
//		// ��]���Ԃ��A��]�ϊ��s��𐶐�����
//		aiQuaternion RotationQ;
//		CalcInterpolatedRotation(RotationQ, AnimationTime, pNodeAnim);
//		Quaternion rotQ(RotationQ.w, RotationQ.x, RotationQ.y, RotationQ.z);
//		mat.matRot = rotQ.GetRotMatrix();
//
//		// �ړ����Ԃ��A�ړ��ϊ��s��𐶐�����
//		aiVector3D Translation;
//		CalcInterpolatedPosition(Translation, AnimationTime, pNodeAnim);
//		mat.trans = Vector3D(Translation.x, Translation.y, Translation.z);
//		mat.SetMatTransform();
//
//		// ������L�̕ϊ�����������
//		NodeTransformation = mat.matScale;
//		NodeTransformation *= mat.matRot;
//		NodeTransformation *= mat.matTrans;
//	}
//
//	Matrix GlobalTransformation = NodeTransformation;
//	GlobalTransformation *= ParentTransform;
//
//	if (boneMapping.find(NodeName) != boneMapping.end()) {
//		UINT BoneIndex = boneMapping[NodeName];
//		boneInfo[BoneIndex].finalTransformation = globalInverseTransform_;
//		boneInfo[BoneIndex].finalTransformation *= boneInfo[BoneIndex].boneOffset;
//		boneInfo[BoneIndex].finalTransformation *= GlobalTransformation;
//	}
//
//	for (UINT i = 0; i < pNode->mNumChildren; i++) {
//		ReadNodeHeirarchy(AnimationTime, pNode->mChildren[i], GlobalTransformation);
//	}
//}
