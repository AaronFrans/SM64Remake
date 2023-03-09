#include "stdafx.h"
#include "TutorialSceneWeek2.h"

#include "ContentManager.h"
#include "MeshObject.h"

#include "Logger.h"

enum InputIds
{
	Up,
	Down,
	Left,
	Right,
	Reset
};


void TutorialSceneWeek2::Initialize()
{

	EnablePhysxDebugRendering(true);
	const auto pPhysX = PhysxManager::GetInstance()->GetPhysics();
	const PxMaterial* pPhysXMat = pPhysX->createMaterial(0.5, 0.5, 0.9f);

	auto pGround = pPhysX->createRigidStatic(PxTransform{ PxQuat{PxPiDivTwo, PxVec3{0,0,1}} });
	PxRigidActorExt::createExclusiveShape(*pGround, PxPlaneGeometry{}, *pPhysXMat);
	m_pPhysxScene->addActor(*pGround);

	//Convex Chair
	m_pChairConvex = new MeshObject(L"Resources/Meshes/Chair.ovm");

	AddGameObject(m_pChairConvex);

	const auto pConvexMesh = ContentManager::GetInstance()->Load<PxConvexMesh>(L"Resources/Meshes/Chair.ovpc");
	const auto pConvexActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });

	PxRigidActorExt::createExclusiveShape(*pConvexActor, PxConvexMeshGeometry{ pConvexMesh }, *pPhysXMat);
	m_pChairConvex->AttachRigidActor(pConvexActor);

	m_pChairConvex->Translate(0, 20, 0);
	PxVec3 massLocalPose{ 0, 4,0 };
	PxRigidBodyExt::updateMassAndInertia(*pConvexActor, 10.f, &massLocalPose);


	//TriangleMesh
	m_pChairTriangle = new MeshObject(L"Resources/Meshes/Chair.ovm");

	AddGameObject(m_pChairTriangle);

	const auto pTriangleMesh = ContentManager::GetInstance()->Load<PxTriangleMesh>(L"Resources/Meshes/Chair.ovpt");
	const auto pTriangleActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });

	pTriangleActor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
	PxRigidActorExt::createExclusiveShape(*pTriangleActor, PxTriangleMeshGeometry{ pTriangleMesh }, *pPhysXMat);
	m_pChairTriangle->AttachRigidActor(pTriangleActor);

	m_pChairTriangle->Translate(10, 0, 0);
	m_KinematicPostion = { 10, 0, 0 };
	//PxRigidBodyExt::updateMassAndInertia(*pTriangleActor, 10.f, &massLocalPose);

}

void TutorialSceneWeek2::Update()
{

	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::pressed, 'R'))
	{
		m_pChairConvex->Translate(0, 10, 0);
		m_pChairConvex->Rotate(0, 0, 0);


		m_pChairTriangle->Translate(10, 0, 0);
		m_KinematicPostion = { 10, 0, 0 };
	}

	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::down, VK_LEFT))
	{
		m_KinematicPostion.x -= 5.0f * m_SceneContext.GetGameTime()->GetElapsed();
		m_pChairTriangle->GetRigidActor()->is<PxRigidDynamic>()->setKinematicTarget(PxTransform{ m_KinematicPostion });
	}
}

void TutorialSceneWeek2::Draw() const
{

}

void TutorialSceneWeek2::OnSceneActivated()
{
	Logger::GetInstance()->LogFormat(LogLevel::Info, L"Scene Activated > \"%ls\"", GetName().c_str());
}

void TutorialSceneWeek2::OnSceneDeactivated()
{
}