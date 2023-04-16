#pragma once
#include "GameScene.h"
class W2_AssignmentScene : public GameScene
{
public:
	W2_AssignmentScene() :GameScene(L"W2_AssignmentScene (SimpleLevel + Triggers + Sound)") {}
	~W2_AssignmentScene() override = default;
	W2_AssignmentScene(const W2_AssignmentScene& other) = delete;
	W2_AssignmentScene(W2_AssignmentScene&& other) noexcept = delete;
	W2_AssignmentScene& operator=(const W2_AssignmentScene& other) = delete;
	W2_AssignmentScene& operator=(W2_AssignmentScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() const override;
	void OnSceneActivated() override;
	void OnSceneDeactivated() override;
	void onTrigger(PxTriggerPair* pairs, PxU32 count) override;

private:


	void ResetScene();
	void ResetDynamicBody(PxRigidDynamic* body);

	GameObject* m_pLevel{ nullptr };
	GameObject* m_pSphere{ nullptr };


	GameObject* m_pHatchLeft{ nullptr };
	GameObject* m_pHatchRight{ nullptr };


	GameObject* m_pBoxLeft{ nullptr };
	GameObject* m_pBoxRight{ nullptr };

	GameObject* m_pSphereTopLeft{ nullptr };
	GameObject* m_pSphereTopRight{ nullptr };


	PxRigidStatic* m_pTriggerLeft{ nullptr };
	PxRigidStatic* m_pTriggerRight{ nullptr };

	FMOD::Sound* m_pSound2D{ nullptr };

	FMOD::Channel* m_pChannel2D{ nullptr };

	bool m_LeftTriggered{ false };
	bool m_RightTriggered{ false };

	bool m_LeftRotating{ false };
	bool m_RightRotating{ false };

	float m_RightAngle{ 0 };
	float m_LeftAngle{ 0 };
	PxVec3 m_RightKinematicPostion{ };
	PxVec3 m_LeftKinematicPostion{ };
};

