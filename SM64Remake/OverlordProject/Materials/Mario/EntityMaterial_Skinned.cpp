#include "stdafx.h"
#include "EntityMaterial_Skinned.h"

EntityMaterial_Skinned::EntityMaterial_Skinned() :
	Material<EntityMaterial_Skinned>(L"Effects/Mario/EntityShader_Skinned.fx")

{
}

void EntityMaterial_Skinned::SetDiffuseTexture(const std::wstring& assetFile)
{

	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
	SetVariable_Texture(L"gDiffuseMap", m_pDiffuseTexture);
}

void EntityMaterial_Skinned::SetOpacityTexture(const std::wstring& assetFile)
{

	m_pOpacityTexture = ContentManager::Load<TextureData>(assetFile);
	SetVariable_Texture(L"gOpacityMap", m_pOpacityTexture);
}

void EntityMaterial_Skinned::InitializeEffectVariables()
{
}

void EntityMaterial_Skinned::OnUpdateModelVariables(const SceneContext&, const ModelComponent* pModel) const
{
	ModelAnimator* pAnim = pModel->GetAnimator();
	ASSERT_NULL_(pAnim);

	const auto& bones = pAnim->GetBoneTransforms();

	SetVariable_MatrixArray(L"gBones", reinterpret_cast<const float*>(bones.data()), static_cast<UINT>(bones.size()));
}
