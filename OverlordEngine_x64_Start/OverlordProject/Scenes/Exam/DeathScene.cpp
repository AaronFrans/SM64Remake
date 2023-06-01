#include "stdafx.h"
#include "DeathScene.h"
#include "Prefabs/Button.h"

void DeathScene::Initialize()
{
	m_SceneContext.settings.enableOnGUI = true;
	m_SceneContext.settings.clearColor = { 0,0,0,1 };
	m_SceneContext.settings.drawUserDebug = false;
	m_SceneContext.settings.drawGrid = false;

	m_SceneContext.pInput->ForceMouseToCenter(false);
	m_SceneContext.settings.drawPhysXDebug = false;

	auto pCamera = AddChild(new FixedCamera)->GetComponent<CameraComponent>();

	SetActiveCamera(pCamera);

	auto pRestart = AddChild(new Button(L"Textures/Mario/Menu/Restart.png",
		[&]() {
			SceneManager::Get()->SetActiveGameScene(L"MarioScene");
		}));
	m_Buttons.emplace_back(pRestart);
	pRestart->GetTransform()->Translate(270, 450, 0.5f);



	auto pQuit = AddChild(new Button(L"Textures/Mario/Menu/Quit.png", [&]() {OverlordGame::Quit(); }));
	pQuit->GetTransform()->Translate(800, 450, 0.5f);

	m_Buttons.emplace_back(pQuit);


	auto pBG = AddChild(new GameObject);
	pBG->AddComponent(new SpriteComponent(L"Textures/Mario/Menu/Death Menu.png"));
	pBG->GetTransform()->Translate(0, 0, 0.9f);
}

void DeathScene::Update()
{
	if (InputManager::IsMouseButton(InputState::pressed, VK_LBUTTON))
	{
		auto mousePos = InputManager::GetMousePosition();
		for (auto button : m_Buttons)
		{
			button->OnClicked(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
		}
	}
}

void DeathScene::OnGUI()
{
}
