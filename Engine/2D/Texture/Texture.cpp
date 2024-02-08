#include "Texture.h"
#include <cassert>

void MNE::Texture::Initialize(const std::string& texName, int32_t handle, ID3D12Resource* texBuff_ptr)
{
	name_ = texName;
	handle_ = handle;
	texBuff_ = texBuff_ptr;
}

MyMath::Vector2D MNE::Texture::GetTextureSize()
{
	ID3D12Resource* texBuff = texBuff_.Get();
	assert(texBuff);

	D3D12_RESOURCE_DESC resDesc = texBuff->GetDesc();
	MyMath::Vector2D size;
	size.x = static_cast<float>(resDesc.Width);
	size.y = static_cast<float>(resDesc.Height);

	return size;
}
