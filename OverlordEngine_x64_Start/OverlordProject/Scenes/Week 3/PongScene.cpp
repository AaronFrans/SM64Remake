#include "stdafx.h"
#include "PongScene.h"

#include "Prefabs/CubePrefab.h"
#include "Prefabs/SpherePrefab.h"
#include "Prefabs/TorusPrefab.h"
#include "Prefabs/FixedCamera.h"


PongScene::PongScene() :
	GameScene(L"Component Test Scene") {}

void PongScene::Initialize()
{
	//auto& physX = PxGetPhysics();
	//auto pBouncyMat = physX.createMaterial(.5f, .5f, 1);

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


	//auto pRigidBody = m_pLeftPadle->AddComponent(new RigidBodyComponent());


	m_pRightPadle = new CubePrefab(paddleDimensions, XMFLOAT4{ Colors::Gray });
	m_pRightPadle->GetTransform()->Translate(25, 20, 0);
	AddChild(m_pRightPadle);

	//Borders

	const auto topBottomDimensions{ XMFLOAT3{ 60,5,2 } };

	auto pTopBorder = new CubePrefab(topBottomDimensions, XMFLOAT4{ Colors::White });
	pTopBorder->GetTransform()->Translate(0, 40, 0);

	AddChild(pTopBorder);

	auto pBottomBorder = new CubePrefab(topBottomDimensions, XMFLOAT4{ Colors::White });
	pBottomBorder->GetTransform()->Translate(0, 0, 0);

	AddChild(pBottomBorder);

	const auto leftRightDimensions{ XMFLOAT3{ 2,40,2 } };
	auto pLeftBorder = new CubePrefab(leftRightDimensions, XMFLOAT4{ Colors::White });
	pLeftBorder->GetTransform()->Translate(-30, 20, 0);

	AddChild(pLeftBorder);

	auto pRigthBorder = new CubePrefab(leftRightDimensions, XMFLOAT4{ Colors::White });
	pRigthBorder->GetTransform()->Translate(30, 20, 0);

	AddChild(pRigthBorder);

	//Ball

	m_pPongBall = new SpherePrefab();
	m_pPongBall->GetTransform()->Translate(0, 20, 0);

	AddChild(m_pPongBall);




}

void PongScene::Update()
{

}
