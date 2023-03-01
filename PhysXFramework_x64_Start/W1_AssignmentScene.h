#pragma once
#include "GameScene.h"



class W1_AssignmentScene final : public GameScene
{
public:
	W1_AssignmentScene() :GameScene(L"W1_AssignmentScene") {}
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

	std::vector<GameObject*> m_pCubeWall{};
	std::vector<PxRigidDynamic*> m_pCubeWallDynamics{};

	GameObject* m_pFloor{ nullptr };

	GameObject* m_pBall{ nullptr };
	PxRigidDynamic* m_pBallDynamic{ nullptr };

	const int m_NrBlocksPerRow{ 5 };
	const int m_NrBlocks{ m_NrBlocksPerRow * m_NrBlocksPerRow };

	const float m_BallRadius{ 2 };

	const float m_BlockDimension{ 2 };
	const XMFLOAT3 m_BlockDims{ m_BlockDimension, m_BlockDimension, m_BlockDimension };

	void SetBallPos();
	void SetupWallPos();
};

