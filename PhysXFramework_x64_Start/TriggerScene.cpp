#include "stdafx.h"
#include "TriggerScene.h"

#include "ContentManager.h"
#include "MeshObject.h"
#include "SpherePosColorNorm.h"

#include "Logger.h"

enum InputIds
{
	Up,
	Down,
	Left,
	Right,
	Reset
};


void TriggerScene::Initialize()
{
	EnablePhysxDebugRendering(true);
	const auto pPhysX = PhysxManager::GetInstance()->GetPhysics();
	const PxMaterial* pPhysXMat = pPhysX->createMaterial(0.5, 0.5, 0.2f);

	auto pGround = pPhysX->createRigidStatic(PxTransform{ PxQuat{PxPiDivTwo, PxVec3{0,0,1}} });
	PxRigidActorExt::createExclusiveShape(*pGround, PxPlaneGeometry{}, *pPhysXMat);
	m_pPhysxScene->addActor(*pGround);


	m_pSphere = new SpherePosColorNorm(1.f, 10, 10, XMFLOAT4{ Colors::Gray });
	AddGameObject(m_pSphere);

	const auto pSphereActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	PxRigidActorExt::createExclusiveShape(*pSphereActor, PxSphereGeometry{ 1.f }, *pPhysXMat);

	m_pSphere->AttachRigidActor(pSphereActor);

	m_pSphere->Translate(0, 2, 0);


	m_pTriggerLeft = pPhysX->createRigidStatic(PxTransform{ { -8, 0.5f , 0} });
	auto pShape = PxRigidActorExt::createExclusiveShape(*m_pTriggerLeft, PxBoxGeometry{ 0.5f, 0.5f, 0.5f }, *pPhysXMat);

	pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	pShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);

	m_pPhysxScene->addActor(*m_pTriggerLeft);


	m_pTriggerRight = pPhysX->createRigidStatic(PxTransform{ {8, 0.5f , 0} });
	pShape = PxRigidActorExt::createExclusiveShape(*m_pTriggerRight, PxBoxGeometry{ 0.5f, 0.5f, 0.5f }, *pPhysXMat);

	pShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
	pShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);

	m_pPhysxScene->addActor(*m_pTriggerRight);

}

void TriggerScene::Update()
{
	constexpr float force{ 5 };

	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::down, VK_LEFT))
	{
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addTorque({ 0, 0, -force });
	}
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::down, VK_RIGHT))
	{
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addTorque({ 0, 0, force });
	}

	if (m_IsTriggerFound)
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addForce({ -10,10,0 }, PxForceMode::eIMPULSE);
}

void TriggerScene::Draw() const
{

}

void TriggerScene::OnSceneActivated()
{
	Logger::GetInstance()->LogFormat(LogLevel::Info, L"Scene Activated > \"%ls\"", GetName().c_str());
}

void TriggerScene::OnSceneDeactivated()
{
}

void TriggerScene::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
	for (size_t i = 0; i < count; i++)
	{
		if (pairs[i].flags & (PxTriggerPairFlag::eREMOVED_SHAPE_OTHER | PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER))
			continue;

		const PxTriggerPair& pair = pairs[i];
		if (pair.triggerActor == m_pTriggerLeft)
		{
			if (pair.status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
			{
				Logger::GetInstance()->LogInfo(L"Sphere FOUND box left");
				m_IsTriggerFound = true;
			}
			else if (pair.status == PxPairFlag::eNOTIFY_TOUCH_LOST)
			{
				Logger::GetInstance()->LogInfo(L"Sphere LOST box left");
				m_IsTriggerFound = false;
			}
		}
		if (pair.triggerActor == m_pTriggerRight)
		{
			if (pair.status == PxPairFlag::eNOTIFY_TOUCH_FOUND)
			{
				Logger::GetInstance()->LogInfo(L"Sphere FOUND box rigth");
				m_IsTriggerFound = true;
			}
			else if (pair.status == PxPairFlag::eNOTIFY_TOUCH_LOST)
			{
				Logger::GetInstance()->LogInfo(L"Sphere LOST box right");
				m_IsTriggerFound = false;
			}
		}
	}

}

