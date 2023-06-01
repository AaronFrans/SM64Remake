#pragma once
#pragma once

class ThirdPersonCharacter;
class ParticleEmmiter;
class MarioScene;
class Goomba final : public GameObject
{
public:

	Goomba(physx::PxMaterial* physicsMaterial, ThirdPersonCharacter* pMario, std::vector<Goomba*>& owningCollection);


	void SetDeath();

protected:

	void Initialize(const SceneContext& scene) override;

	void Update(const SceneContext&) override;

private:

	void MakeParticleEmmiter();

	physx::PxMaterial* m_pPhysxMat{};

	MarioScene* m_OwningScene{};

	ControllerComponent* m_pController{};
	GameObject* m_pHitBox{};
	GameObject* m_pMesh{};

	ThirdPersonCharacter* m_pMario{};

	ParticleEmmiter* m_pEmmiter{ nullptr };

	float m_YRot{ 0 };

	float m_TimeSinceSpawm{ 0 };

	float m_MoveSpeed{ 3 };

	bool m_CanBeHit{ false };
	bool m_HasDied{ false };
	std::vector<Goomba*>& m_OwningVec;
};

