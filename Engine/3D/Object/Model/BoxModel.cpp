#include "BoxModel.h"

void MNE::BoxModel::LoadModel(const std::string& /*modelname*/, bool /*smoothing*/)
{
	Material* material = nullptr;
	material = Material::Create();

	AddMaterial(material);

	meshes_.emplace_back();
	Mesh* mesh = &meshes_.back();

	AddVertex(mesh);
	AddIndex(mesh);
	mesh->SetMaterial(material);
}

void MNE::BoxModel::AddIndex(Mesh* mesh)
{
	mesh->AddIndex(0);
	mesh->AddIndex(1);
	mesh->AddIndex(2);

	mesh->AddIndex(2);
	mesh->AddIndex(1);
	mesh->AddIndex(3);

	mesh->AddIndex(6);
	mesh->AddIndex(5);
	mesh->AddIndex(4);

	mesh->AddIndex(7);
	mesh->AddIndex(5);
	mesh->AddIndex(6);

	mesh->AddIndex(8);
	mesh->AddIndex(9);
	mesh->AddIndex(10);

	mesh->AddIndex(10);
	mesh->AddIndex(9);
	mesh->AddIndex(11);

	mesh->AddIndex(14);
	mesh->AddIndex(13);
	mesh->AddIndex(12);

	mesh->AddIndex(15);
	mesh->AddIndex(13);
	mesh->AddIndex(14);

	mesh->AddIndex(18);
	mesh->AddIndex(17);
	mesh->AddIndex(16);

	mesh->AddIndex(19);
	mesh->AddIndex(17);
	mesh->AddIndex(18);

	mesh->AddIndex(20);
	mesh->AddIndex(21);
	mesh->AddIndex(22);

	mesh->AddIndex(22);
	mesh->AddIndex(21);
	mesh->AddIndex(23);
}

void MNE::BoxModel::AddVertex(Mesh* mesh)
{
	MNE::ModelVertex vertex{};

	float size = 0.5f;
	
	//	前面
	//	左下
	vertex.pos = { -size,-size,-size };
	vertex.normal = { 0.0f,0.0f,-1.0f };
	vertex.uv = { 0.0f, 1.0f };
	vertex.boneWeight[0] = 1.0f;			//	fbxVertex用
	mesh->AddVertex(vertex);
	
	//	左上
	vertex.pos = { -size, size,-size };
	vertex.normal = { 0.0f,0.0f,-1.0f };
	vertex.uv = { 0.0f, 0.0f };
	vertex.boneWeight[0] = 1.0f;			//	fbxVertex用
	mesh->AddVertex(vertex);

	//	右下
	vertex.pos = { size,-size,-size };
	vertex.normal = { 0.0f,0.0f,-1.0f };
	vertex.uv = { 1.0f, 1.0f };
	vertex.boneWeight[0] = 1.0f;			//	fbxVertex用
	mesh->AddVertex(vertex);
	
	//	右上
	vertex.pos = { size, size,-size };
	vertex.normal = { 0.0f,0.0f,-1.0f };
	vertex.uv = { 1.0f, 0.0f };
	vertex.boneWeight[0] = 1.0f;			//	fbxVertex用
	mesh->AddVertex(vertex);
	
	//	後面
	//	左下
	vertex.pos = { -size,-size, size };
	vertex.normal = { 0.0f,0.0f,1.0f };
	vertex.uv = { 0.0f, 1.0f };
	vertex.boneWeight[0] = 1.0f;			//	fbxVertex用
	mesh->AddVertex(vertex);
	
	//	左上
	vertex.pos = { -size, size, size };
	vertex.normal = { 0.0f,0.0f,1.0f };
	vertex.uv = { 0.0f, 0.0f };
	vertex.boneWeight[0] = 1.0f;			//	fbxVertex用
	mesh->AddVertex(vertex);

	//	右下
	vertex.pos = { size,-size, size };
	vertex.normal = { 0.0f,0.0f,1.0f };
	vertex.uv = { 1.0f, 1.0f };
	vertex.boneWeight[0] = 1.0f;			//	fbxVertex用
	mesh->AddVertex(vertex);
	
	//	右上
	vertex.pos = { size, size, size };
	vertex.normal = { 0.0f,0.0f,1.0f };
	vertex.uv = { 1.0f, 0.0f };
	vertex.boneWeight[0] = 1.0f;			//	fbxVertex用
	mesh->AddVertex(vertex);
	
	//	左面
	//	左下
	vertex.pos = { -size,-size,-size };
	vertex.normal = { -1.0f,0.0f,0.0f };
	vertex.uv = { 0.0f, 1.0f };
	vertex.boneWeight[0] = 1.0f;			//	fbxVertex用
	mesh->AddVertex(vertex);
	
	//	左上
	vertex.pos = { -size,-size, size };
	vertex.normal = { -1.0f,0.0f,0.0f };
	vertex.uv = { 0.0f, 0.0f };
	vertex.boneWeight[0] = 1.0f;			//	fbxVertex用
	mesh->AddVertex(vertex);

	//	右下
	vertex.pos = { -size, size,-size };
	vertex.normal = { -1.0f,0.0f,0.0f };
	vertex.uv = { 1.0f, 1.0f };
	vertex.boneWeight[0] = 1.0f;			//	fbxVertex用
	mesh->AddVertex(vertex);
	
	//	右上
	vertex.pos = { -size, size, size };
	vertex.normal = { -1.0f,0.0f,0.0f };
	vertex.uv = { 1.0f, 0.0f };
	vertex.boneWeight[0] = 1.0f;			//	fbxVertex用
	mesh->AddVertex(vertex);
	
	//	右面
	//	左下
	vertex.pos = { size,-size,-size };
	vertex.normal = { 1.0f,0.0f,0.0f };
	vertex.uv = { 0.0f, 1.0f };
	vertex.boneWeight[0] = 1.0f;			//	fbxVertex用
	mesh->AddVertex(vertex);
	
	//	左上
	vertex.pos = { size,-size, size };
	vertex.normal = { 1.0f,0.0f,0.0f };
	vertex.uv = { 0.0f, 0.0f };
	vertex.boneWeight[0] = 1.0f;			//	fbxVertex用
	mesh->AddVertex(vertex);

	//	右下
	vertex.pos = { size, size,-size };
	vertex.normal = { 1.0f,0.0f,0.0f };
	vertex.uv = { 1.0f, 1.0f };
	vertex.boneWeight[0] = 1.0f;			//	fbxVertex用
	mesh->AddVertex(vertex);
	
	//	右上
	vertex.pos = { size, size, size };
	vertex.normal = { 1.0f,0.0f,0.0f };
	vertex.uv = { 1.0f, 0.0f };
	vertex.boneWeight[0] = 1.0f;			//	fbxVertex用
	mesh->AddVertex(vertex);
	
	//	下面
	//	左下
	vertex.pos = { -size,-size,-size };
	vertex.normal = { 0.0f,-1.0f,0.0f };
	vertex.uv = { 0.0f, 1.0f };
	vertex.boneWeight[0] = 1.0f;			//	fbxVertex用
	mesh->AddVertex(vertex);
	
	//	左上
	vertex.pos = { -size,-size, size };
	vertex.normal = { 0.0f,-1.0f,0.0f };
	vertex.uv = { 0.0f, 0.0f };
	vertex.boneWeight[0] = 1.0f;			//	fbxVertex用
	mesh->AddVertex(vertex);

	//	右下
	vertex.pos = { size,-size,-size };
	vertex.normal = { 0.0f,-1.0f,0.0f };
	vertex.uv = { 1.0f, 1.0f };
	vertex.boneWeight[0] = 1.0f;			//	fbxVertex用
	mesh->AddVertex(vertex);
	
	//	右上
	vertex.pos = { size,-size, size };
	vertex.normal = { 0.0f,-1.0f,0.0f };
	vertex.uv = { 1.0f, 0.0f };
	vertex.boneWeight[0] = 1.0f;			//	fbxVertex用
	mesh->AddVertex(vertex);
	
	//	上面
	//	左下
	vertex.pos = { -size, size,-size };
	vertex.normal = { 0.0f,1.0f,0.0f };
	vertex.uv = { 0.0f, 1.0f };
	vertex.boneWeight[0] = 1.0f;			//	fbxVertex用
	mesh->AddVertex(vertex);
	
	//	左上
	vertex.pos = { -size, size, size };
	vertex.normal = { 0.0f,1.0f,0.0f };
	vertex.uv = { 0.0f, 0.0f };
	vertex.boneWeight[0] = 1.0f;			//	fbxVertex用
	mesh->AddVertex(vertex);

	//	右下
	vertex.pos = { size, size,-size };
	vertex.normal = { 0.0f,1.0f,0.0f };
	vertex.uv = { 1.0f, 1.0f };
	vertex.boneWeight[0] = 1.0f;			//	fbxVertex用
	mesh->AddVertex(vertex);
	
	//	右上
	vertex.pos = { size, size, size };
	vertex.normal = { 0.0f,1.0f,0.0f };
	vertex.uv = { 1.0f, 0.0f };
	vertex.boneWeight[0] = 1.0f;			//	fbxVertex用
	mesh->AddVertex(vertex);
}

MNE::BoxModel::BoxModel(const char* filename, bool smoothing)
{
	Initialize(filename, smoothing);
}

MNE::BoxModel::~BoxModel()
{
	meshes_.clear();

	materials_.clear();
}
