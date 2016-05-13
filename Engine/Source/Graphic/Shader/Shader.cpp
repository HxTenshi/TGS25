#include "Shader.h"


#include "Engine/AssetFile/Shader/ShaderFileData.h"

void Shader::Create(const char* fileName){
	AssetDataBase::Instance(fileName, m_ShaderAssetData);
}

void Shader::SetShader(bool UseAnime, ID3D11DeviceContext* context) const{
	if (!m_ShaderAssetData)return;
	m_ShaderAssetData->GetFileData()->GetShader()->SetShader(UseAnime,context);
}
