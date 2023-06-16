#include "stdafx.h"
#include "UberMaterialScene.h"
#include "Prefabs/SpherePrefab.h"
#include "Materials/UberMaterial.h"

UberMaterialScene::UberMaterialScene() :
	GameScene(L"Uber Material Scene")
{}

void UberMaterialScene::Initialize()
{
	m_pUberMaterial = MaterialManager::Get()->CreateMaterial<UberMaterial>();

	auto pSphere = new GameObject();
	AddChild(pSphere);

	pSphere->AddComponent(new ModelComponent(L"Meshes/Sphere.ovm"))->SetMaterial(m_pUberMaterial);
	auto pTrans = pSphere->GetComponent<TransformComponent>();

	pTrans->Scale({ 10, 10, 10 });

	m_SceneContext.settings.drawGrid = false;
	m_SceneContext.settings.enableOnGUI = true;
}

void UberMaterialScene::Update()
{
}

void UberMaterialScene::OnGUI()
{
	m_pUberMaterial->DrawImGui();
}
