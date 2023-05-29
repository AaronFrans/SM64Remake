#pragma once
class ParticleEmmiter;
class Coin final : public GameObject
{
public:
	Coin(physx::PxMaterial* physicsMaterial, unsigned* coinCounter, std::vector<Coin*>& owningCollection);

protected:

	void Initialize(const SceneContext& scene) override;

	void Update(const SceneContext&) override;

	void PostInitialize(const SceneContext& sceneContext) override;

private:
	physx::PxMaterial* m_pPhysicsMaterial;

	GameObject* m_pMario{};
	RigidBodyComponent* m_pRB{};

	const PxVec3 m_CorrectScale{ 20,20,20 };

	UINT m_ColliderId{};

	bool m_WasHit{ false };
	bool m_CanBeHit{ false };

	ParticleEmmiter* m_pEmmiter{ nullptr };

	unsigned* m_pCoinCounter{};

	float m_CoinRotation{ 0 };
	float m_TimeSinceSpawm{ 0 };

	std::vector<Coin*>& m_OwningVec;
};

