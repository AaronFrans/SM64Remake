#include "stdafx.h"
#include "MarioScene.h"
#include "Prefabs/ThirdPersonCharacter.h"

#include "Materials/EntityMaterial.h"
#include "Materials/UberMaterial.h"
#include "Materials/ColorMaterial.h"

void MarioScene::Initialize()
{
	m_SceneContext.settings.enableOnGUI = true;


	auto& physX = PxGetPhysics();
	auto pMat = physX.createMaterial(.5f, .5f, 0);

	//Mario
	MakeMario(pMat);



	//Level
	MakeLevel(pMat);

	//pModel->SetMaterial(pEntityTest);


	//Input
	auto inputAction = InputAction(CharacterMoveLeft, InputState::down, 'A');
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(CharacterMoveRight, InputState::down, 'D');
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(CharacterMoveForward, InputState::down, 'W');
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(CharacterMoveBackward, InputState::down, 'S');
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(CharacterJump, InputState::pressed, VK_SPACE, -1, XINPUT_GAMEPAD_A);
	m_SceneContext.pInput->AddInputAction(inputAction);



	//GameSceneExt::CreatePhysXGroundPlane(*this, pMat);
}

void MarioScene::OnGUI()
{
	if (m_pDebugMat)
		m_pDebugMat->DrawImGui();
}

void MarioScene::MakeMario(physx::PxMaterial* pPhysicsMaterial)
{


	const auto pMarioMat = MaterialManager::Get()->CreateMaterial<EntityMaterial>();

	pMarioMat->SetDiffuseTexture(L"Textures/Mario/Mario/MarioDiffuse.png");
	pMarioMat->SetOpacityTexture(L"Textures/Mario/Mario/MarioOpacity.png");

	CharacterDesc characterDesc{ pPhysicsMaterial };
	characterDesc.actionId_MoveForward = CharacterMoveForward;
	characterDesc.actionId_MoveBackward = CharacterMoveBackward;
	characterDesc.actionId_MoveLeft = CharacterMoveLeft;
	characterDesc.actionId_MoveRight = CharacterMoveRight;
	characterDesc.actionId_Jump = CharacterJump;

	const auto pObject = AddChild(new ThirdPersonCharacter(characterDesc));

	const auto pModel = pObject->AddComponent(new ModelComponent(L"Meshes/Mario/MarioModel/Mario.ovm"));
	pModel->SetMaterial(pMarioMat);

	//pModel->GetTransform()->Scale({ CorrectScale.x, CorrectScale.y, CorrectScale.z });
	pObject->GetTransform()->Scale({ CorrectScale.x, CorrectScale.y, CorrectScale.z });

	pObject->GetTransform()->Translate(0.f, 5.f, 0.f);

}

void MarioScene::MakeLevel(physx::PxMaterial* pPhysicsMaterial)
{


	const auto pCastleRoot = AddChild(new GameObject());
	auto pRootTransform = pCastleRoot->GetTransform();
	pRootTransform->Translate(0, 0.25f, 0);

	//Grass 1
	auto pGrass1Mat = MaterialManager::Get()->CreateMaterial<UberMaterial>();
	pGrass1Mat->SetDiffuseTexture(L"Textures/Mario/Castle/Grass1.png");
	pGrass1Mat->SetUseNormalMaps(false);
	pGrass1Mat->SetUseSpecularTexture(false);

	const auto pGrass1 = pCastleRoot->AddChild(new GameObject());
	const auto pGrass1Model = pGrass1->AddComponent(new ModelComponent(L"Meshes/Mario/CastleModels/Grass1.ovm"));
	pGrass1->GetTransform()->Scale({ CorrectScale.x, CorrectScale.y, CorrectScale.z });


	const auto pGrass1Mesh = ContentManager::Load<PxTriangleMesh>(L"Meshes/Mario/CastleModels/Grass1.ovpt");

	auto pGrass1RB = pGrass1->AddComponent(new RigidBodyComponent(true));
	pGrass1RB->AddCollider(PxTriangleMeshGeometry{ pGrass1Mesh, PxMeshScale{{ CorrectScale.x, CorrectScale.y, CorrectScale.z }} }, *pPhysicsMaterial);
	pGrass1Model->SetMaterial(pGrass1Mat);



	//Grass 2
	auto pGrass2Mat = MaterialManager::Get()->CreateMaterial<UberMaterial>();
	pGrass2Mat->SetDiffuseTexture(L"Textures/Mario/Castle/Grass2.png");
	pGrass2Mat->SetUseNormalMaps(false);
	pGrass2Mat->SetUseSpecularTexture(false);

	const auto pGrass2 = pCastleRoot->AddChild(new GameObject());
	const auto pGrass2Model = pGrass2->AddComponent(new ModelComponent(L"Meshes/Mario/CastleModels/Grass2.ovm"));
	pGrass2->GetTransform()->Scale({ CorrectScale.x, CorrectScale.y, CorrectScale.z });


	const auto pGrass2Mesh = ContentManager::Load<PxTriangleMesh>(L"Meshes/Mario/CastleModels/Grass2.ovpt");

	auto pGrass2RB = pGrass2->AddComponent(new RigidBodyComponent(true));
	pGrass2RB->AddCollider(PxTriangleMeshGeometry{ pGrass2Mesh, PxMeshScale{{ CorrectScale.x, CorrectScale.y, CorrectScale.z }} }, *pPhysicsMaterial);

	pGrass2Model->SetMaterial(pGrass2Mat);


	//Water
	//auto pWaterMat = MaterialManager::Get()->CreateMaterial<UberMaterial>();
	//pWaterMat->SetDiffuseTexture(L"Textures/Mario/Castle/Water.png");
	//pWaterMat->SetUseNormalMaps(false);
	//pWaterMat->SetUseSpecularTexture(false);
	//pWaterMat->SetOpacityIntensity(0.75f);
	//
	//const auto pWater = pCastleRoot->AddChild(new GameObject());
	//const auto pWaterModel = pWater->AddComponent(new ModelComponent(L"Meshes/Mario/CastleModels/Water.ovm"));
	//pWater->GetTransform()->Scale({ CorrectScale.x, CorrectScale.y, CorrectScale.z });
	//
	//
	//const auto pWaterMesh = ContentManager::Load<PxTriangleMesh>(L"Meshes/Mario/CastleModels/Water.ovpt");
	//
	//auto pWaterRB = pWater->AddComponent(new RigidBodyComponent(true));
	//pWaterRB->AddCollider(PxTriangleMeshGeometry{ pWaterMesh, PxMeshScale{{ CorrectScale.x, CorrectScale.y, CorrectScale.z }} }, *pPhysicsMaterial);
	//
	//pWaterModel->SetMaterial(pWaterMat);


	//Sand
	auto pSandMat = MaterialManager::Get()->CreateMaterial<UberMaterial>();
	pSandMat->SetDiffuseTexture(L"Textures/Mario/Castle/Sand.png");
	pSandMat->SetUseNormalMaps(false);
	pSandMat->SetUseSpecularTexture(false);

	const auto pSand = pCastleRoot->AddChild(new GameObject());
	const auto pSandModel = pSand->AddComponent(new ModelComponent(L"Meshes/Mario/CastleModels/Sand.ovm"));
	pSand->GetTransform()->Scale({ CorrectScale.x, CorrectScale.y, CorrectScale.z });


	const auto pSandMesh = ContentManager::Load<PxTriangleMesh>(L"Meshes/Mario/CastleModels/Sand.ovpt");

	auto pSandRB = pSand->AddComponent(new RigidBodyComponent(true));
	pSandRB->AddCollider(PxTriangleMeshGeometry{ pSandMesh, PxMeshScale{{ CorrectScale.x, CorrectScale.y, CorrectScale.z }} }, *pPhysicsMaterial);

	pSandModel->SetMaterial(pSandMat);


	//BridgeWalkWay
	auto pBridgeWalkWayMat = MaterialManager::Get()->CreateMaterial<UberMaterial>();
	pBridgeWalkWayMat->SetDiffuseTexture(L"Textures/Mario/Castle/BridgeWalkWay.png");
	pBridgeWalkWayMat->SetUseNormalMaps(false);
	pBridgeWalkWayMat->SetUseSpecularTexture(false);

	const auto pBridgeWalkWay = pCastleRoot->AddChild(new GameObject());
	const auto pBridgeWalkWayModel = pBridgeWalkWay->AddComponent(new ModelComponent(L"Meshes/Mario/CastleModels/BridgeWalkWay.ovm"));
	pBridgeWalkWay->GetTransform()->Scale({ CorrectScale.x, CorrectScale.y, CorrectScale.z });


	const auto pBridgeWalkWayMesh = ContentManager::Load<PxTriangleMesh>(L"Meshes/Mario/CastleModels/BridgeWalkWay.ovpt");

	auto pBridgeWalkWayRB = pBridgeWalkWay->AddComponent(new RigidBodyComponent(true));
	pBridgeWalkWayRB->AddCollider(PxTriangleMeshGeometry{ pBridgeWalkWayMesh, PxMeshScale{{ CorrectScale.x, CorrectScale.y, CorrectScale.z }} }, *pPhysicsMaterial);

	pBridgeWalkWayModel->SetMaterial(pBridgeWalkWayMat);

	//Details
	auto pDetailsMat = MaterialManager::Get()->CreateMaterial<UberMaterial>();
	pDetailsMat->SetDiffuseTexture(L"Textures/Mario/Castle/Details.png");
	pDetailsMat->SetUseNormalMaps(false);
	pDetailsMat->SetUseSpecularTexture(false);

	const auto pDetails = pCastleRoot->AddChild(new GameObject());
	const auto pDetailsModel = pDetails->AddComponent(new ModelComponent(L"Meshes/Mario/CastleModels/Details.ovm"));
	pDetails->GetTransform()->Scale({ CorrectScale.x, CorrectScale.y, CorrectScale.z });


	const auto pDetailsMesh = ContentManager::Load<PxTriangleMesh>(L"Meshes/Mario/CastleModels/Details.ovpt");

	auto pDetailsRB = pDetails->AddComponent(new RigidBodyComponent(true));
	pDetailsRB->AddCollider(PxTriangleMeshGeometry{ pDetailsMesh, PxMeshScale{{ CorrectScale.x, CorrectScale.y, CorrectScale.z }} }, *pPhysicsMaterial);

	pDetailsModel->SetMaterial(pDetailsMat);

	//Fences
	auto pFencesMat = MaterialManager::Get()->CreateMaterial<UberMaterial>();
	pFencesMat->SetDiffuseTexture(L"Textures/Mario/Castle/Fences.png");
	pFencesMat->SetUseNormalMaps(false);
	pFencesMat->SetUseSpecularTexture(false);
	pFencesMat->SetOpacityTexture(L"Textures/Mario/Castle/Fences_Opacity.png");
	pFencesMat->SetUseOpacity(true);

	const auto pFences = pCastleRoot->AddChild(new GameObject());
	const auto pFencesModel = pFences->AddComponent(new ModelComponent(L"Meshes/Mario/CastleModels/Fences.ovm"));
	pFences->GetTransform()->Scale({ CorrectScale.x, CorrectScale.y, CorrectScale.z });


	const auto pFencesMesh = ContentManager::Load<PxTriangleMesh>(L"Meshes/Mario/CastleModels/Fences.ovpt");

	auto pFencesRB = pFences->AddComponent(new RigidBodyComponent(true));
	pFencesRB->AddCollider(PxTriangleMeshGeometry{ pFencesMesh, PxMeshScale{{ CorrectScale.x, CorrectScale.y, CorrectScale.z }} }, *pPhysicsMaterial);

	pFencesModel->SetMaterial(pFencesMat);


	m_pDebugMat = pFencesMat;

	//Grates
	auto pGratesMat = MaterialManager::Get()->CreateMaterial<UberMaterial>();
	pGratesMat->SetDiffuseTexture(L"Textures/Mario/Castle/Grates.png");
	pGratesMat->SetUseNormalMaps(false);
	pGratesMat->SetUseSpecularTexture(false);
	pGratesMat->SetOpacityTexture(L"Textures/Mario/Castle/Grates_Opacity.png");
	pGratesMat->SetUseOpacity(true);

	const auto pGrates = pCastleRoot->AddChild(new GameObject());
	const auto pGratesModel = pGrates->AddComponent(new ModelComponent(L"Meshes/Mario/CastleModels/Grates.ovm"));
	pGrates->GetTransform()->Scale({ CorrectScale.x, CorrectScale.y, CorrectScale.z });


	const auto pGratesMesh = ContentManager::Load<PxTriangleMesh>(L"Meshes/Mario/CastleModels/Grates.ovpt");

	auto pGratesRB = pGrates->AddComponent(new RigidBodyComponent(true));
	pGratesRB->AddCollider(PxTriangleMeshGeometry{ pGratesMesh, PxMeshScale{{ CorrectScale.x, CorrectScale.y, CorrectScale.z }} }, *pPhysicsMaterial);

	pGratesModel->SetMaterial(pGratesMat);

	//Rocks
	auto pRocksMat = MaterialManager::Get()->CreateMaterial<UberMaterial>();
	pRocksMat->SetDiffuseTexture(L"Textures/Mario/Castle/Rocks.png");
	pRocksMat->SetUseNormalMaps(false);
	pRocksMat->SetUseSpecularTexture(false);

	const auto pRocks = pCastleRoot->AddChild(new GameObject());
	const auto pRocksModel = pRocks->AddComponent(new ModelComponent(L"Meshes/Mario/CastleModels/Rocks.ovm"));
	pRocks->GetTransform()->Scale({ CorrectScale.x, CorrectScale.y, CorrectScale.z });


	const auto pRocksMesh = ContentManager::Load<PxTriangleMesh>(L"Meshes/Mario/CastleModels/Rocks.ovpt");

	auto pRocksRB = pRocks->AddComponent(new RigidBodyComponent(true));
	pRocksRB->AddCollider(PxTriangleMeshGeometry{ pRocksMesh, PxMeshScale{{ CorrectScale.x, CorrectScale.y, CorrectScale.z }} }, *pPhysicsMaterial);

	pRocksModel->SetMaterial(pRocksMat);


	m_pDebugMat = pRocksMat;

	//RocksUnderWater
	auto pRocksUnderWaterMat = MaterialManager::Get()->CreateMaterial<UberMaterial>();
	pRocksUnderWaterMat->SetDiffuseTexture(L"Textures/Mario/Castle/RocksUnderWater.png");
	pRocksUnderWaterMat->SetUseNormalMaps(false);
	pRocksUnderWaterMat->SetUseSpecularTexture(false);

	const auto pRocksUnderWater = pCastleRoot->AddChild(new GameObject());
	const auto pRocksUnderWaterModel = pRocksUnderWater->AddComponent(new ModelComponent(L"Meshes/Mario/CastleModels/RocksUnderWater.ovm"));
	pRocksUnderWater->GetTransform()->Scale({ CorrectScale.x, CorrectScale.y, CorrectScale.z });


	const auto pRocksUnderWaterMesh = ContentManager::Load<PxTriangleMesh>(L"Meshes/Mario/CastleModels/RocksUnderWater.ovpt");

	auto pRocksUnderWaterRB = pRocksUnderWater->AddComponent(new RigidBodyComponent(true));
	pRocksUnderWaterRB->AddCollider(PxTriangleMeshGeometry{ pRocksUnderWaterMesh, PxMeshScale{{ CorrectScale.x, CorrectScale.y, CorrectScale.z }} }, *pPhysicsMaterial);

	pRocksUnderWaterModel->SetMaterial(pRocksUnderWaterMat);

	//UnderWaterSand
	auto pUnderWaterSandMat = MaterialManager::Get()->CreateMaterial<UberMaterial>();
	pUnderWaterSandMat->SetDiffuseTexture(L"Textures/Mario/Castle/UnderWaterSand.png");
	pUnderWaterSandMat->SetUseNormalMaps(false);
	pUnderWaterSandMat->SetUseSpecularTexture(false);

	const auto pUnderWaterSand = pCastleRoot->AddChild(new GameObject());
	const auto pUnderWaterSandModel = pUnderWaterSand->AddComponent(new ModelComponent(L"Meshes/Mario/CastleModels/UnderWaterSand.ovm"));
	pUnderWaterSand->GetTransform()->Scale({ CorrectScale.x, CorrectScale.y, CorrectScale.z });


	const auto pUnderWaterSandMesh = ContentManager::Load<PxTriangleMesh>(L"Meshes/Mario/CastleModels/UnderWaterSand.ovpt");

	auto pUnderWaterSandRB = pUnderWaterSand->AddComponent(new RigidBodyComponent(true));
	pUnderWaterSandRB->AddCollider(PxTriangleMeshGeometry{ pUnderWaterSandMesh, PxMeshScale{{ CorrectScale.x, CorrectScale.y, CorrectScale.z }} }, *pPhysicsMaterial);

	pUnderWaterSandModel->SetMaterial(pUnderWaterSandMat);


	//Cobble
	auto pCobbleMat = MaterialManager::Get()->CreateMaterial<UberMaterial>();
	pCobbleMat->SetDiffuseTexture(L"Textures/Mario/Castle/Cobble.png");
	pCobbleMat->SetUseNormalMaps(false);
	pCobbleMat->SetUseSpecularTexture(false);

	const auto pCobble = pCastleRoot->AddChild(new GameObject());
	const auto pCobbleModel = pCobble->AddComponent(new ModelComponent(L"Meshes/Mario/CastleModels/Cobble.ovm"));
	pCobble->GetTransform()->Scale({ CorrectScale.x, CorrectScale.y, CorrectScale.z });


	const auto pCobbleMesh = ContentManager::Load<PxTriangleMesh>(L"Meshes/Mario/CastleModels/Cobble.ovpt");

	auto pCobbleRB = pCobble->AddComponent(new RigidBodyComponent(true));
	pCobbleRB->AddCollider(PxTriangleMeshGeometry{ pCobbleMesh, PxMeshScale{{ CorrectScale.x, CorrectScale.y, CorrectScale.z }} }, *pPhysicsMaterial);

	pCobbleModel->SetMaterial(pCobbleMat);

	//Boulders
	auto pBouldersMat = MaterialManager::Get()->CreateMaterial<UberMaterial>();
	pBouldersMat->SetDiffuseTexture(L"Textures/Mario/Castle/Boulders.png");
	pBouldersMat->SetUseNormalMaps(false);
	pBouldersMat->SetUseSpecularTexture(false);

	const auto pBoulders = pCastleRoot->AddChild(new GameObject());
	const auto pBouldersModel = pBoulders->AddComponent(new ModelComponent(L"Meshes/Mario/CastleModels/Boulders.ovm"));
	pBoulders->GetTransform()->Scale({ CorrectScale.x, CorrectScale.y, CorrectScale.z });


	const auto pBouldersMesh = ContentManager::Load<PxTriangleMesh>(L"Meshes/Mario/CastleModels/Boulders.ovpt");

	auto pBouldersRB = pBoulders->AddComponent(new RigidBodyComponent(true));
	pBouldersRB->AddCollider(PxTriangleMeshGeometry{ pBouldersMesh, PxMeshScale{{ CorrectScale.x, CorrectScale.y, CorrectScale.z }} }, *pPhysicsMaterial);

	pBouldersModel->SetMaterial(pBouldersMat);



	//Grates
	auto pWindowsMat = MaterialManager::Get()->CreateMaterial<UberMaterial>();
	pWindowsMat->SetDiffuseTexture(L"Textures/Mario/Castle/Windows.png");
	pWindowsMat->SetUseNormalMaps(false);
	pWindowsMat->SetUseSpecularTexture(false);
	pWindowsMat->SetOpacityTexture(L"Textures/Mario/Castle/Windows_Opacity.png");
	pWindowsMat->SetUseOpacity(true);

	const auto pWindows = pCastleRoot->AddChild(new GameObject());
	const auto pWindowsModel = pWindows->AddComponent(new ModelComponent(L"Meshes/Mario/CastleModels/Windows.ovm"));
	pWindows->GetTransform()->Scale({ CorrectScale.x, CorrectScale.y, CorrectScale.z });


	const auto pWindowsMesh = ContentManager::Load<PxTriangleMesh>(L"Meshes/Mario/CastleModels/Windows.ovpt");

	auto pWindowsRB = pWindows->AddComponent(new RigidBodyComponent(true));
	pWindowsRB->AddCollider(PxTriangleMeshGeometry{ pWindowsMesh, PxMeshScale{{ CorrectScale.x, CorrectScale.y, CorrectScale.z }} }, *pPhysicsMaterial);

	pWindowsModel->SetMaterial(pWindowsMat);



	//UnderWaterBricks
	auto pUnderWaterBricksMat = MaterialManager::Get()->CreateMaterial<UberMaterial>();
	pUnderWaterBricksMat->SetDiffuseTexture(L"Textures/Mario/Castle/UnderWaterBricks.png");
	pUnderWaterBricksMat->SetUseNormalMaps(false);
	pUnderWaterBricksMat->SetUseSpecularTexture(false);

	const auto pUnderWaterBricks = pCastleRoot->AddChild(new GameObject());
	const auto pUnderWaterBricksModel = pUnderWaterBricks->AddComponent(new ModelComponent(L"Meshes/Mario/CastleModels/UnderWaterBricks.ovm"));
	pUnderWaterBricks->GetTransform()->Scale({ CorrectScale.x, CorrectScale.y, CorrectScale.z });


	const auto pUnderWaterBricksMesh = ContentManager::Load<PxTriangleMesh>(L"Meshes/Mario/CastleModels/UnderWaterBricks.ovpt");

	auto pUnderWaterBricksRB = pUnderWaterBricks->AddComponent(new RigidBodyComponent(true));
	pUnderWaterBricksRB->AddCollider(PxTriangleMeshGeometry{ pUnderWaterBricksMesh, PxMeshScale{{ CorrectScale.x, CorrectScale.y, CorrectScale.z }} }, *pPhysicsMaterial);

	pUnderWaterBricksModel->SetMaterial(pUnderWaterBricksMat);


	//Waterfall
	auto pWaterfallMat = MaterialManager::Get()->CreateMaterial<UberMaterial>();
	pWaterfallMat->SetDiffuseTexture(L"Textures/Mario/Castle/Water.png");
	pWaterfallMat->SetUseNormalMaps(false);
	pWaterfallMat->SetUseSpecularTexture(false);

	const auto pWaterfall = pCastleRoot->AddChild(new GameObject());
	const auto pWaterfallModel = pWaterfall->AddComponent(new ModelComponent(L"Meshes/Mario/CastleModels/Waterfall.ovm"));
	pWaterfall->GetTransform()->Scale({ CorrectScale.x, CorrectScale.y, CorrectScale.z });


	const auto pWaterfallMesh = ContentManager::Load<PxTriangleMesh>(L"Meshes/Mario/CastleModels/Waterfall.ovpt");

	auto pWaterfallRB = pWaterfall->AddComponent(new RigidBodyComponent(true));
	pWaterfallRB->AddCollider(PxTriangleMeshGeometry{ pWaterfallMesh, PxMeshScale{{ CorrectScale.x, CorrectScale.y, CorrectScale.z }} }, *pPhysicsMaterial);

	pWaterfallModel->SetMaterial(pWaterfallMat);


	//Castle
	auto pCastleMat = MaterialManager::Get()->CreateMaterial<UberMaterial>();
	pCastleMat->SetDiffuseTexture(L"Textures/Mario/Castle/Castle.png");
	pCastleMat->SetUseNormalMaps(false);
	pCastleMat->SetUseSpecularTexture(false);

	const auto pCastle = pCastleRoot->AddChild(new GameObject());
	const auto pCastleModel = pCastle->AddComponent(new ModelComponent(L"Meshes/Mario/CastleModels/Castle.ovm"));
	pCastle->GetTransform()->Scale({ CorrectScale.x, CorrectScale.y, CorrectScale.z });


	const auto pCastleMesh = ContentManager::Load<PxTriangleMesh>(L"Meshes/Mario/CastleModels/Castle.ovpt");

	auto pCastleRB = pCastle->AddComponent(new RigidBodyComponent(true));
	pCastleRB->AddCollider(PxTriangleMeshGeometry{ pCastleMesh, PxMeshScale{{ CorrectScale.x, CorrectScale.y, CorrectScale.z }} }, *pPhysicsMaterial);

	pCastleModel->SetMaterial(pCastleMat);


	//Roof
	auto pRoofMat = MaterialManager::Get()->CreateMaterial<UberMaterial>();
	pRoofMat->SetDiffuseTexture(L"Textures/Mario/Castle/Roof.png");
	pRoofMat->SetUseNormalMaps(false);
	pRoofMat->SetUseSpecularTexture(false);

	const auto pRoof = pCastleRoot->AddChild(new GameObject());
	const auto pRoofModel = pRoof->AddComponent(new ModelComponent(L"Meshes/Mario/CastleModels/Roof.ovm"));
	pRoof->GetTransform()->Scale({ CorrectScale.x, CorrectScale.y, CorrectScale.z });


	const auto pRoofMesh = ContentManager::Load<PxTriangleMesh>(L"Meshes/Mario/CastleModels/Roof.ovpt");

	auto pRoofRB = pRoof->AddComponent(new RigidBodyComponent(true));
	pRoofRB->AddCollider(PxTriangleMeshGeometry{ pRoofMesh, PxMeshScale{{ CorrectScale.x, CorrectScale.y, CorrectScale.z }} }, *pPhysicsMaterial);

	pRoofModel->SetMaterial(pRoofMat);


	//Carpet
	auto pCarpetMat = MaterialManager::Get()->CreateMaterial<UberMaterial>();
	pCarpetMat->SetDiffuseTexture(L"Textures/Mario/Castle/Carpet.png");
	pCarpetMat->SetUseNormalMaps(false);
	pCarpetMat->SetUseSpecularTexture(false);

	const auto pCarpet = pCastleRoot->AddChild(new GameObject());
	pCarpet->GetTransform()->Translate(0, 0.01f, 0);
	const auto pCarpetModel = pCarpet->AddComponent(new ModelComponent(L"Meshes/Mario/CastleModels/Carpet.ovm"));
	pCarpet->GetTransform()->Scale({ CorrectScale.x, CorrectScale.y, CorrectScale.z });


	const auto pCarpetMesh = ContentManager::Load<PxTriangleMesh>(L"Meshes/Mario/CastleModels/Carpet.ovpt");

	auto pCarpetRB = pCarpet->AddComponent(new RigidBodyComponent(true));
	pCarpetRB->AddCollider(PxTriangleMeshGeometry{ pCarpetMesh, PxMeshScale{{ CorrectScale.x, CorrectScale.y, CorrectScale.z }} }, *pPhysicsMaterial);

	pCarpetModel->SetMaterial(pCarpetMat);


	//BridgeTop
	auto pBridgeTopMat = MaterialManager::Get()->CreateMaterial<UberMaterial>();
	pBridgeTopMat->SetDiffuseTexture(L"Textures/Mario/Castle/BridgeTop.png");
	pBridgeTopMat->SetUseNormalMaps(false);
	pBridgeTopMat->SetUseSpecularTexture(false);

	const auto pBridgeTop = pCastleRoot->AddChild(new GameObject());
	const auto pBridgeTopModel = pBridgeTop->AddComponent(new ModelComponent(L"Meshes/Mario/CastleModels/BridgeTop.ovm"));
	pBridgeTop->GetTransform()->Scale({ CorrectScale.x, CorrectScale.y, CorrectScale.z });


	const auto pBridgeTopMesh = ContentManager::Load<PxTriangleMesh>(L"Meshes/Mario/CastleModels/BridgeTop.ovpt");

	auto pBridgeTopRB = pBridgeTop->AddComponent(new RigidBodyComponent(true));
	pBridgeTopRB->AddCollider(PxTriangleMeshGeometry{ pBridgeTopMesh, PxMeshScale{{ CorrectScale.x, CorrectScale.y, CorrectScale.z }} }, * pPhysicsMaterial);

	pBridgeTopModel->SetMaterial(pBridgeTopMat);

	//BridgeSide
	auto pBridgeSideMat = MaterialManager::Get()->CreateMaterial<UberMaterial>();
	pBridgeSideMat->SetDiffuseTexture(L"Textures/Mario/Castle/BridgeSide.png");
	pBridgeSideMat->SetUseNormalMaps(false);
	pBridgeSideMat->SetUseSpecularTexture(false);

	const auto pBridgeSide = pCastleRoot->AddChild(new GameObject());
	const auto pBridgeSideModel = pBridgeSide->AddComponent(new ModelComponent(L"Meshes/Mario/CastleModels/BridgeSide.ovm"));
	pBridgeTop->GetTransform()->Scale({ CorrectScale.x, CorrectScale.y, CorrectScale.z });


	const auto pBridgeSideMesh = ContentManager::Load<PxTriangleMesh>(L"Meshes/Mario/CastleModels/BridgeSide.ovpt");

	auto pBridgeSideRB = pBridgeSide->AddComponent(new RigidBodyComponent(true));
	pBridgeSideRB->AddCollider(PxTriangleMeshGeometry{ pBridgeSideMesh, PxMeshScale{{ CorrectScale.x, CorrectScale.y, CorrectScale.z }} }, * pPhysicsMaterial);

	pBridgeSideModel->SetMaterial(pBridgeSideMat);


	//PeachTop
	auto pPeachTopMat = MaterialManager::Get()->CreateMaterial<UberMaterial>();
	pPeachTopMat->SetDiffuseTexture(L"Textures/Mario/Castle/PeachTop.png");
	pPeachTopMat->SetUseNormalMaps(false);
	pPeachTopMat->SetUseSpecularTexture(false);
	pPeachTopMat->SetOpacityTexture(L"Textures/Mario/Castle/PeachTop_Opacity.png");
	pPeachTopMat->SetUseOpacity(true);

	const auto pPeachTop = pCastleRoot->AddChild(new GameObject());
	const auto pPeachTopModel = pPeachTop->AddComponent(new ModelComponent(L"Meshes/Mario/CastleModels/PeachTop.ovm"));
	pPeachTop->GetTransform()->Scale({ CorrectScale.x, CorrectScale.y, CorrectScale.z });


	const auto pPeachTopMesh = ContentManager::Load<PxTriangleMesh>(L"Meshes/Mario/CastleModels/PeachTop.ovpt");

	auto pPeachTopRB = pPeachTop->AddComponent(new RigidBodyComponent(true));
	pPeachTopRB->AddCollider(PxTriangleMeshGeometry{ pPeachTopMesh, PxMeshScale{{ CorrectScale.x, CorrectScale.y, CorrectScale.z }} }, * pPhysicsMaterial);

	pPeachTopModel->SetMaterial(pPeachTopMat);


	//PeachBottom
	auto pPeachBottomMat = MaterialManager::Get()->CreateMaterial<UberMaterial>();
	pPeachBottomMat->SetDiffuseTexture(L"Textures/Mario/Castle/PeachBottom.png");
	pPeachBottomMat->SetUseNormalMaps(false);
	pPeachBottomMat->SetUseSpecularTexture(false);
	pPeachBottomMat->SetOpacityTexture(L"Textures/Mario/Castle/PeachBottom_Opacity.png");
	pPeachBottomMat->SetUseOpacity(true);

	const auto pPeachBottom = pCastleRoot->AddChild(new GameObject());
	const auto pPeachBottomModel = pPeachBottom->AddComponent(new ModelComponent(L"Meshes/Mario/CastleModels/PeachBottom.ovm"));
	pPeachBottom->GetTransform()->Scale({ CorrectScale.x, CorrectScale.y, CorrectScale.z });


	const auto pPeachBottomMesh = ContentManager::Load<PxTriangleMesh>(L"Meshes/Mario/CastleModels/PeachBottom.ovpt");

	auto pPeachBottomRB = pPeachBottom->AddComponent(new RigidBodyComponent(true));
	pPeachBottomRB->AddCollider(PxTriangleMeshGeometry{ pPeachBottomMesh, PxMeshScale{{ CorrectScale.x, CorrectScale.y, CorrectScale.z }} }, * pPhysicsMaterial);

	pPeachBottomModel->SetMaterial(pPeachBottomMat);

	//m_pDebugMat = pWaterMat;
}
