#pragma once
#include "GameScene.h"
class W1_AssignmentScene final : public GameScene
{
public:
	W1_AssignmentScene() :GameScene(L"TestScene") {}
	~W1_AssignmentScene() override = default;
	W1_AssignmentScene(const W1_AssignmentScene& other) = delete;
	W1_AssignmentScene(W1_AssignmentScene&& other) noexcept = delete;
	W1_AssignmentScene& operator=(const W1_AssignmentScene& other) = delete;
	W1_AssignmentScene& operator=(W1_AssignmentScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() const override;
	void OnSceneActivated() override;
	void OnSceneDeactivated() override;

private:

	GameObject* m_pFloor{ nullptr };

	GameObject* m_pBall{ nullptr };
	PxRigidDynamic* m_pBallRigid{ nullptr };


};

