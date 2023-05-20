#include "stdafx.h"
#include "MainMenuScene.h"
#include "Prefabs/Button.h"
#include "MarioScene.h"

void MainMenuScene::Initialize()
{
	m_SceneContext.settings.enableOnGUI = true;
	m_SceneContext.settings.clearColor = { 0,0,0,1 };

	m_SceneContext.pInput->ForceMouseToCenter(false);
	m_SceneContext.settings.drawPhysXDebug = false;

	auto pCamera = AddChild(new FixedCamera)->GetComponent<CameraComponent>();

	SetActiveCamera(pCamera);


	auto pStart = AddChild(new Button(L"Textures/Mario/Main Menu/Start.png",
		[&]() {
			SceneManager::Get()->SetActiveGameScene(L"MarioScene");
		}));
	m_Buttons.emplace_back(pStart);
	pStart->GetTransform()->Translate(290, 550, 0.7f);



	auto pQuit = AddChild(new Button(L"Textures/Mario/Main Menu/Quit.png", [&]() {OverlordGame::Quit(); }));
	pQuit->GetTransform()->Translate(820, 550, 0.5f);

	m_Buttons.emplace_back(pQuit);


	auto pBG = AddChild(new GameObject);
	pBG->AddComponent(new SpriteComponent(L"Textures/Mario/Main Menu/Main Menu.png"));
	pBG->GetTransform()->Translate(0, 0, 0.9f);

	PositionTemp = pStart;


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



	if (PositionTemp)
	{

		auto curPos = PositionTemp->GetTransform()->GetWorldPosition();
		float pos[3]{ curPos.x, curPos.y, curPos.z };

		ImGui::DragFloat3("Translation", pos, 1, -300, 2000);
		PositionTemp->GetTransform()->Translate(pos[0], pos[1], pos[2]);
	}
}
