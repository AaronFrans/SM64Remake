#pragma once
#include "BaseComponent.h"

class MeshFilter;
class ModelAnimator;

class ModelComponent : public BaseComponent
{
public:
	ModelComponent(const std::wstring& assetFile, bool castShadows = true);
	~ModelComponent() override;

	ModelComponent(const ModelComponent& other) = delete;
	ModelComponent(ModelComponent&& other) noexcept = delete;
	ModelComponent& operator=(const ModelComponent& other) = delete;
	ModelComponent& operator=(ModelComponent&& other) noexcept = delete;

	void SetMaterial(BaseMaterial* pMaterial, int submeshId = -1);
	void SetMaterial(UINT materialId, int submeshId = -1);

	ModelAnimator* GetAnimator() const { return m_pAnimator; }
	bool HasAnimator() const { return m_pAnimator != nullptr; }

	void DrawPost(bool value) {
		m_DrawPost = value;
		m_enablePostDraw = value;
	};

	void DrawShadows(bool value) { m_CastShadows = value; };

	bool IsAcive() { return m_IsActive; };
	void SetActive(bool isActive) { m_IsActive = isActive; };

protected:
	void Initialize(const SceneContext& sceneContext) override;
	void Update(const SceneContext&) override;
	void Draw(const SceneContext& sceneContext) override;
	void PostDraw(const SceneContext& sceneContext) override;


	void InnerDraw(const SceneContext& sceneContext);

	void ShadowMapDraw(const SceneContext& sceneContext) override; //update_W9

private:
	std::wstring m_AssetFile{};
	MeshFilter* m_pMeshFilter{};

	std::vector<BaseMaterial*> m_Materials{};
	BaseMaterial* m_pDefaultMaterial{};

	bool m_MaterialChanged{};

	ModelAnimator* m_pAnimator{};

	//W9
	bool m_CastShadows{ true };

	bool m_DrawPost{ false };

	bool m_IsActive{ true };
};
