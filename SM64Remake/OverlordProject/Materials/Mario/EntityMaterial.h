#pragma once

class EntityMaterial final : public Material<EntityMaterial>
{
public:
	EntityMaterial();
	~EntityMaterial() override = default;

	EntityMaterial(const EntityMaterial& other) = delete;
	EntityMaterial(EntityMaterial&& other) noexcept = delete;
	EntityMaterial& operator=(const EntityMaterial& other) = delete;
	EntityMaterial& operator=(EntityMaterial&& other) noexcept = delete;


	void SetDiffuseTexture(const std::wstring& assetFile);

	void SetOpacityTexture(const std::wstring& assetFile);

protected:
	void InitializeEffectVariables() override; 
private:
	TextureData* m_pDiffuseTexture{ nullptr };
	TextureData* m_pOpacityTexture{ nullptr };
};
