﻿#include "ModelManager.h"
#include "ObjModel.h"
#include "FbxModel.h"
#include "BoxModel.h"
#include "ImGuiController.h"
#include "ImGuiManager.h"

ModelManager* ModelManager::GetInstance()
{
    static ModelManager instance;
    return &instance;
}

void ModelManager::Initialize()
{
    LoadModel("");
    previewObj_.reset(Object3D::Create(GetModel("")));
}

void ModelManager::ImGuiUpdate()
{
    if (!ImGuiController::GetInstance()->GetActiveModelManager()) return;

    ImGuiManager* imgui = ImGuiManager::GetInstance();

    imgui->BeginWindow("ModelManager");


    imgui->InputText("Search", searchWord_);

    imgui->CheckBox("Preview", preview_);

    int32_t previdx = previewIdx_;

    int32_t i = 0;
    std::string modelname;
    for (auto& model : models_)
    {
        //	Wordがなかったら
        if (searchWord_.length() != 0) {
            if (model.first.find(searchWord_) == -1) continue;
        }

		imgui->PushID(i++);
		imgui->Text("Name : %s", model.first.c_str());
        if (imgui->SetRadioButton("PreviewTex", previewIdx_, i)) modelname = model.first;

		imgui->Spacing();
		imgui->Separator();
		imgui->Spacing();

		imgui->PopID();
    }

    imgui->EndWindow();

    if (previdx != previewIdx_) {
        previewObj_.reset(Object3D::Create(GetModel(modelname)));
    }

    if (!preview_) return;

    previewObj_->MatUpdate();
}

void ModelManager::DrawPreview()
{
    if (!preview_) return;

    previewObj_->Draw();
}

void ModelManager::LoadModel(const std::string& filename, bool fbx)
{
    //  既に読み込まれてたら
    if (models_.count(filename) != 0) return;

    std::unique_ptr<IModel> model;
    //  何も入力されていなかったら
    if (filename.length() == 0)
    {
        model = std::make_unique<BoxModel>("");
        models_.emplace("", std::move(model));

        return;
    }

    if (fbx)    model = std::make_unique<FbxModel>(filename.c_str());
    else        model = std::make_unique<ObjModel>(filename.c_str());

    models_.emplace(filename, std::move(model));
}

void ModelManager::DeleteModel(const std::string& filename)
{
    models_.erase(filename);
}

IModel* ModelManager::GetModel(const std::string& modelname)
{
    //  読み込まれていなかったら
    if (models_.count(modelname) == 0) return nullptr;

    return models_.at(modelname).get();
}
