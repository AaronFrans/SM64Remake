//Precompiled Header [ALWAYS ON TOP IN CPP]
#include "stdafx.h"
#include "MinionScene.h"

#include "Prefabs/CubePrefab.h"
#include "Prefabs/SpherePrefab.h"
#include "Prefabs/TorusPrefab.h"


MinionScene::MinionScene() :
	GameScene(L"MinionScene") {}

void MinionScene::Initialize()
{
	//m_SceneContext.settings.showInfoOverlay = true;
	//m_SceneContext.settings.drawPhysXDebug = true;
	//m_SceneContext.settings.drawGrid = true;
	m_SceneContext.settings.enableOnGUI = true;

	Logger::LogInfo(L"Welcome, humble Minion!");

	//Torus Big
	m_pTorusBig = new TorusPrefab(10, 50, 1, 20, XMFLOAT4{ Colors::Blue });
	AddChild(m_pTorusBig);
	
	//Torus Mid
	m_pTorusMid = new TorusPrefab( 7, 50, 1, 20, XMFLOAT4{ Colors::Red });
	AddChild(m_pTorusMid);
	
	//Torus Sml
	m_pTorusSml = new TorusPrefab(4, 50, 1, 20, XMFLOAT4{ Colors::Green });
	AddChild(m_pTorusSml);

}

void MinionScene::Update()
{
	//Optional
	const float rotation = 90 * m_SceneContext.pGameTime->GetTotal();
	m_pTorusBig->GetTransform()->Rotate(0, rotation * 1.5f, 0);
	m_pTorusMid->GetTransform()->Rotate(rotation * 1.5f, rotation, 0);
	m_pTorusSml->GetTransform()->Rotate(rotation * 1.5f, rotation * 1.5f, 0);
}

void MinionScene::Draw()
{
	//Optional
}

void MinionScene::OnGUI()
{
	ImGui::Text("This only activates if\n SceneSettings.enableOnGUI is True.\n\n");
	ImGui::Text("Use ImGui to add custom\n controllable scene parameters!");
	ImGui::ColorEdit3("Demo ClearColor", &m_SceneContext.settings.clearColor.x, ImGuiColorEditFlags_NoInputs);

}
