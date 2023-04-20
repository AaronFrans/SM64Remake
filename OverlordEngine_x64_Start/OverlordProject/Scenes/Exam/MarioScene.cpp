#include "stdafx.h"
#include "MarioScene.h"
#include "Materials/DiffuseMaterial.h"

void MarioScene::Initialize()
{
	m_SceneContext.settings.enableOnGUI = true;


	auto& physX = PxGetPhysics();
	auto pMat = physX.createMaterial(.5f, .5f, .5f);

	const auto pEntityTest = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();
	pEntityTest->SetDiffuseTexture(L"Textures/Chair_Dark.dds");

	const auto pObject = AddChild(new GameObject);
	const auto pModel = pObject->AddComponent(new ModelComponent(L"Meshes/Mario/MarioModel/Mario.ovm"));
	pModel->SetMaterial(pEntityTest);

	pModel->SetMaterial(pEntityTest);

	GameSceneExt::CreatePhysXGroundPlane(*this, pMat);
}

void MarioScene::OnGUI()
{
}
