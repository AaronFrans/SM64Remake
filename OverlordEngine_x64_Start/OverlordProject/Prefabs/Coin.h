#pragma once
class Coin final : public GameObject
{
public:
	Coin(physx::PxMaterial* physicsMaterial, GameObject* mario);

	bool WasHit() { return m_WasHit; };

protected:

	void Initialize(const SceneContext& scene) override;

	void PostInitialize(const SceneContext& sceneContext) override;

private:
	physx::PxMaterial* m_pPhysicsMaterial;

	GameObject* m_pMario{};
	RigidBodyComponent* m_pRB{};

	const PxVec3 m_CorrectScale{ 20,20,20 };

	UINT m_ColliderId{};

	bool m_WasHit{false};
};

