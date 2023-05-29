#pragma once
#pragma once

class ThirdPersonCharacter;
class Goomba final : public GameObject
{
public:

	Goomba(physx::PxMaterial* physicsMaterial, ThirdPersonCharacter* pMario, std::vector<Goomba*>& owningCollection);


	void SetDeath();

protected:

	void Initialize(const SceneContext& scene) override;

	void Update(const SceneContext&) override;

private:

	physx::PxMaterial* m_pPhysxMat{};

	ControllerComponent* m_pController{};
	GameObject* m_pHitBox{};

	ThirdPersonCharacter* m_pMario{};

	float m_YRot{ 0 };

	float m_MoveSpeed{ 3 };

	bool m_CanBeHit{ false };
	bool m_HasDied{ false };
	std::vector<Goomba*>& m_OwningVec;
};

