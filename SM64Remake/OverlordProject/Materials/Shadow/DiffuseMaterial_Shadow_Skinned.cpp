#include "stdafx.h"
#include "DiffuseMaterial_Shadow_Skinned.h"

DiffuseMaterial_Shadow_Skinned::DiffuseMaterial_Shadow_Skinned() :
	Material(L"Effects/Shadow/PosNormTex3D_Shadow_Skinned.fx")
{}

void DiffuseMaterial_Shadow_Skinned::SetDiffuseTexture(const std::wstring& assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
	SetVariable_Texture(L"gDiffuseMap", m_pDiffuseTexture);
}

void DiffuseMaterial_Shadow_Skinned::InitializeEffectVariables()
{
}

void DiffuseMaterial_Shadow_Skinned::OnUpdateModelVariables(const SceneContext& sceneContext, const ModelComponent* pModel) const
{
	/*
	 * TODO_W8
	 * Update The Shader Variables
	 * 1. Update the LightWVP > Used to Transform a vertex into Light clipping space
	 * 	LightWVP = model_world * light_viewprojection
	 * 	(light_viewprojection [LightVP] can be acquired from the ShadowMapRenderer)
	 *
	 * 2. Update the ShadowMap texture
	 *
	 * 3. Update the Light Direction (retrieve the direction from the LightManager > sceneContext)
	 *
	 * 4. Update Bones
	*/



	const auto lightWVPMatrix{ XMLoadFloat4x4(&pModel->GetTransform()->GetWorld()) *
							 XMLoadFloat4x4(&ShadowMapRenderer::Get()->GetLightVP()) };

	XMFLOAT4X4 lightWVP;
	XMStoreFloat4x4(&lightWVP, lightWVPMatrix);


	SetVariable_Matrix(L"gWorldViewProj_Light", reinterpret_cast<const float*>(&lightWVP));


	const auto pShadowMapRenderer = ShadowMapRenderer::Get();

	SetVariable_Texture(L"gShadowMap", pShadowMapRenderer->GetShadowMap());



	SetVariable_Vector(L"gLightDirection", sceneContext.pLights->GetDirectionalLight().direction);
	ModelAnimator* pAnimator{ pModel->GetAnimator() };

	ASSERT_NULL_(pAnimator);

	const auto& bones = pAnimator->GetBoneTransforms();

	SetVariable_MatrixArray(L"gBones", reinterpret_cast<const float*>(bones.data()), static_cast<UINT>(bones.size()));

	//Update Shadow Variables
	//const auto pShadowMapRenderer = ShadowMapRenderer::Get();
	//...
}
