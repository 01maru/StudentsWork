#pragma once
#include "IModel.h"
#include "Object3D.h"
#include <map>

class ModelManager
{
private:
	std::map<std::string, std::unique_ptr<IModel>, std::less<>> models_;
	std::string searchWord_;
	int32_t previewIdx_ = 0;

	std::unique_ptr<Object3D> previewObj_;
	bool preview_ = false;

private:
	ModelManager() {}
	~ModelManager() {}

public:
	static ModelManager* GetInstance();
	ModelManager(const ModelManager& obj) = delete;
	ModelManager& operator=(const ModelManager& obj) = delete;

	void Initialize();
	void ImGuiUpdate();
	void DrawPreview();

	//	何も入力しないとboxが読み込まれる
	void LoadModel(const std::string& filename = "", bool fbx = false);
	void DeleteModel(const std::string& filename);

	//	Getter
	IModel* GetModel(const std::string& modelname = "");
};

