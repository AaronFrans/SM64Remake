#pragma once
class Door final :
	public GameObject
{
public:
	Door(physx::PxMaterial* physicsMaterial);

protected:

	void Initialize(const SceneContext& scene) override;

private:
	physx::PxMaterial* m_pPhysicsMaterial;

	const PxVec3 CorrectScale{ 20,20,20 };
};

