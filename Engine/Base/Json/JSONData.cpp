#include "JSONData.h"
#include "ModelManager.h"

void MNE::JSONData::SetObjects(std::vector<std::unique_ptr<Object3D>>& objects)
{
	ModelManager* model = ModelManager::GetInstance();

	for (auto& obj : objects_)
	{
		std::unique_ptr<MNE::Object3D> newObject;
		newObject = std::move(MNE::Object3D::Create(model->GetModel(obj.fileName)));

		newObject->SetPosition(obj.translation);
		newObject->SetRotation(obj.rotation);
		newObject->SetScale(obj.scaling);

		objects.push_back(std::move(newObject));
		obj.parent = nullptr;
	}
}
