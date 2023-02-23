#include "stdafx.h"
#include "TestScene.h"

#include "CubePosColorNorm.h"
#include "TorusPosColorNorm.h"
#include "CylinderPosColorNorm.h"

#include "Logger.h"

enum InputIds
{
	Up,
	Down,
	Left,
	Right
};

void TestScene::Initialize()
{

	//Cube
	m_pCube = new CubePosColorNorm{ 5,2,1 };
	AddGameObject(m_pCube);

	m_pCube->Translate(0, 1, 0);

	//Camera
	m_SceneContext.GetCamera()->SetPosition({ 0, 10, -25 });
	m_SceneContext.GetCamera()->SetForward({ 0, -3,  1 });

	//Torus
	m_pTorus = new TorusPosColorNorm(8, 50, 1, 10, XMFLOAT4{ Colors::Crimson });
	AddGameObject(m_pTorus);

	//Cillinder
	auto pCylinder = new CylinderPosColorNorm(1.5f, 10, 4);
	m_pTorus->AddChild(pCylinder);

	m_pTorus->Translate(20, 0, 0);

	pCylinder->Translate(0, 8, 0);


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

void TestScene::Update()
{

	const float rotSpeed{ (INT32_MAX - 1) * m_SceneContext.GetGameTime()->GetTotal() };
	m_pCube->RotateDegrees(0, rotSpeed, 0);
	m_pTorus->RotateDegrees(0, 0, rotSpeed);

	const float moveSpeed{ (INT32_MAX - 1) * m_SceneContext.GetGameTime()->GetElapsed() };
	XMFLOAT3 translation{};


	if (m_SceneContext.GetInput()->IsActionTriggered(Left))
		translation.x -= moveSpeed;
	if (m_SceneContext.GetInput()->IsActionTriggered(Right))
		translation.x += moveSpeed;

	if (m_SceneContext.GetInput()->IsActionTriggered(Up))
		translation.y += moveSpeed;
	if (m_SceneContext.GetInput()->IsActionTriggered(Down))
		translation.y -= moveSpeed;

	XMStoreFloat3(&translation, XMLoadFloat3(&translation) + XMLoadFloat3(&m_pCube->GetPosition()));
	m_pCube->Translate(translation.x, translation.y, translation.z);
}

void TestScene::Draw() const
{

}

void TestScene::OnSceneActivated()
{
	Logger::GetInstance()->LogFormat(LogLevel::Info, L"Scene Activated > \"%ls\"", GetName().c_str());
}

void TestScene::OnSceneDeactivated()
{
}
