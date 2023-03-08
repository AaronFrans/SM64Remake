#include "stdafx.h"
#include <time.h>       
#include "PongScene.h"

#include "Prefabs/CubePrefab.h"
#include "Prefabs/SpherePrefab.h"
#include "Prefabs/TorusPrefab.h"
#include "Prefabs/FixedCamera.h"


PongScene::PongScene() :
	GameScene(L"Component Test Scene") {}

void PongScene::Initialize()
{

	srand((unsigned int)time(NULL));
	rand();
	auto& physX = PxGetPhysics();
	auto pBouncyMat = physX.createMaterial(.5f, .5f, 1);

	m_pCamera = new FixedCamera();


	m_pCamera->GetTransform()->Translate(0, 20, -50);
	m_pCamera->GetTransform()->Rotate(0, 0, 0); // -> does not work?
	AddChild(m_pCamera)->GetComponent<CameraComponent>()->SetActive();



	//m_SceneContext.pCamera->SetActive(m_pCamera);



	//Padles
	const auto paddleDimensions{ XMFLOAT3{ 1,6,2 } };

	m_pLeftPadle = new CubePrefab(paddleDimensions, XMFLOAT4{ Colors::Gray });
	m_pLeftPadle->GetTransform()->Translate(-25, 20, 0);
	AddChild(m_pLeftPadle);


	auto pRigidBody = m_pLeftPadle->AddComponent(new RigidBodyComponent());

	pRigidBody->AddCollider(
		PxBoxGeometry{ paddleDimensions.x / 2.f, paddleDimensions.y / 2.f, paddleDimensions.z / 2.f },
		*pBouncyMat);
	pRigidBody->SetKinematic(true);

	pRigidBody->SetCollisionGroup(CollisionGroup::Group0);

	m_pRightPadle = new CubePrefab(paddleDimensions, XMFLOAT4{ Colors::Gray });
	m_pRightPadle->GetTransform()->Translate(25, 20, 0);
	AddChild(m_pRightPadle);

	pRigidBody = m_pRightPadle->AddComponent(new RigidBodyComponent());

	pRigidBody->AddCollider(
		PxBoxGeometry{ paddleDimensions.x / 2.f, paddleDimensions.y / 2.f, paddleDimensions.z / 2.f },
		*pBouncyMat);
	pRigidBody->SetKinematic(true);


	pRigidBody->SetCollisionGroup(CollisionGroup::Group0);

	//Borders

	const auto topBottomDimensions{ XMFLOAT3{ 60,5,2 } };

	auto pTopBorder = new CubePrefab(topBottomDimensions, XMFLOAT4{ Colors::White });
	pTopBorder->GetTransform()->Translate(0, 40, 0);

	pRigidBody = pTopBorder->AddComponent(new RigidBodyComponent(true));

	pRigidBody->AddCollider(
		PxBoxGeometry{ topBottomDimensions.x / 2.f, topBottomDimensions.y / 2.f, topBottomDimensions.z / 2.f },
		*pBouncyMat);


	pRigidBody->SetCollisionGroup(CollisionGroup::Group1);

	AddChild(pTopBorder);

	auto pBottomBorder = new CubePrefab(topBottomDimensions, XMFLOAT4{ Colors::White });
	pBottomBorder->GetTransform()->Translate(0, 0, 0);

	pRigidBody = pBottomBorder->AddComponent(new RigidBodyComponent(true));

	pRigidBody->AddCollider(
		PxBoxGeometry{ topBottomDimensions.x / 2.f, topBottomDimensions.y / 2.f, topBottomDimensions.z / 2.f },
		*pBouncyMat);


	pRigidBody->SetCollisionGroup(CollisionGroup::Group1);

	AddChild(pBottomBorder);

	const auto leftRightDimensions{ XMFLOAT3{ 2,40,2 } };
	auto pLeftBorder = new CubePrefab(leftRightDimensions, XMFLOAT4{ Colors::White });
	pLeftBorder->GetTransform()->Translate(-30, 20, 0);
	
	
	pRigidBody = pLeftBorder->AddComponent(new RigidBodyComponent(true));
	
	pRigidBody->AddCollider(
		PxBoxGeometry{ leftRightDimensions.x / 2.f, leftRightDimensions.y / 2.f, leftRightDimensions.z / 2.f },
		*pBouncyMat, true);


	pRigidBody->SetCollisionGroup(CollisionGroup::Group1);
	
	AddChild(pLeftBorder);
	
	auto pRigthBorder = new CubePrefab(leftRightDimensions, XMFLOAT4{ Colors::White });
	pRigthBorder->GetTransform()->Translate(30, 20, 0);
	
	
	pRigidBody = pRigthBorder->AddComponent(new RigidBodyComponent(true));
	
	pRigidBody->AddCollider(
		PxBoxGeometry{ leftRightDimensions.x / 2.f, leftRightDimensions.y / 2.f, leftRightDimensions.z / 2.f },
		*pBouncyMat, true);
	

	pRigidBody->SetCollisionGroup(CollisionGroup::Group1);

	AddChild(pRigthBorder);

	//Ball

	m_pPongBall = new SpherePrefab();
	m_pPongBall->GetTransform()->Translate(0, 20, 0);


	

	pRigidBody = m_pPongBall->AddComponent(new RigidBodyComponent(false));

	pRigidBody->AddCollider(
		PxSphereGeometry{ 1 },
		*pBouncyMat
	);
	


	pRigidBody->SetCollisionGroup(CollisionGroup::Group1 | CollisionGroup::Group0);
	
	//auto actor = pRigidBody->GetPxRigidActor();
	//actor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);

	AddChild(m_pPongBall);




}

void PongScene::PostInitialize()
{
	constexpr float bounceSpeed{ 20.f };
	int random = rand() % 62831;
	float randAngle = random / 10000.f;


	auto pRigidBody = m_pPongBall->GetComponent<RigidBodyComponent>();
	pRigidBody->GetPxRigidActor()->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);;

	pRigidBody->AddForce({ cosf(randAngle) * bounceSpeed,sinf(randAngle) * bounceSpeed,0}, physx::PxForceMode::eIMPULSE);
}

void PongScene::Update()
{
	
}
