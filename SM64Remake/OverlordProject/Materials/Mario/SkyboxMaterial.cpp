#include "stdafx.h"
#include "SkyboxMaterial.h"

SkyboxMaterial::SkyboxMaterial()
	:Material(L"Effects/Mario/Skybox.fx")
{
}

void SkyboxMaterial::SetDiffuseTexture(const std::wstring& assetFile)
{
	auto ShaderView = ContentManager::Load<TextureData>(assetFile)->GetShaderResourceView();
	SetVariable_Texture(L"m_CubeMap", ShaderView);
}

void SkyboxMaterial::InitializeEffectVariables()
{

}

void SkyboxMaterial::OnUpdateModelVariables(const SceneContext& , const ModelComponent*) const
{

}
