#pragma once

class EntityMaterial_Skinned final : public Material<EntityMaterial_Skinned>
{
public:
	EntityMaterial_Skinned();
	~EntityMaterial_Skinned() override = default;

	EntityMaterial_Skinned(const EntityMaterial_Skinned& other) = delete;
	EntityMaterial_Skinned(EntityMaterial_Skinned&& other) noexcept = delete;
	EntityMaterial_Skinned& operator=(const EntityMaterial_Skinned& other) = delete;
	EntityMaterial_Skinned& operator=(EntityMaterial_Skinned&& other) noexcept = delete;


	void SetDiffuseTexture(const std::wstring& assetFile);

	void SetOpacityTexture(const std::wstring& assetFile);

protected:
	void InitializeEffectVariables() override;

	void OnUpdateModelVariables(const SceneContext& /*sceneContext*/, const ModelComponent* pModel) const;
private:
	TextureData* m_pDiffuseTexture{ nullptr };
	TextureData* m_pOpacityTexture{ nullptr };
};
