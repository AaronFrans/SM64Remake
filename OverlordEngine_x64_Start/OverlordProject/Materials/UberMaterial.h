#pragma once
class UberMaterial : public Material<UberMaterial>
{
public:
	UberMaterial();
	~UberMaterial() override = default;

	UberMaterial(const UberMaterial& other) = delete;
	UberMaterial(UberMaterial&& other) noexcept = delete;
	UberMaterial& operator=(const UberMaterial& other) = delete;
	UberMaterial& operator=(UberMaterial&& other) noexcept = delete;

	//Light
	void SetLightDirection(XMFLOAT3 direction);

	// Diffuse
	void SetDiffuseTexture(const std::wstring& assetFile);
	void SetUseDiffuseTexture(const bool useTexture);
	void SetDiffuseColor(const XMFLOAT4 color);

	//Specular
	void SetSpecularColor(const XMFLOAT4 color);
	void SetSpecularTexture(const std::wstring& assetFile);
	void SetUseSpecularTexture(const bool useTexture);
	void SetShininess(const int shininess);

	//Ambiant
	void SetAmbiantColor(const XMFLOAT4 color);
	void SetAmbiantIntensity(const int ambientIntensity);


	//Normal maps
	void SetFlipGreenChannel(const bool flipChannel);
	void SetUseNormalMaps(const bool useNormalMap);
	void SetNormalTexture(const std::wstring& assetFile);

	//Environment Mapping
	void SetCubeEnvironment(const std::wstring& assetFile);
	void SetUseEnvironmentMaps(const bool useEnvironmentMap);
	void SetReflectionStrength(const float reflectionStrength);
	void SetRefractionStrength(const float refractionStrength);
	void SetRefractionIndex(const float refractionIndex);

	//Fresnel Falloff
	void SetUseFresnelFalloff(const bool useFresnelFalloff);
	void SetFresnelColor(const XMFLOAT4 color);
	void SetFresnelStrength(const float fresnelStrength);
	void SetFresnelMultiplier(const float fresnelMultiplier);
	void SetFresnelHardness(const float fresnelHardness);

	//Opacity
	void SetOpacityIntensity(const float opacityIntensity);
	void SetUseOpacity(const bool useOpacity);
	void SetOpacityTexture(const std::wstring& assetFile);


	//Specular Models
	void SetUseSpecularBlinn(const bool useSpecularBlinn);
	void SetUseSpecularPhong(const bool useSpecularPhong);


protected:
	void InitializeEffectVariables() override;

private:
	TextureData* m_pDiffuseTexture{ nullptr };
	TextureData* m_pSpecularTexture{ nullptr };
	TextureData* m_pNormalTexture{ nullptr };
	TextureData* m_pEnvironmentTexture{ nullptr };
	TextureData* m_pOpacityTexture{ nullptr };
};