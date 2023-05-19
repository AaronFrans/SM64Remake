#include "stdafx.h"
#include "MainMenuScene.h"

void MainMenuScene::Initialize()
{
	m_SceneContext.settings.enableOnGUI = true;
	m_SceneContext.settings.clearColor = { 0,0,0,1 };
	//const auto pMaterial = PxGetPhysics().createMaterial(.5f, .5f, .5f);

	m_SceneContext.pInput->ForceMouseToCenter(false);
	m_SceneContext.settings.drawPhysXDebug = false;

	auto pCamera = AddChild(new FixedCamera)->GetComponent<CameraComponent>();


	//pCamera->SetFieldOfView(XMConvertToRadians(80.0f));

	SetActiveCamera(pCamera);

	auto pStart = AddChild(new GameObject);
	pStart->AddComponent(new SpriteComponent(L"Textures/Mario/Main Menu/Start.png"));

	AddChild(new GameObject)->AddComponent(new SpriteComponent(L"Textures/Mario/Main Menu/Main Menu.png"));
	//pBackground->GetTransform()->Scale({ 2, 1.6f, 1 });


	m_pFont = ContentManager::Load<SpriteFont>(L"SpriteFonts/Mario/Mario64.fnt");


}

void MainMenuScene::Update()
{
	if (InputManager::IsMouseButton(InputState::pressed, VK_LBUTTON))
	{
		if (const auto pPickedObject = m_SceneContext.pCamera->Pick())
		{
			std::cout << "clicked button";
		}
	}

	TextRenderer::Get()->DrawText(m_pFont, StringUtil::utf8_decode(m_Text), m_TextPosition, m_TextColor);
}

void MainMenuScene::OnGUI()
{
	char buffer[256]{};
	m_Text.copy(&buffer[0], 256);
	if (ImGui::InputText("Text", &buffer[0], 256))
	{
		m_Text = std::string(buffer);
	}

	ImGui::SliderFloat2("Position", &m_TextPosition.x, 0, m_SceneContext.windowWidth);
	ImGui::ColorEdit4("Color", &m_TextColor.x, ImGuiColorEditFlags_NoInputs);
}
