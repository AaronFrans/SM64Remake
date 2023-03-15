#include "stdafx.h"
#include "UberMaterial.h"

UberMaterial::UberMaterial()
{
}

void UberMaterial::SetDiffuseTexture(const std::wstring& assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
}

void UberMaterial::InitializeEffectVariables()
{
	SetVariable_Vector(L"gLightDirection", {0.5,0.5, 0.5});

	SetVariable_(L"gUseTextureDiffuse", m_pDiffuseTexture);
	
}
