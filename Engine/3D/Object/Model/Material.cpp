#include "Material.h"
#include "TextureManager.h"
#include "ConstBuffStruct.h"
#include <cassert>
#include "ConvertString.h"

MNE::Material* MNE::Material::Create()
{
	Material* instance = new Material;
	instance->Initialize();

	return instance;
}

void MNE::Material::Initialize()
{
	material_.Initialize((sizeof(CBuff::CBufferMaterialData) + 0xFF) & ~0xFF);
}

void MNE::Material::Update()
{
	// 定数バッファへデータ転送
	CBuff::CBufferMaterialData* constMap = nullptr;
	HRESULT result = material_.GetResource()->Map(0, nullptr, (void**)&constMap);

	if (SUCCEEDED(result)) {
		constMap->ambient = ambient_;
		constMap->diffuse = diffuse_;
		constMap->specular = specular_;
		constMap->alpha = alpha_;
		material_.GetResource()->Unmap(0, nullptr);
	}
}

//std::wstring ReplaceExtension(const std::wstring& origin, const wchar_t* ext)
//{
//	int last = (int)origin.find_last_of('.');
//	std::wstring ret = origin.substr(0, last + 1);
//	wchar_t buf[5];
//	std::swprintf(buf, 5, L"%s", ext);
//	ret.append(ext);
//	return ret;
//}

void MNE::Material::LoadTexture()
{
	std::string fileName = Util::GetFileName(filepath_);
	std::string dirPath = Util::GetDirectoryPath(filepath_);

	texture_ = TextureManager::GetInstance()->AsyncLoadTextureGraph(fileName, dirPath);
}

void MNE::Material::SetGraphicsRootCBuffView(uint32_t rootparaIdx)
{
	material_.SetGraphicsRootCBuffView(rootparaIdx);
}

int32_t MNE::Material::GetTextureHandle()
{
	if (texture_ == nullptr)
	{
		texture_ = TextureManager::GetInstance()->GetTextureGraph(Util::GetFileName(filepath_));
	}

	return texture_->GetHandle();
}
