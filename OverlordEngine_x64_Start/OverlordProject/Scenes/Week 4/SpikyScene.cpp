#include "stdafx.h"
#include "SpikyScene.h"
#include "Materials/SpikyMaterial.h"

SpikyScene::SpikyScene() :
	GameScene(L"Spiky Scene")
{
}

void SpikyScene::Initialize()
{
	m_pSpiky = MaterialManager::Get()->CreateMaterial<SpikyMaterial>();

	m_pSphere = new GameObject();
	AddChild(m_pSphere);

	m_pSphere->AddComponent(new ModelComponent(L"Meshes/OctaSphere.ovm"))->SetMaterial(m_pSpiky);
	auto pTrans = m_pSphere->GetComponent<TransformComponent>();

	pTrans->Scale({ 15, 15, 15 });

	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.enableOnGUI = true;
}

void SpikyScene::Update()
{
	const float rotation = 20 * m_SceneContext.pGameTime->GetTotal();

	m_pSphere->GetTransform()->Rotate(0, rotation, 0);
}

void SpikyScene::OnGUI()
{
	m_pSpiky->DrawImGui();
}
