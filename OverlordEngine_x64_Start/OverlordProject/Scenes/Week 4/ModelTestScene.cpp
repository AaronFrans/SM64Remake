#include "stdafx.h"
#include <time.h>       
#include "ModelTestScene.h"

#include "Prefabs/CubePrefab.h"
#include "Prefabs/SpherePrefab.h"
#include "Prefabs/TorusPrefab.h"
#include "Prefabs/FixedCamera.h"
#include <Materials/ColorMaterial.h>
#include <Materials/DiffuseMaterial.h>


ModelTestScene::ModelTestScene() :
	GameScene(L"Model Test Scene") {}

void ModelTestScene::Initialize()
{
	auto& physX = PxGetPhysics();
	auto pMat = physX.createMaterial(.5f, .5f, .5f);

	DiffuseMaterial* pDiffuse = MaterialManager::Get()->CreateMaterial<DiffuseMaterial>();


	pDiffuse->SetDiffuseTexture(L"Textures/Chair_Dark.dds");

	const auto pModel = new ModelComponent(L"Meshes/Chair.ovm");

	pModel->SetMaterial(pDiffuse);

	m_pChair = new GameObject();
	m_pChair->AddComponent(pModel);

	m_pChair->GetTransform()->Translate(XMFLOAT3{ 0, 10, 0 });


	const auto pChairMesh = ContentManager::Load<PxConvexMesh>(L"Meshes/Chair.ovpc");

	

	auto pRB = m_pChair->AddComponent(new RigidBodyComponent());
	pRB->AddCollider(PxConvexMeshGeometry{ pChairMesh }, *pMat);

	pRB->SetDensity(1);

	AddChild(m_pChair);

	GameSceneExt::CreatePhysXGroundPlane(*this, pMat);
}