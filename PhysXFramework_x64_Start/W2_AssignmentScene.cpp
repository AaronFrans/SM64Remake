#include "stdafx.h"
#include "W2_AssignmentScene.h"

#include "ContentManager.h"
#include "MeshObject.h"
#include "SpherePosColorNorm.h"
#include "CubePosColorNorm.h"

#include "Logger.h"
#include "SoundManager.h"

enum InputIds
{
	Left,
	Right,
	Reset
};


void W2_AssignmentScene::Initialize()
{
	EnablePhysxDebugRendering(true);
	const auto pPhysX = PhysxManager::GetInstance()->GetPhysics();
	const PxMaterial* pLevelMat = pPhysX->createMaterial(0.5, 0.5, 0.2f);
	const PxMaterial* pFloorMat = pPhysX->createMaterial(0.5, 0.5, 0.2f);
	const PxMaterial* pBoxMat = pPhysX->createMaterial(0, 0, 0.2f);
	const PxMaterial* pBallMat = pPhysX->createMaterial(0.5, 0.5, 0.2f);

	auto pGround = pPhysX->createRigidStatic(PxTransform{ PxQuat{PxPiDivTwo, PxVec3{0,0,1}} });
	PxRigidActorExt::createExclusiveShape(*pGround, PxPlaneGeometry{}, *pFloorMat);
	m_pPhysxScene->addActor(*pGround);


	//Level
	m_pLevel = new MeshObject(L"Resources/Meshes/Level.ovm");


	AddGameObject(m_pLevel);

	const auto pLevelMesh = ContentManager::GetInstance()->Load<PxTriangleMesh>(L"Resources/Meshes/Level.ovpt");
	const auto pLevelActor = pPhysX->createRigidStatic(PxTransform{ PxIdentity });

	PxRigidActorExt::createExclusiveShape(*pLevelActor, PxTriangleMeshGeometry{ pLevelMesh }, *pLevelMat);

	m_pLevel->AttachRigidActor(pLevelActor);


	//Boxes
	const XMFLOAT3 boxSizes{ 1.5f, 1.5f, 1.5f };

	//BoxLeft
	m_pBoxLeft = new CubePosColorNorm(boxSizes.x, boxSizes.y, boxSizes.z, XMFLOAT4{ Colors::Blue });
	AddGameObject(m_pBoxLeft);

	const auto pBoxLeftActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	const auto pBoxLeftShape = PxRigidActorExt::createExclusiveShape(*pBoxLeftActor, PxBoxGeometry{ boxSizes.x / 2, boxSizes.y / 2, boxSizes.z / 2 }, *pBoxMat);

	m_pBoxLeft->AttachRigidActor(pBoxLeftActor);

	m_pBoxLeft->Translate(-3, 5, 0);

	//BoxRigth
	m_pBoxRight = new CubePosColorNorm(boxSizes.x, boxSizes.y, boxSizes.z, XMFLOAT4{ Colors::Red });
	AddGameObject(m_pBoxRight);

	const auto pBoxRightActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	const auto pBoxRightShape = PxRigidActorExt::createExclusiveShape(*pBoxRightActor, PxBoxGeometry{ boxSizes.x / 2, boxSizes.y / 2, boxSizes.z / 2 }, *pBoxMat);

	m_pBoxRight->AttachRigidActor(pBoxRightActor);

	m_pBoxRight->Translate(3, 5, 0);



	//Triggers
	const XMFLOAT3 triggerBoxSizes{ 5, 2, 5 };

	//BoxLeftTrigger
	const auto pBoxLeftTrigger = new CubePosColorNorm(triggerBoxSizes.x, triggerBoxSizes.y, triggerBoxSizes.z, XMFLOAT4{ Colors::Blue });
	AddGameObject(pBoxLeftTrigger);

	m_pTriggerLeft = pPhysX->createRigidStatic(PxTransform{ PxIdentity });
	const auto pTriggerLeftShape = PxRigidActorExt::createExclusiveShape(*m_pTriggerLeft, PxBoxGeometry{ boxSizes.x / 2, boxSizes.y / 2, boxSizes.z / 2 }, *pBoxMat);
	pTriggerLeftShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	pTriggerLeftShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);


	pBoxLeftTrigger->AttachRigidActor(m_pTriggerLeft);

	pBoxLeftTrigger->Translate(-8, 2, 0);

	//BoxRightTrigger
	const auto pBoxRightTrigger = new CubePosColorNorm(triggerBoxSizes.x, triggerBoxSizes.y, triggerBoxSizes.z, XMFLOAT4{ Colors::Red });
	AddGameObject(pBoxRightTrigger);

	m_pTriggerRight = pPhysX->createRigidStatic(PxTransform{ PxIdentity });
	const auto pTriggerRigthShape = PxRigidActorExt::createExclusiveShape(*m_pTriggerRight, PxBoxGeometry{ boxSizes.x / 2, boxSizes.y / 2, boxSizes.z / 2 }, *pBoxMat);
	pTriggerRigthShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	pTriggerRigthShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);


	pBoxRightTrigger->AttachRigidActor(m_pTriggerRight);

	pBoxRightTrigger->Translate(8, 2, 0);

	//Hatches
	const XMFLOAT3 hatchSizes{ 5, 0.5f, 2 };

	//Hatches Left
	m_pHatchLeft = new CubePosColorNorm(hatchSizes.x, hatchSizes.y, hatchSizes.z, XMFLOAT4{ Colors::Blue });
	AddGameObject(m_pHatchLeft);

	const auto pHatchLeftActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	const auto pHatchLeftShape = PxRigidActorExt::createExclusiveShape(*pHatchLeftActor, PxBoxGeometry{ hatchSizes.x / 2, hatchSizes.y / 2, hatchSizes.z / 2 }, *pBoxMat);


	pHatchLeftActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
	m_LeftKinematicPostion = { -9, 17, 0 };

	m_pHatchLeft->AttachRigidActor(pHatchLeftActor);

	m_pHatchLeft->Translate(m_LeftKinematicPostion.x, m_LeftKinematicPostion.y, m_LeftKinematicPostion.z);

	//Hatches Rigth
	m_pHatchRight = new CubePosColorNorm(hatchSizes.x, hatchSizes.y, hatchSizes.z, XMFLOAT4{ Colors::Red });
	AddGameObject(m_pHatchRight);

	const auto pHatchRigthActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	const auto pHatchRigthShape = PxRigidActorExt::createExclusiveShape(*pHatchRigthActor, PxBoxGeometry{ hatchSizes.x / 2, hatchSizes.y / 2, hatchSizes.z / 2 }, *pBoxMat);


	pHatchRigthActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
	m_RightKinematicPostion = { 9, 17, 0 };

	m_pHatchRight->AttachRigidActor(pHatchRigthActor);

	m_pHatchRight->Translate(m_RightKinematicPostion.x, m_RightKinematicPostion.y, m_RightKinematicPostion.z);



	//Player
	m_pSphere = new SpherePosColorNorm(1.f, 10, 10, XMFLOAT4{ Colors::Gray });
	AddGameObject(m_pSphere);

	const auto pSphereActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	PxRigidActorExt::createExclusiveShape(*pSphereActor, PxSphereGeometry{ 1.f }, *pBallMat);
	pSphereActor->setMass(5);

	auto d6Joint = PxD6JointCreate(*pPhysX, nullptr, PxTransform{ PxIdentity }, pSphereActor, PxTransform{ PxIdentity });
	d6Joint->setMotion(PxD6Axis::eX, PxD6Motion::eFREE);
	d6Joint->setMotion(PxD6Axis::eY, PxD6Motion::eFREE);
	d6Joint->setMotion(PxD6Axis::eTWIST, PxD6Motion::eFREE);
	d6Joint->setMotion(PxD6Axis::eSWING1, PxD6Motion::eFREE);
	d6Joint->setMotion(PxD6Axis::eSWING2, PxD6Motion::eFREE);

	m_pSphere->AttachRigidActor(pSphereActor);

	m_pSphere->Translate(0, 5, 0);


	//Balls

	//Ball1
	m_pSphereTopRight = new SpherePosColorNorm(1.f, 10, 10, XMFLOAT4{ Colors::Gray });
	AddGameObject(m_pSphereTopRight);

	const auto pSphereActorTopRight = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	PxRigidActorExt::createExclusiveShape(*pSphereActorTopRight, PxSphereGeometry{ 1.f }, *pBallMat);
	pSphereActorTopRight->setMass(5);

	m_pSphereTopRight->AttachRigidActor(pSphereActorTopRight);

	m_pSphereTopRight->Translate(1, 20, 0);


	//Ball2
	m_pSphereTopLeft = new SpherePosColorNorm(1.f, 10, 10, XMFLOAT4{ Colors::Gray });
	AddGameObject(m_pSphereTopLeft);

	const auto pSphereActorTopLeft = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	PxRigidActorExt::createExclusiveShape(*pSphereActorTopLeft, PxSphereGeometry{ 1.f }, *pBallMat);
	pSphereActorTopLeft->setMass(5);

	m_pSphereTopLeft->AttachRigidActor(pSphereActorTopLeft);

	m_pSphereTopLeft->Translate(-1, 20, 0);


	//Sound Effect
	const auto pFmod = SoundManager::GetInstance()->GetSystem();


	auto result = pFmod->createStream("Resources/Sounds/bell.mp3", FMOD_2D | FMOD_LOOP_OFF, nullptr, &m_pSound2D);
	SoundManager::GetInstance()->ErrorCheck(result);




}

void W2_AssignmentScene::Update()
{
	constexpr float force{ 5 };

	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::down, 'R'))
	{
		ResetScene();
	}
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::down, VK_LEFT))
	{
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addTorque({ 0, 0, force });
	}
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::down, VK_RIGHT))
	{
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addTorque({ 0, 0, -force });
	}

	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::pressed, VK_SPACE))
	{
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addForce({ 0,40,0 }, PxForceMode::eIMPULSE);
	}

	constexpr float rotateSpeed{ 45 };

	if (m_LeftTriggered)
	{

		const auto pFmod = SoundManager::GetInstance()->GetSystem();

		auto result = pFmod->playSound(m_pSound2D, nullptr, false, &m_pChannel2D);
		SoundManager::GetInstance()->ErrorCheck(result);

		m_pChannel2D->setPaused(false);
		m_LeftTriggered = false;
		m_LeftRotating = true;
	}
	if (m_RightTriggered)
	{
		const auto pFmod = SoundManager::GetInstance()->GetSystem();

		auto result = pFmod->playSound(m_pSound2D, nullptr, false, &m_pChannel2D);
		SoundManager::GetInstance()->ErrorCheck(result);

		m_pChannel2D->setPaused(false);
		m_RightTriggered = false;
		m_RightRotating = true;
	}

	if (m_LeftRotating)
	{
		m_LeftAngle += rotateSpeed * m_SceneContext.GetGameTime()->GetElapsed();
		m_pHatchLeft->RotateDegrees(0, 0, m_LeftAngle);
		m_LeftKinematicPostion.y -= 1.0f * m_SceneContext.GetGameTime()->GetElapsed();
		m_LeftKinematicPostion.x += 1.0f * m_SceneContext.GetGameTime()->GetElapsed();
		m_pHatchLeft->Translate(m_LeftKinematicPostion.x, m_LeftKinematicPostion.y, m_LeftKinematicPostion.z);
		if (m_LeftAngle >= 90)
		{
			m_LeftRotating = false;
		}
	}

	if (m_RightRotating)
	{
		m_RightAngle -= rotateSpeed * m_SceneContext.GetGameTime()->GetElapsed();
		m_pHatchRight->RotateDegrees(0, 0, m_RightAngle);
		m_RightKinematicPostion.y -= 1.0f * m_SceneContext.GetGameTime()->GetElapsed();
		m_RightKinematicPostion.x -= 1.0f * m_SceneContext.GetGameTime()->GetElapsed();
		m_pHatchRight->Translate(m_RightKinematicPostion.x, m_RightKinematicPostion.y, m_RightKinematicPostion.z);
		if (m_RightAngle <= -90)
		{
			m_RightRotating = false;
		}
	}


}

void W2_AssignmentScene::Draw() const
{

}

void W2_AssignmentScene::OnSceneActivated()
{
	Logger::GetInstance()->LogFormat(LogLevel::Info, L"Scene Activated > \"%ls\"", GetName().c_str());
}

void W2_AssignmentScene::OnSceneDeactivated()
{
}





void W2_AssignmentScene::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
	for (size_t i = 0; i < count; i++)
	{
		if (pairs[i].flags & (PxTriggerPairFlag::eREMOVED_SHAPE_OTHER | PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER))
			continue;

		const PxTriggerPair& pair = pairs[i];
		if (pair.triggerActor == m_pTriggerLeft && pair.otherActor == m_pBoxLeft->GetRigidActor())
		{
			if (pair.status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
			{
				Logger::GetInstance()->LogInfo(L"Sphere FOUND box left");
				m_LeftTriggered = true;
			}
		}
		if (pair.triggerActor == m_pTriggerRight && pair.otherActor == m_pBoxRight->GetRigidActor())
		{
			if (pair.status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
			{
				Logger::GetInstance()->LogInfo(L"Sphere FOUND box rigth");
				m_RightTriggered = true;
			}
		}
	}

}

void W2_AssignmentScene::ResetScene()
{

	m_pHatchLeft->RotateDegrees(0, 0, 0);
	m_LeftKinematicPostion = { -9, 17, 0 };
	m_pHatchLeft->Translate(m_LeftKinematicPostion.x, m_LeftKinematicPostion.y, m_LeftKinematicPostion.z);

	m_pHatchRight->RotateDegrees(0, 0, 0);
	m_RightKinematicPostion = { 9, 17, 0 };
	m_pHatchRight->Translate(m_RightKinematicPostion.x, m_RightKinematicPostion.y, m_RightKinematicPostion.z);

	m_pBoxLeft->Translate(-3, 5, 0);
	m_pBoxLeft->RotateDegrees(0, 0, 0);
	if (auto pBoxLeft = m_pBoxLeft->GetRigidActor()->is<PxRigidDynamic>())
	{
		ResetDynamicBody(pBoxLeft);
	}
	m_pBoxRight->Translate(3, 5, 0);
	m_pBoxRight->RotateDegrees(0, 0, 0);
	if (auto pBoxRight = m_pBoxRight->GetRigidActor()->is<PxRigidDynamic>())
	{
		ResetDynamicBody(pBoxRight);
	}


	m_pSphere->Translate(0, 5, 0);
	m_pSphere->RotateDegrees(0, 0, 0);
	if (auto pSphere = m_pSphere->GetRigidActor()->is<PxRigidDynamic>())
	{
		ResetDynamicBody(pSphere);
	}

	m_pSphereTopRight->Translate(1, 20, 0);
	m_pSphereTopRight->RotateDegrees(0, 0, 0);
	if (auto pSphereTopRight = m_pSphereTopRight->GetRigidActor()->is<PxRigidDynamic>())
	{
		ResetDynamicBody(pSphereTopRight);
	}
	m_pSphereTopLeft->Translate(-1, 20, 0);
	m_pSphereTopLeft->RotateDegrees(0, 0, 0);
	if (auto pSphereTopLeft = m_pSphereTopLeft->GetRigidActor()->is<PxRigidDynamic>())
	{
		ResetDynamicBody(pSphereTopLeft);
	}


	m_LeftTriggered = false;
	m_RightTriggered = false;

	m_LeftRotating = false;
	m_RightRotating = false;

	m_RightAngle = 0;
	m_LeftAngle = 0;

}

void W2_AssignmentScene::ResetDynamicBody(PxRigidDynamic* body)
{
	body->putToSleep();
	body->wakeUp();
}
