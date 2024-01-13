#pragma once
#include <string>
#include "Vector3D.h"
#include "Texture.h"
#include "ConstBuff.h"

namespace MNE
{

	namespace CBuff {
		struct CBufferMaterialData;
	}

	class Material
	{
	public:
		std::string name_;
		Vector3D ambient_ = { 1.0f,1.0f,1.0f };
		Vector3D diffuse_ = { 0.8f,0.8f,0.8f };
		Vector3D specular_ = { 0.5f,0.5f,0.5f };
		float alpha_ = 1.0f;

		std::string textureFilename_;
		std::string filepath_;

		MNE::ConstBuff material_;

		MNE::Texture* texture_;

	public:
		static Material* Create();

		void Initialize();
		void Update();
		void LoadTexture();
		void SetGraphicsRootCBuffView(uint32_t rootparaIdx);

		int32_t GetTextureHandle();
	};

}
