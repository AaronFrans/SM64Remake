#include "stdafx.h"
#include "FlowingWater.h"

FlowingWater::FlowingWater() :
	Material<FlowingWater>(L"Effects/Mario/MovingWater.fx")
{
}

#pragma region Light
void FlowingWater::SetLightDirection(const XMFLOAT3 direction)
{
	SetVariable_Vector(L"gLightDirection", direction);
}
#pragma endregion

#pragma region Diffuse
void FlowingWater::SetUseDiffuseTexture(const bool useTexture)
{
	SetVariable_Scalar(L"gUseTextureDiffuse", useTexture);
}

void FlowingWater::SetDiffuseTexture(const std::wstring& assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);

	SetVariable_Texture(L"gTextureDiffuse", m_pDiffuseTexture);
}

void FlowingWater::SetDiffuseColor(const XMFLOAT4 color)
{
	SetVariable_Vector(L"gColorDiffuse", color);
}
#pragma endregion

#pragma region Specular
void FlowingWater::SetSpecularColor(const XMFLOAT4 color)
{
	SetVariable_Vector(L"gColorSpecular", color);
}

void FlowingWater::SetSpecularTexture(const std::wstring& assetFile)
{
	m_pSpecularTexture = ContentManager::Load<TextureData>(assetFile);

	SetVariable_Texture(L"gTextureSpecularIntensity", m_pDiffuseTexture);
}

void FlowingWater::SetUseSpecularTexture(const bool useTexture)
{
	SetVariable_Scalar(L"gUseTextureSpecularIntensity", useTexture);
}

void FlowingWater::SetShininess(const int shininess)
{
	SetVariable_Scalar(L"gShininess", shininess);
}
#pragma endregion

#pragma region Ambiant
void FlowingWater::SetAmbiantColor(const XMFLOAT4 color)
{
	SetVariable_Vector(L"gColorAmbient", color);
}
void FlowingWater::SetAmbiantIntensity(const int ambientIntensity)
{
	SetVariable_Scalar(L"gAmbientIntensity", ambientIntensity);
}
#pragma endregion

#pragma region Normal Maps
void FlowingWater::SetFlipGreenChannel(const bool flipChannel)
{
	SetVariable_Scalar(L"gFlipGreenChannel", flipChannel);
}
void FlowingWater::SetUseNormalMaps(const bool useNormalMap)
{
	SetVariable_Scalar(L"gUseTextureNormal", useNormalMap);
}
void FlowingWater::SetNormalTexture(const std::wstring& assetFile)
{
	m_pNormalTexture = ContentManager::Load<TextureData>(assetFile);

	SetVariable_Texture(L"gTextureNormal", m_pNormalTexture);
}
#pragma endregion

#pragma region Environment Mapping
void FlowingWater::SetCubeEnvironment(const std::wstring& assetFile)
{
	m_pEnvironmentTexture = ContentManager::Load<TextureData>(assetFile);

	SetVariable_Texture(L"gCubeEnvironment", m_pEnvironmentTexture);
}

void FlowingWater::SetUseEnvironmentMaps(const bool useEnvironmentMap)
{
	SetVariable_Scalar(L"gUseEnvironmentMapping", useEnvironmentMap);
}

void FlowingWater::SetReflectionStrength(const float reflectionStrength)
{
	SetVariable_Scalar(L"gReflectionStrength", reflectionStrength);
}

void FlowingWater::SetRefractionStrength(const float refractionStrength)
{
	SetVariable_Scalar(L"gRefractionStrength", refractionStrength);
}

void FlowingWater::SetRefractionIndex(const float refractionIndex)
{
	SetVariable_Scalar(L"gRefractionIndex", refractionIndex);
}
#pragma endregion

#pragma region Fresnel Fallof
void FlowingWater::SetUseFresnelFalloff(const bool useFresnelFalloff)
{
	SetVariable_Scalar(L"gUseFresnelFalloff", useFresnelFalloff);
}

void FlowingWater::SetFresnelColor(const XMFLOAT4 color)
{
	SetVariable_Vector(L"gColorFresnel", color);
}

void FlowingWater::SetFresnelStrength(const float fresnelStrength)
{
	SetVariable_Scalar(L"gFresnelPower", fresnelStrength);
}

void FlowingWater::SetFresnelMultiplier(const float fresnelMultiplier)
{
	SetVariable_Scalar(L"gFresnelMultiplier", fresnelMultiplier);
}

void FlowingWater::SetFresnelHardness(const float fresnelHardness)
{
	SetVariable_Scalar(L"gFresnelHardness", fresnelHardness);
}
#pragma endregion

#pragma region Opacity
void FlowingWater::SetOpacityIntensity(const float opacityIntensity)
{
	SetVariable_Scalar(L"gOpacityIntensity", opacityIntensity);
}
void FlowingWater::SetUseOpacity(const bool useOpacity)
{
	SetVariable_Scalar(L"gTextureOpacityIntensity", useOpacity);
}
void FlowingWater::SetOpacityTexture(const std::wstring& assetFile)
{
	m_pOpacityTexture = ContentManager::Load<TextureData>(assetFile);

	SetVariable_Texture(L"gTextureOpacity", m_pOpacityTexture);
}
#pragma endregion

#pragma region Specular Models
void FlowingWater::SetUseSpecularBlinn(const bool useSpecularBlinn)
{
	SetVariable_Scalar(L"gUseSpecularPhong", useSpecularBlinn);
}
void FlowingWater::SetUseSpecularPhong(const bool useSpecularPhong)
{
	SetVariable_Scalar(L"gUseSpecularPhong", useSpecularPhong);
}

void FlowingWater::SetFlowDir(const XMFLOAT2& dir)
{

	SetVariable_Vector(L"gFlowDirection", { dir.x, dir.y });
}

void FlowingWater::SetPerlinValues(const std::wstring& assetFile, float scale, float amplitude)
{
	m_pPerlinTexture = ContentManager::Load<TextureData>(assetFile);

	SetVariable_Texture(L"gRandomnessTexture", m_pPerlinTexture);

	SetVariable_Scalar(L"gPerlinScale", scale);

	SetVariable_Scalar(L"gPerlinAmplitude", amplitude);
	SetVariable_Scalar(L"gUsePerlin", true);

}
#pragma endregion

void FlowingWater::InitializeEffectVariables()
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

void FlowingWater::OnUpdateModelVariables(const SceneContext& sceneContext, const ModelComponent*) const
{
	SetVariable_Scalar(L"gTime", sceneContext.pGameTime->GetTotal());
}
