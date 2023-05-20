#pragma once
class Goomba final : public GameObject
{
public:

	Goomba(physx::PxMaterial* physicsMaterial);

protected:

	void Initialize(const SceneContext& scene) override;

	void Update(const SceneContext&) override;

private:


};

