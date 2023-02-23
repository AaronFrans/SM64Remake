#include "stdafx.h"
#include "PhysXTestScene.h"

#include "CubePosColorNorm.h"
#include "TorusPosColorNorm.h"
#include "CylinderPosColorNorm.h"

#include "Logger.h"


void PhysXTestScene::Initialize()
{
	EnablePhysxDebugRendering(true);
	const auto pPhysX = PhysxManager::GetInstance()->GetPhysics();
	const PxMaterial* pPhysXMat = pPhysX->createMaterial(0.5, 0.5, 2);


	const XMFLOAT3 actorDims{ 1,1,1 };
	//Cube
	m_pCube = new CubePosColorNorm{ actorDims.x, actorDims.y, actorDims.z };
	AddGameObject(m_pCube);

	//Cube Actor
	PxRigidDynamic* pCubeActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	PxBoxGeometry boxGeomitry = PxBoxGeometry{ actorDims.x / 2, actorDims.y / 2, actorDims.z / 2 };

	//PxShape* pCubeShape = pPhysX->createShape(boxGeomitry, *pPhysXMat, true);
	//
	//pCubeActor->attachShape(*pCubeShape);
	//
	//
	//m_pPhysxScene->addActor(*pCubeActor);


	PxRigidActorExt::createExclusiveShape(*pCubeActor, boxGeomitry, *pPhysXMat);

	//Link mesh with actor
	m_pCube->AttachRigidActor(pCubeActor);



	//Ground Plane

	auto pGround = pPhysX->createRigidStatic(PxTransform{ PxQuat{PxPiDivTwo, PxVec3{0,0,1}} });
	PxRigidActorExt::createExclusiveShape(*pGround, PxPlaneGeometry{}, *pPhysXMat);
	m_pPhysxScene->addActor(*pGround);


}

void PhysXTestScene::Update()
{
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::pressed, 'R'))
		m_pCube->Translate(0, 10, 0);

}

void PhysXTestScene::Draw() const
{

}

void PhysXTestScene::OnSceneActivated()
{
	Logger::GetInstance()->LogFormat(LogLevel::Info, L"Scene Activated > \"%ls\"", GetName().c_str());
}

void PhysXTestScene::OnSceneDeactivated()
{
}
