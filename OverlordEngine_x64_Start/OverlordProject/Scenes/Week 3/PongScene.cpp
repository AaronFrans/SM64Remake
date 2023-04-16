#include "stdafx.h"
#include <time.h>       
#include "PongScene.h"

#include "Prefabs/CubePrefab.h"
#include "Prefabs/SpherePrefab.h"
#include "Prefabs/TorusPrefab.h"
#include "Prefabs/FixedCamera.h"


PongScene::PongScene() :
	GameScene(L"Pong Scene") {}

void PongScene::Initialize()
{
	srand((unsigned int)time(NULL));
	int throwAwayRand = rand();
	++throwAwayRand;


	auto& physX = PxGetPhysics();
	auto pBouncyMat = physX.createMaterial(0, 0, 1);

	m_pCamera = new FixedCamera();


	m_pCamera->GetTransform()->Translate(0, 20, -50);
	m_pCamera->GetTransform()->Rotate(0, 0, 0);
	AddChild(m_pCamera)->GetComponent<CameraComponent>()->SetActive();
	//m_SceneContext.pCamera->SetActive(m_pCamera);



	//Padles
	constexpr auto paddleDimensions{ XMFLOAT3{ 1,6,2 } };

	m_pLeftPadle = new CubePrefab(paddleDimensions, XMFLOAT4{ Colors::Gray });
	m_pLeftPadle->GetTransform()->Translate(ORIGINAL_LEFT_PADDLE_POSS.x, ORIGINAL_LEFT_PADDLE_POSS.y, ORIGINAL_LEFT_PADDLE_POSS.z);
	AddChild(m_pLeftPadle);


	auto pRigidBody = m_pLeftPadle->AddComponent(new RigidBodyComponent());

	pRigidBody->AddCollider(
		PxBoxGeometry{ paddleDimensions.x / 2.f, paddleDimensions.y / 2.f, paddleDimensions.z / 2.f },
		*pBouncyMat);
	pRigidBody->SetKinematic(true);

	pRigidBody->SetCollisionGroup(CollisionGroup::Group0);




	m_pRightPadle = new CubePrefab(paddleDimensions, XMFLOAT4{ Colors::Gray });
	m_pRightPadle->GetTransform()->Translate(ORIGINAL_RIGHT_PADDLE_POSS.x, ORIGINAL_RIGHT_PADDLE_POSS.y, ORIGINAL_RIGHT_PADDLE_POSS.z);
	AddChild(m_pRightPadle);

	pRigidBody = m_pRightPadle->AddComponent(new RigidBodyComponent());

	pRigidBody->AddCollider(
		PxBoxGeometry{ paddleDimensions.x / 2.f, paddleDimensions.y / 2.f, paddleDimensions.z / 2.f },
		*pBouncyMat);
	pRigidBody->SetKinematic(true);


	pRigidBody->SetCollisionGroup(CollisionGroup::Group0);

	//Borders

	constexpr auto topBottomDimensions{ XMFLOAT3{ 60,5,2 } };

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

	constexpr auto leftRightDimensions{ XMFLOAT3{ 2,40,2 } };

	auto onTrigger = [&](GameObject*, GameObject*, PxTriggerAction action)
	{
		if (action == PxTriggerAction::ENTER)
		{
			m_HasHitWall = true;
		}
	};

	auto pLeftBorder = new CubePrefab(leftRightDimensions, XMFLOAT4{ Colors::White });
	pLeftBorder->GetTransform()->Translate(-30, 20, 0);


	pRigidBody = pLeftBorder->AddComponent(new RigidBodyComponent(true));

	pRigidBody->AddCollider(
		PxBoxGeometry{ leftRightDimensions.x / 2.f, leftRightDimensions.y / 2.f, leftRightDimensions.z / 2.f },
		*pBouncyMat, true);

	pLeftBorder->SetOnTriggerCallBack(onTrigger);


	pRigidBody->SetCollisionGroup(CollisionGroup::Group1);

	AddChild(pLeftBorder);

	auto pRigthBorder = new CubePrefab(leftRightDimensions, XMFLOAT4{ Colors::White });
	pRigthBorder->GetTransform()->Translate(30, 20, 0);

	pRigthBorder->SetOnTriggerCallBack(onTrigger);


	pRigidBody = pRigthBorder->AddComponent(new RigidBodyComponent(true));

	pRigidBody->AddCollider(
		PxBoxGeometry{ leftRightDimensions.x / 2.f, leftRightDimensions.y / 2.f, leftRightDimensions.z / 2.f },
		*pBouncyMat, true);


	pRigidBody->SetCollisionGroup(CollisionGroup::Group1);

	AddChild(pRigthBorder);

	//Ball

	m_pPongBall = new SpherePrefab(1, 10, { 1,0,0,1 });
	m_pPongBall->GetTransform()->Translate(ORIGINAL_BALL_POSS.x, ORIGINAL_BALL_POSS.y, ORIGINAL_BALL_POSS.z);

	pRigidBody = m_pPongBall->AddComponent(new RigidBodyComponent(false));

	pRigidBody->AddCollider(
		PxSphereGeometry{ 1 },
		*pBouncyMat
	);



	pRigidBody->SetCollisionGroup(CollisionGroup::Group1 | CollisionGroup::Group0);

	//auto actor = pRigidBody->GetPxRigidActor();
	//actor->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);

	AddChild(m_pPongBall);


	//Inputs

	InputAction action = InputAction{ Inputs::Start, InputState::released, VK_SPACE };
	m_SceneContext.pInput->AddInputAction(action);
	action = InputAction{ Inputs::Reset, InputState::released,'R' };
	m_SceneContext.pInput->AddInputAction(action);


	action = InputAction{ Inputs::LeftUp, InputState::down, 'W' };
	m_SceneContext.pInput->AddInputAction(action);
	action = InputAction{ Inputs::LeftDown, InputState::down, 'S' };
	m_SceneContext.pInput->AddInputAction(action);

	action = InputAction{ Inputs::RigthUp, InputState::down, 'I' };
	m_SceneContext.pInput->AddInputAction(action);
	action = InputAction{ Inputs::RigthDown, InputState::down, 'K' };
	m_SceneContext.pInput->AddInputAction(action);



	m_SceneContext.settings.clearColor = { 0, 0, 0, 1 };
	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.drawPhysXDebug= false;
	m_SceneContext.settings.drawUserDebug= false;

}

void PongScene::PostInitialize()
{
	auto pRigidBody = m_pPongBall->GetComponent<RigidBodyComponent>();
	pRigidBody->GetPxRigidActor()->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);;
	pRigidBody->SetConstraint(RigidBodyConstraint::TransZ | RigidBodyConstraint::AllRot, false);

}

void PongScene::Update()
{
	if (m_HasHitWall)
	{
		ResetScene();
		m_HasHitWall = false;
	}


	auto& input = m_SceneContext.pInput;
	if (!m_HasStarted && input->IsActionTriggered(Inputs::Start))
	{
		PushBall();
		m_HasStarted = true;
	}

	if (!m_HasStarted)
		return;


	if (input->IsActionTriggered(Inputs::Reset))
	{
		ResetScene();
	}

	float moveSpeed{ 10 * m_SceneContext.pGameTime->GetElapsed() };

	if (input->IsActionTriggered(Inputs::LeftUp))
	{
		auto transform = m_pLeftPadle->GetTransform();

		auto curPos = transform->GetPosition();
		transform->Translate(curPos.x, curPos.y + moveSpeed, curPos.z);

	}
	if (input->IsActionTriggered(Inputs::LeftDown))
	{

		auto transform = m_pLeftPadle->GetTransform();

		auto curPos = transform->GetPosition();
		transform->Translate(curPos.x, curPos.y - moveSpeed, curPos.z);
	}
	if (input->IsActionTriggered(Inputs::RigthUp))
	{

		auto transform = m_pRightPadle->GetTransform();

		auto curPos = transform->GetPosition();
		transform->Translate(curPos.x, curPos.y + moveSpeed, curPos.z);
	}
	if (input->IsActionTriggered(Inputs::RigthDown))
	{

		auto transform = m_pRightPadle->GetTransform();

		auto curPos = transform->GetPosition();
		transform->Translate(curPos.x, curPos.y - moveSpeed, curPos.z);
	}
}

void PongScene::PushBall()
{
	constexpr float bounceSpeed{ 10.f };
	constexpr int maxAngle{ 62831 };
	int random = rand() % maxAngle;
	constexpr float angleDiv{ 10000.f };
	float randAngle = random / angleDiv;

	auto pRigidBody = m_pPongBall->GetComponent<RigidBodyComponent>();

	pRigidBody->AddForce({ cosf(randAngle) * bounceSpeed,sinf(randAngle) * bounceSpeed,0 }, physx::PxForceMode::eIMPULSE);
}

void PongScene::ResetScene()
{

	m_pLeftPadle->GetTransform()->Translate(ORIGINAL_LEFT_PADDLE_POSS.x, ORIGINAL_LEFT_PADDLE_POSS.y, ORIGINAL_LEFT_PADDLE_POSS.z);

	m_pRightPadle->GetTransform()->Translate(ORIGINAL_RIGHT_PADDLE_POSS.x, ORIGINAL_RIGHT_PADDLE_POSS.y, ORIGINAL_RIGHT_PADDLE_POSS.z);

	m_pPongBall->GetTransform()->Translate(ORIGINAL_BALL_POSS.x, ORIGINAL_BALL_POSS.y, ORIGINAL_BALL_POSS.z);
	m_pPongBall->GetComponent<RigidBodyComponent>()->ClearForce();

	m_HasStarted = false;

}