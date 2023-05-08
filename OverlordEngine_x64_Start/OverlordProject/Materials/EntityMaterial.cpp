#include "stdafx.h"
#include "EntityMaterial.h"

EntityMaterial::EntityMaterial() :
	Material<EntityMaterial>(L"Effects/Mario/EntityShader.fx")

{
}

void EntityMaterial::SetDiffuseTexture(const std::wstring& assetFile)
{

	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
	SetVariable_Texture(L"gDiffuseMap", m_pDiffuseTexture);
}

void EntityMaterial::SetOpacityTexture(const std::wstring& assetFile)
{

	m_pOpacityTexture = ContentManager::Load<TextureData>(assetFile);
	SetVariable_Texture(L"gOpacityMap", m_pOpacityTexture);
}

void EntityMaterial::InitializeEffectVariables()
{
}
