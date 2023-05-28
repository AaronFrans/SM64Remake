#pragma once
class Goomba final : public GameObject
{
public:

	Goomba(physx::PxMaterial* physicsMaterial);

protected:

	void Initialize(const SceneContext& scene) override;

	void Update(const SceneContext&) override;

private:

	physx::PxMaterial* m_pPhysxMat{};

	ControllerComponent* m_pController;

	float m_YRot{ 0 };

	float m_MoveSpeed{ 10 };

};

