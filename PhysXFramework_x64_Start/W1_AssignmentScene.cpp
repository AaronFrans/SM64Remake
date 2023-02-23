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
	Right
};


void W1_AssignmentScene::Initialize()
{
	EnablePhysxDebugRendering(true);
	const auto pPhysX = PhysxManager::GetInstance()->GetPhysics();
	const PxMaterial* pPhysXMat = pPhysX->createMaterial(0.7f, 0.7f, 0.2f);


	const XMFLOAT3 cubeDims{ 70, 0.1f, 70 };

	//floor
	m_pFloor = new CubePosColorNorm{ cubeDims.x, cubeDims.y, cubeDims.z };
	AddGameObject(m_pFloor);

	//floor Actor
	PxRigidStatic* pCubeActor = pPhysX->createRigidStatic(PxTransform{ PxIdentity });
	PxBoxGeometry boxGeomitry = PxBoxGeometry{ cubeDims.x / 2, cubeDims.y / 2, cubeDims.z / 2 };

	PxRigidActorExt::createExclusiveShape(*pCubeActor, boxGeomitry, *pPhysXMat);

	//Link floor with actor
	m_pFloor->AttachRigidActor(pCubeActor);



	const float ballRadius{ 1 };
	//Ball
	m_pBall = new SpherePosColorNorm{ ballRadius, 30, 30, XMFLOAT4{Colors::Azure} };
	AddGameObject(m_pBall);
	m_pBall->Translate(0, 10, 0);

	//Ball Actor
	m_pBallRigid = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	PxSphereGeometry ballGeomitry = PxSphereGeometry{ ballRadius };

	PxRigidActorExt::createExclusiveShape(*m_pBallRigid, ballGeomitry, *pPhysXMat);

	//Link floor with actor
	m_pBall->AttachRigidActor(m_pBallRigid);



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

}

void W1_AssignmentScene::Update()
{


	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::pressed, 'R'))
		m_pFloor->Translate(0, 10, 0);

	const float moveSpeed{ 500 * m_SceneContext.GetGameTime()->GetElapsed() };

	if (m_SceneContext.GetInput()->IsActionTriggered(Left))
		m_pBallRigid->addTorque({ -moveSpeed, 0, 0 });
	if (m_SceneContext.GetInput()->IsActionTriggered(Right))
		m_pBallRigid->addTorque({ moveSpeed, 0, 0 });

	if (m_SceneContext.GetInput()->IsActionTriggered(Up))
		m_pBallRigid->addTorque({ 0, 0 , moveSpeed });
	if (m_SceneContext.GetInput()->IsActionTriggered(Down))
		m_pBallRigid->addTorque({ 0, 0 ,-moveSpeed, });



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
