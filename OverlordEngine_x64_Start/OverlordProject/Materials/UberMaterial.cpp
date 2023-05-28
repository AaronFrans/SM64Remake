#include "stdafx.h"
#include "UberMaterial.h"

UberMaterial::UberMaterial() : 
	Material<UberMaterial>(L"Effects/GP2_W3X-2_UberShader.fx")
{
}

#pragma region Light
void UberMaterial::SetLightDirection(const XMFLOAT3 direction)
{
	SetVariable_Vector(L"gLightDirection", direction);
}
#pragma endregion

#pragma region Diffuse
void UberMaterial::SetUseDiffuseTexture(const bool useTexture)
{
	SetVariable_Scalar(L"gUseTextureDiffuse", useTexture);
}

void UberMaterial::SetDiffuseTexture(const std::wstring& assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);

	SetVariable_Texture(L"gTextureDiffuse", m_pDiffuseTexture);
}

void UberMaterial::SetDiffuseColor(const XMFLOAT4 color)
{
	SetVariable_Vector(L"gColorDiffuse", color);
}
#pragma endregion

#pragma region Specular
void UberMaterial::SetSpecularColor(const XMFLOAT4 color)
{
	SetVariable_Vector(L"gColorSpecular", color);
}

void UberMaterial::SetSpecularTexture(const std::wstring& assetFile)
{
	m_pSpecularTexture = ContentManager::Load<TextureData>(assetFile);

	SetVariable_Texture(L"gTextureSpecularIntensity", m_pDiffuseTexture);
}

void UberMaterial::SetUseSpecularTexture(const bool useTexture)
{
	SetVariable_Scalar(L"gUseTextureSpecularIntensity", useTexture);
}

void UberMaterial::SetShininess(const int shininess)
{
	SetVariable_Scalar(L"gShininess", shininess);
}
#pragma endregion

#pragma region Ambiant
void UberMaterial::SetAmbiantColor(const XMFLOAT4 color)
{
	SetVariable_Vector(L"gColorAmbient", color);
}
void UberMaterial::SetAmbiantIntensity(const int ambientIntensity)
{
	SetVariable_Scalar(L"gAmbientIntensity", ambientIntensity);
}
#pragma endregion

#pragma region Normal Maps
void UberMaterial::SetFlipGreenChannel(const bool flipChannel)
{
	SetVariable_Scalar(L"gFlipGreenChannel", flipChannel);
}
void UberMaterial::SetUseNormalMaps(const bool useNormalMap)
{
	SetVariable_Scalar(L"gUseTextureNormal", useNormalMap);
}
void UberMaterial::SetNormalTexture(const std::wstring& assetFile)
{
	m_pNormalTexture = ContentManager::Load<TextureData>(assetFile);

	SetVariable_Texture(L"gTextureNormal", m_pNormalTexture);
}
#pragma endregion

#pragma region Environment Mapping
void UberMaterial::SetCubeEnvironment(const std::wstring& assetFile)
{
	m_pEnvironmentTexture = ContentManager::Load<TextureData>(assetFile);

	SetVariable_Texture(L"gCubeEnvironment", m_pEnvironmentTexture);
}

void UberMaterial::SetUseEnvironmentMaps(const bool useEnvironmentMap)
{
	SetVariable_Scalar(L"gUseEnvironmentMapping", useEnvironmentMap);
}

void UberMaterial::SetReflectionStrength(const float reflectionStrength)
{
	SetVariable_Scalar(L"gReflectionStrength", reflectionStrength);
}

void UberMaterial::SetRefractionStrength(const float refractionStrength)
{
	SetVariable_Scalar(L"gRefractionStrength", refractionStrength);
}

void UberMaterial::SetRefractionIndex(const float refractionIndex)
{
	SetVariable_Scalar(L"gRefractionIndex", refractionIndex);
}
#pragma endregion

#pragma region Fresnel Fallof
void UberMaterial::SetUseFresnelFalloff(const bool useFresnelFalloff)
{
	SetVariable_Scalar(L"gUseFresnelFalloff", useFresnelFalloff);
}

void UberMaterial::SetFresnelColor(const XMFLOAT4 color)
{
	SetVariable_Vector(L"gColorFresnel", color);
}

void UberMaterial::SetFresnelStrength(const float fresnelStrength)
{
	SetVariable_Scalar(L"gFresnelPower", fresnelStrength);
}

void UberMaterial::SetFresnelMultiplier(const float fresnelMultiplier)
{
	SetVariable_Scalar(L"gFresnelMultiplier", fresnelMultiplier);
}

void UberMaterial::SetFresnelHardness(const float fresnelHardness)
{
	SetVariable_Scalar(L"gFresnelHardness", fresnelHardness);
}
#pragma endregion

#pragma region Opacity
void UberMaterial::SetOpacityIntensity(const float opacityIntensity)
{
	SetVariable_Scalar(L"gOpacityIntensity", opacityIntensity);
}
void UberMaterial::SetUseOpacity(const bool useOpacity)
{
	SetVariable_Scalar(L"gTextureOpacityIntensity", useOpacity);
}
void UberMaterial::SetOpacityTexture(const std::wstring& assetFile)
{
	m_pOpacityTexture = ContentManager::Load<TextureData>(assetFile);

	SetVariable_Texture(L"gTextureOpacity", m_pOpacityTexture);
}
#pragma endregion

#pragma region Specular Models
void UberMaterial::SetUseSpecularBlinn(const bool useSpecularBlinn)
{
	SetVariable_Scalar(L"gUseSpecularPhong", useSpecularBlinn);
}
void UberMaterial::SetUseSpecularPhong(const bool useSpecularPhong)
{
	SetVariable_Scalar(L"gUseSpecularPhong", useSpecularPhong);
}
#pragma endregion

void UberMaterial::InitializeEffectVariables()
{

	SetTechnique(L"WithAlphaBlending");

	//Ligth
	SetVariable_Vector(L"gLightDirection", { 0.577f, 0.577f, -0.577f });

	//Diffuse

	m_pDiffuseTexture = ContentManager::Load<TextureData>(L"Textures/UberShader/Diffuse.dds");

	SetVariable_Texture(L"gTextureDiffuse", m_pDiffuseTexture);

	SetVariable_Scalar(L"gUseTextureDiffuse", true);

	SetVariable_Vector(L"gColorDiffuse", { 1, 1, 1, 1 });

	//Specular

	m_pSpecularTexture = ContentManager::Load<TextureData>(L"Textures/UberShader/Specular.dds");

	SetVariable_Texture(L"gTextureSpecularIntensity", m_pSpecularTexture);

	SetVariable_Vector(L"gColorSpecular", { 1, 1, 1, 1 });

	SetVariable_Scalar(L"gUseTextureSpecularIntensity", false);

	SetVariable_Scalar(L"gShininess", 15);

	//Ambiant
	SetVariable_Vector(L"gColorAmbient", { 0, 0, 0, 1 });

	SetVariable_Scalar(L"gAmbientIntensity", 0);
	
	//NormalMaps

	m_pNormalTexture = ContentManager::Load<TextureData>(L"Textures/UberShader/Normal.dds");

	SetVariable_Texture(L"gTextureNormal", m_pNormalTexture);


	SetVariable_Scalar(L"gFlipGreenChannel", false);

	SetVariable_Scalar(L"gUseTextureNormal", false);

	//Environment Mapping

	m_pEnvironmentTexture = ContentManager::Load<TextureData>(L"Textures/UberShader/Cubemap.dds");

	SetVariable_Texture(L"gCubeEnvironment", m_pEnvironmentTexture);

	SetVariable_Scalar(L"gUseEnvironmentMapping", false);

	SetVariable_Scalar(L"gReflectionStrength", 0.0f);

	SetVariable_Scalar(L"gRefractionStrength", 0.0f);

	SetVariable_Scalar(L"gRefractionIndex", 0.3f);

	//Fresnel Falloff
	SetVariable_Scalar(L"gUseFresnelFalloff", false);

	SetVariable_Vector(L"gColorFresnel", { 0, 0, 0, 1 });

	//Opacity
	m_pOpacityTexture = ContentManager::Load<TextureData>(L"Textures/UberShader/Oppacity.dds");

	SetVariable_Texture(L"gTextureOpacity", m_pOpacityTexture);


	SetVariable_Scalar(L"gOpacityIntensity", 1.0f);

	SetVariable_Scalar(L"gTextureOpacityIntensity", false);

	//Specular Models
	SetVariable_Scalar(L"gUseSpecularPhong", false);

	SetVariable_Scalar(L"gUseSpecularBlinn", false);

}

void UberMaterial::OnUpdateModelVariables(const SceneContext& sceneContext, const ModelComponent* pModel) const
{
	const auto lightWVPMatrix{ XMLoadFloat4x4(&pModel->GetTransform()->GetWorld()) *
							 XMLoadFloat4x4(&ShadowMapRenderer::Get()->GetLightVP()) };

	XMFLOAT4X4 lightWVP;
	XMStoreFloat4x4(&lightWVP, lightWVPMatrix);


	SetVariable_Matrix(L"gWorldViewProj_Light", reinterpret_cast<const float*>(&lightWVP));


	const auto pShadowMapRenderer = ShadowMapRenderer::Get();

	SetVariable_Texture(L"gShadowMap", pShadowMapRenderer->GetShadowMap());



	SetVariable_Vector(L"gLightDirection", sceneContext.pLights->GetDirectionalLight().direction);
}
