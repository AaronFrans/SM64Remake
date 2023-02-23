#include "stdafx.h"
#include "W1_AssignmentScene.h"

#include "CubePosColorNorm.h"
#include "SpherePosColorNorm.h"
#include "CylinderPosColorNorm.h"

#include "Logger.h"

enum InputIds
{
	Up,
	Down,
	Left,
	Right,
	Reset
};


void W1_AssignmentScene::Initialize()
{
	EnablePhysxDebugRendering(true);
	const auto pPhysX = PhysxManager::GetInstance()->GetPhysics();
	const PxMaterial* pMatFloor = pPhysX->createMaterial(0.7f, 0.7f, 0.2f);
	const PxMaterial* pMatBall = pPhysX->createMaterial(0.7f, 0.7f, 0.2f);
	const PxMaterial* pMatBlock = pPhysX->createMaterial(0.7f, 0.7f, 0.2f);


	const XMFLOAT3 cubeDims{ 70, 0.1f, 70 };

	//floor
	m_pFloor = new CubePosColorNorm{ cubeDims.x, cubeDims.y, cubeDims.z };
	AddGameObject(m_pFloor);
	m_pFloor->Translate(0, -cubeDims.y * 2, 0);

	//floor Actor
	PxRigidStatic* pCubeActor = pPhysX->createRigidStatic(PxTransform{ PxIdentity });
	PxBoxGeometry boxGeomitry = PxBoxGeometry{ cubeDims.x / 2, cubeDims.y / 2, cubeDims.z / 2 };

	PxRigidActorExt::createExclusiveShape(*pCubeActor, boxGeomitry, *pMatFloor);

	//Link floor with actor
	m_pFloor->AttachRigidActor(pCubeActor);




	//Ball
	m_pBall = new SpherePosColorNorm{ m_BallRadius, 30, 30, XMFLOAT4{Colors::Azure} };
	AddGameObject(m_pBall);
	SetBallPos();

	//Ball Actor
	m_pBallDynamic = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	m_pBallDynamic->setMass(10);
	PxSphereGeometry ballGeomitry = PxSphereGeometry{ m_BallRadius };

	PxRigidActorExt::createExclusiveShape(*m_pBallDynamic, ballGeomitry, *pMatBall);

	//Link floor with actor
	m_pBall->AttachRigidActor(m_pBallDynamic);

	//Wall
	PxRigidDynamic* pBlockDynamic{ nullptr };
	PxBoxGeometry blockGeomitry = PxBoxGeometry{ m_BlockDims.x / 2.f, m_BlockDims.x / 2.f, m_BlockDims.z / 2.f };
	for (int curBlock{ 0 }; curBlock < m_NrBlocks; curBlock++)
	{
		//Block
		m_pCubeWall.push_back(new CubePosColorNorm{ m_BlockDims.x, m_BlockDims.y, m_BlockDims.z });
		AddGameObject(m_pCubeWall[curBlock]);

		//Block Actor
		pBlockDynamic = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });

		pBlockDynamic->setMass(0.5f);

		PxRigidActorExt::createExclusiveShape(*pBlockDynamic, blockGeomitry, *pMatBlock);


		m_pCubeWall[curBlock]->AttachRigidActor(pBlockDynamic);
	}

	SetupWallPos();



	//Inputs
	m_SceneContext.GetInput()->AddInputAction(InputAction
		{
		  Down, InputTriggerState::down, VK_DOWN, -1,
		  XINPUT_GAMEPAD_DPAD_DOWN
		});
	m_SceneContext.GetInput()->AddInputAction(InputAction
		{
		  Up, InputTriggerState::down, VK_UP, -1,
		  XINPUT_GAMEPAD_DPAD_UP
		});
	m_SceneContext.GetInput()->AddInputAction(InputAction
		{
		  Left, InputTriggerState::down, VK_LEFT, -1,
		  XINPUT_GAMEPAD_DPAD_LEFT
		});
	m_SceneContext.GetInput()->AddInputAction(InputAction
		{
		  Right, InputTriggerState::down, VK_RIGHT, -1,
		  XINPUT_GAMEPAD_DPAD_RIGHT
		});
	m_SceneContext.GetInput()->AddInputAction(InputAction
		{
		  Reset, InputTriggerState::down, 'R', -1,
		  XINPUT_GAMEPAD_A
		});
}

void W1_AssignmentScene::Update()
{


	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::pressed, 'R'))
		m_pFloor->Translate(0, 10, 0);
	const float moveSpeedBase{ 2000 };
	const float moveSpeed{ moveSpeedBase * m_SceneContext.GetGameTime()->GetElapsed() };
	XMFLOAT3 camForward = m_SceneContext.GetCamera()->GetForward();
	XMFLOAT3 camRigth = m_SceneContext.GetCamera()->GetRight();



	if (m_SceneContext.GetInput()->IsActionTriggered(Left))
		m_pBallDynamic->addForce(PxVec3{ -camRigth.x * moveSpeed, -camRigth.y * moveSpeed, -camRigth.z * moveSpeed });
	if (m_SceneContext.GetInput()->IsActionTriggered(Right))
		m_pBallDynamic->addForce(PxVec3{ camRigth.x * moveSpeed, camRigth.y * moveSpeed, camRigth.z * moveSpeed });

	if (m_SceneContext.GetInput()->IsActionTriggered(Up))
		m_pBallDynamic->addForce(PxVec3{ camForward.x * moveSpeed, camForward.y * moveSpeed, camForward.z * moveSpeed });
	if (m_SceneContext.GetInput()->IsActionTriggered(Down))
		m_pBallDynamic->addForce(PxVec3{ -camForward.x * moveSpeed, -camForward.y * moveSpeed, -camForward.z * moveSpeed });


	if (m_SceneContext.GetInput()->IsActionTriggered(Reset))
	{
		SetBallPos();
		SetupWallPos();
	}
}

void W1_AssignmentScene::Draw() const
{

}

void W1_AssignmentScene::OnSceneActivated()
{
	Logger::GetInstance()->LogFormat(LogLevel::Info, L"Scene Activated > \"%ls\"", GetName().c_str());
}

void W1_AssignmentScene::OnSceneDeactivated()
{
}

void W1_AssignmentScene::SetBallPos()
{
	const float ballZPos{ -20 };
	const float ballYPos{ m_BallRadius * 2 + 0.5f };

	m_pBall->Translate(0, ballYPos, ballZPos);
}

void W1_AssignmentScene::SetupWallPos()
{
	const float xOffset{ 0.5f };
	const float yOffset{ 1 };

	for (int curBlock{ 0 }; curBlock < m_NrBlocks; curBlock++)
	{
		const int row = curBlock / m_NrBlocksPerRow;
		const int col = curBlock % m_NrBlocksPerRow;

		m_pCubeWall[row * m_NrBlocksPerRow + col]->Translate(row * (m_BlockDims.x + xOffset), (col * (m_BlockDims.y + yOffset)), m_BlockDims.y * 2);

		const float YRotation{ static_cast<float>(rand() % 30) };

		m_pCubeWall[row * m_NrBlocksPerRow + col]->RotateDegrees(0, YRotation, 0);
	}
}
