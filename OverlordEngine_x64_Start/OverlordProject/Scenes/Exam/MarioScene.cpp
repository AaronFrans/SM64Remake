#include "stdafx.h"
#include "MarioScene.h"
#include "Materials/EntityMaterial.h"

void MarioScene::Initialize()
{
	m_SceneContext.settings.enableOnGUI = true;


	auto& physX = PxGetPhysics();
	auto pMat = physX.createMaterial(.5f, .5f, .5f);

	const auto pEntityTest = MaterialManager::Get()->CreateMaterial<EntityMaterial>();

	pEntityTest->SetDiffuseTexture(L"Textures\\Mario\\Mario\\MarioDiffuse.png");
	pEntityTest->SetOpacityTexture(L"Textures\\Mario\\Mario\\MarioOpacity.png");

	const auto pObject = AddChild(new GameObject);
	const auto pModel = pObject->AddComponent(new ModelComponent(L"Meshes/Mario/MarioModel/Mario.ovm"));
	pModel->SetMaterial(pEntityTest);

	//pModel->SetMaterial(pEntityTest);
	pObject->GetTransform()->Scale(10.f, 10.f, 10.f);

	GameSceneExt::CreatePhysXGroundPlane(*this, pMat);
}

void MarioScene::OnGUI()
{
}
