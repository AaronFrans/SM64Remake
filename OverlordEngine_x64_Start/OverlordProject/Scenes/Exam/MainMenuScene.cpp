#include "stdafx.h"
#include "MainMenuScene.h"
#include "Prefabs/Button.h"
#include "MarioScene.h"

void MainMenuScene::Initialize()
{
	m_SceneContext.settings.enableOnGUI = true;
	m_SceneContext.settings.clearColor = { 0,0,0,1 };
	m_SceneContext.settings.drawUserDebug = false;
	m_SceneContext.settings.drawGrid = false;

	m_SceneContext.pInput->ForceMouseToCenter(false);
	m_SceneContext.settings.drawPhysXDebug = false;

	auto pCamera = AddChild(new FixedCamera)->GetComponent<CameraComponent>();

	SetActiveCamera(pCamera);


	auto pStart = AddChild(new Button(L"Textures/Mario/Menu/Start.png",
		[&]() {
			SceneManager::Get()->SetActiveGameScene(L"MarioScene");
		}, "Resources/Sounds/Mario/Start-Restart.wav"));
	m_Buttons.emplace_back(pStart);
	pStart->GetTransform()->Translate(270, 450, 0.5f);



	auto pQuit = AddChild(new Button(L"Textures/Mario/Menu/Quit.png", [&]() {OverlordGame::Quit(); }, "Resources/Sounds/Mario/Exit.wav"));
	pQuit->GetTransform()->Translate(800, 450, 0.5f);

	m_Buttons.emplace_back(pQuit);


	auto pBG = AddChild(new GameObject);
	pBG->AddComponent(new SpriteComponent(L"Textures/Mario/Menu/Main Menu.png"));
	pBG->GetTransform()->Translate(0, 0, 0.9f);

	const auto pFmod = SoundManager::Get()->GetSystem();
	FMOD::Sound* pSound2D{ nullptr };
	auto result = pFmod->createStream("Resources/Sounds/Mario/MainMenu.mp3", FMOD_2D | FMOD_LOOP_NORMAL, nullptr, &pSound2D);
	result = pFmod->playSound(pSound2D, nullptr, false, &m_pChannel2D);
	m_pChannel2D->setPaused(true);
	m_pChannel2D->setVolume(0.5f);

}

void MainMenuScene::Update()
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

void MainMenuScene::OnGUI()
{

}

void MainMenuScene::OnSceneActivated()
{
	m_pChannel2D->setPosition(0, FMOD_TIMEUNIT_MS);
	m_pChannel2D->setPaused(false);
}

void MainMenuScene::OnSceneDeactivated()
{
	m_pChannel2D->setPaused(true);
}
