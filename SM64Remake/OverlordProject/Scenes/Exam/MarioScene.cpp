#include "stdafx.h"
#include "MarioScene.h"
#include "Inputs.cpp"

#include "Prefabs/ThirdPersonCharacter.h"

#include "Materials/Mario/EntityMaterial_Skinned.h"
#include "Materials/UberMaterial.h"
#include "Materials/ColorMaterial.h"
#include "Materials/Mario/Post/PostUnderwater.h"
#include "Materials/Post/PostBlur.h"
#include "Materials/Mario/SkyboxMaterial.h"
#include "Materials/Mario/FlowingWater.h"


#include "Prefabs/Door.h"
#include "Prefabs/Coin.h"
#include "Prefabs/ParticleEmmiter.h"
#include "Prefabs/CubePrefab.h"
#include "Prefabs/Goomba.h"
#include "Prefabs/Button.h"

MarioScene::~MarioScene()
{
}

void MarioScene::RemoveHealth()
{
	if (m_pMario->IsDamaged())
		return;

	if (m_pHearts.size() == 0)
	{
		m_IsDead = true;
		return;
	}
	m_pHeartsHud->RemoveChild(m_pHearts.back(), true);
	m_pHearts.pop_back();


	m_pMario->Damage();
}

void MarioScene::Initialize()
{
	m_SceneContext.settings.enableOnGUI = true;
	m_SceneContext.settings.clearColor = { 0,0,0,1 };
	m_SceneContext.settings.drawUserDebug = false;
	m_SceneContext.settings.drawGrid = false;

	m_SceneContext.pInput->ForceMouseToCenter(true);
	m_SceneContext.settings.drawPhysXDebug = false;

	m_SceneContext.pLights->SetDirectionalLight({ -95.6139526f,66.1346436f,-41.1850471f }, { 0.740129888f, -0.597205281f, 0.309117377f });

	auto& physX = PxGetPhysics();
	m_pDefaultPhysxMat = physX.createMaterial(.5f, .5f, 0);
	m_pSlipperyPhysxMat = physX.createMaterial(0.0f, 0.0f, 0);

	ShadowMapRenderer::Get()->SetFarPlane(100);
	ShadowMapRenderer::Get()->SetNearPlane(-100);

	//Skybox
	MakeSkybox();

	//Mario
	MakeMario(m_pDefaultPhysxMat);

	MakeUnderwater(m_pDefaultPhysxMat);

	//Coins
	MakeCoins();

	//Goombas
	MakeGoombas();

	//Level
	MakeLevel();

	auto door = AddChild(new Door(m_pDefaultPhysxMat));
	door->GetTransform()->Translate(52.514f, -12.258f, -146.816f);
	door->GetTransform()->Rotate(96, -50, -27.5f);

	auto door2 = AddChild(new Door(m_pDefaultPhysxMat));
	door2->GetTransform()->Translate(52.702f, -12.16f, -146.689f);
	door2->GetTransform()->Rotate(115.9f, 161.1f, 9.3f);


	//Input
	auto inputAction = InputAction(Inputs::CharacterMoveLeft, InputState::down, 'A');
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(Inputs::CharacterMoveRight, InputState::down, 'D');
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(Inputs::CharacterMoveForward, InputState::down, 'W');
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(Inputs::CharacterMoveBackward, InputState::down, 'S');
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(Inputs::CharacterJump, InputState::pressed, VK_SPACE, -1, XINPUT_GAMEPAD_A);
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(Inputs::CharacterPunch, InputState::pressed, 'F', -1, XINPUT_GAMEPAD_B);
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(Inputs::CharacterSprint, InputState::down, VK_SHIFT, -1, XINPUT_GAMEPAD_LEFT_THUMB);
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(Inputs::PauseGame, InputState::pressed, VK_ESCAPE, -1, XINPUT_GAMEPAD_START);
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(Inputs::Left, InputState::pressed, -1, -1, XINPUT_GAMEPAD_DPAD_LEFT);
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(Inputs::Right, InputState::pressed, -1, -1, XINPUT_GAMEPAD_DPAD_RIGHT);
	m_SceneContext.pInput->AddInputAction(inputAction);

	inputAction = InputAction(Inputs::Press, InputState::pressed, -1, -1, XINPUT_GAMEPAD_A);
	m_SceneContext.pInput->AddInputAction(inputAction);



	//Bubbles
	MakeBubbles();

	MakeUI();

	MakePauseMenu();

	m_pFont = ContentManager::Load<SpriteFont>(L"SpriteFonts/Mario/Mario64.fnt");



	const auto pFmod = SoundManager::Get()->GetSystem();
	FMOD::Sound* pSound2D{ nullptr };
	auto result = pFmod->createStream("Resources/Sounds/Mario/Courtyard.mp3", FMOD_2D | FMOD_LOOP_NORMAL, nullptr, &pSound2D);
	result = pFmod->playSound(pSound2D, nullptr, false, &m_pChannel2D);
	m_pChannel2D->setPaused(true);
	m_pChannel2D->setVolume(0.5f);


	m_pControlls = AddChild(new GameObject)->AddComponent(new SpriteComponent(L"Textures/Mario/Controlls/Controlls.png"));
	m_pControlls->GetTransform()->Translate(0, 0, 0.9f);

}

void MarioScene::OnGUI()
{
	if (m_pDebugMat)
		m_pDebugMat->DrawImGui();

	char buffer[256]{};
	m_CoinsGotten.copy(&buffer[0], 256);
	if (ImGui::InputText("Text", &buffer[0], 256))
	{
		m_CoinsGotten = std::string(buffer);
	}

	ImGui::SliderFloat2("Position", &m_CoinsGottenPosition.x, 0, m_SceneContext.windowWidth);
	ImGui::ColorEdit4("Color", &m_TextColor.x, ImGuiColorEditFlags_NoInputs);

	if (PositionTemp)
	{

		auto curPos = PositionTemp->GetTransform()->GetWorldPosition();
		float pos[3]{ curPos.x, curPos.y, curPos.z };

		ImGui::DragFloat3("Translation", pos, 0.1f, -2000, 2000);
		PositionTemp->GetTransform()->Translate(pos[0], pos[1], pos[2]);

		ImGui::DragFloat4("Rotation", rot, 0.01f, -380, 380);
		PositionTemp->GetTransform()->Rotate(rot[0], rot[1], rot[2], rot[3]);
	}



}

void MarioScene::OnSceneActivated()
{
	m_pControlls->SetActive(true);
	m_ControllDisplayTimer = 0;
	m_pChannel2D->setPosition(0, FMOD_TIMEUNIT_MS);
	m_pChannel2D->setPaused(false);

	m_NrOfLives = 3;
	m_NrCoinsPickedUp = 0;


	m_NrTotalCoins = 0;
	for (auto& coin : m_Coins)
	{
		RemoveChild(coin, true);
	}
	m_Coins.clear();

	MakeCoins();
}

void MarioScene::OnSceneDeactivated()
{
	m_pChannel2D->setPaused(true);
}

void MarioScene::Update()
{

#ifdef _DEBUG
	//std::cout << "Mario at: " << m_pMario->GetTransform()->GetWorldPosition().x <<
	//	", " << m_pMario->GetTransform()->GetWorldPosition().y <<
	//	", " << m_pMario->GetTransform()->GetWorldPosition().z << '\n';
	std::cout << "Camera at: " << m_SceneContext.pCamera->GetTransform()->GetWorldPosition().x << "f" <<
		", " << m_SceneContext.pCamera->GetTransform()->GetWorldPosition().y << "f" <<
		", " << m_SceneContext.pCamera->GetTransform()->GetWorldPosition().z << "f" << '\n';
#endif // _DEBUG

	m_ControllDisplayTimer += m_SceneContext.pGameTime->GetElapsed();

	if (m_ControllDisplayTimer < 1)
		return;
	else
		m_pControlls->SetActive(false);


	if (m_IsDead)
	{
		Reset();
		m_NrOfLives--;
		if (m_NrOfLives == -1)
			SceneManager::Get()->SetActiveGameScene(L"DeathScene");

	}

	if (m_NrTotalCoins == m_NrCoinsPickedUp)
	{
		Reset();
		SceneManager::Get()->SetActiveGameScene(L"VictoryScene");
	}

	if (InputManager::IsMouseButton(InputState::pressed, VK_LBUTTON) && m_IsPaused)
	{
		auto mousePos = InputManager::GetMousePosition();
		for (auto button : m_Buttons)
		{
			button->OnClicked(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
		}
	}


	if (m_SceneContext.pInput->IsActionTriggered(PauseGame) && !m_IsPaused)
		Pause();
	else if (m_SceneContext.pInput->IsActionTriggered(PauseGame) && m_IsPaused)
		Resume();

	if (m_IsPaused)
	{
		if (m_SceneContext.pInput->IsActionTriggered(0))
		{
			m_Buttons[m_CurButton]->SetSelected(false);
			++m_CurButton;
			if (m_CurButton == m_Buttons.size())
				m_CurButton = 0;
			m_Buttons[m_CurButton]->SetSelected(true);
		}
		if (m_SceneContext.pInput->IsActionTriggered(1))
		{
			m_Buttons[m_CurButton]->SetSelected(false);
			if (static_cast<int>(m_CurButton - 1) < 0)
				m_CurButton = static_cast<unsigned>(m_Buttons.size() - 1);
			else
				--m_CurButton;

			m_Buttons[m_CurButton]->SetSelected(true);
		}
		if (m_SceneContext.pInput->IsActionTriggered(2))
		{
			m_Buttons[m_CurButton]->DoOnClick();
		}
	}

	auto pMarioPos = m_pMario->GetTransform()->GetWorldPosition();

	m_SceneContext.pLights->GetDirectionalLight().position = { pMarioPos.x + m_LightOffset[0] , pMarioPos.y + m_LightOffset[1], pMarioPos.z + m_LightOffset[2], 0 };

	TextRenderer::Get()->DrawText(m_pFont, StringUtil::utf8_decode("X" + std::to_string(m_NrCoinsPickedUp)), m_CoinsGottenPosition, m_TextColor);
	TextRenderer::Get()->DrawText(m_pFont, StringUtil::utf8_decode("X" + std::to_string(m_NrOfLives)), m_LivesLeftPosition, m_TextColor);
}

void MarioScene::PostDraw()
{
	//ShadowMapRenderer::Get()->Debug_DrawDepthSRV({ m_SceneContext.windowWidth - 10.f, 10.f }, { m_ShadowMapScale, m_ShadowMapScale }, { 1.f,0.f });
}

void MarioScene::MakeMario(physx::PxMaterial* pPhysicsMaterial)
{

	const auto pMarioMat = MaterialManager::Get()->CreateMaterial<EntityMaterial_Skinned>();

	pMarioMat->SetDiffuseTexture(L"Textures/Mario/Mario/MarioDiffuse.png");
	pMarioMat->SetOpacityTexture(L"Textures/Mario/Mario/MarioOpacity.png");

	CharacterDesc characterDesc{ pPhysicsMaterial };
	characterDesc.actionId_MoveForward = Inputs::CharacterMoveForward;
	characterDesc.actionId_MoveBackward = Inputs::CharacterMoveBackward;
	characterDesc.actionId_MoveLeft = Inputs::CharacterMoveLeft;
	characterDesc.actionId_MoveRight = Inputs::CharacterMoveRight;
	characterDesc.actionId_Jump = Inputs::CharacterJump;
	characterDesc.actionId_Punch = Inputs::CharacterPunch;
	characterDesc.actionId_Sprint = Inputs::CharacterSprint;

	m_pMario = AddChild(new ThirdPersonCharacter(characterDesc, pPhysicsMaterial));

	m_pMario->SetTag(L"Mario");

	const auto pModelObject = m_pMario->AddChild(new GameObject());

	const auto pModel = pModelObject->AddComponent(new ModelComponent(L"Meshes/Mario/MarioModel/Mario.ovm"));
	pModelObject->SetTag(L"Mario");

	m_pMario->SetModel(pModel);

	pModel->SetMaterial(pMarioMat);

	//pModel->GetTransform()->Scale({ CorrectScale.x, CorrectScale.y, CorrectScale.z });
	pModelObject->GetTransform()->Scale({ MarioScale.x, MarioScale.y,MarioScale.z });
	pModelObject->GetTransform()->Translate(0, -2, 0);


	//Animation Block
	m_pAnimator = pModel->GetAnimator();
	m_pMario->SetAnimator(m_pAnimator);

	m_pAnimator->SetAnimation(m_AnimationClipId);
	m_pAnimator->SetAnimationSpeed(m_AnimationSpeed);

	//Gather Clip Names

	m_pAnimator->SetAnimation(L"Idle");

	m_pAnimator->Play();

	//Animation Block
	m_pMario->GetTransform()->Translate(0, 5, 0);
}

void MarioScene::MakeBubbleEmitter(float x, float y, float z)
{
	ParticleEmitterSettings settings{};
	settings.velocity = { 0,0,0 };
	settings.minSize = 0.5f;
	settings.maxSize = 1.1f;
	settings.minEnergy = 1;
	settings.maxEnergy = 1;
	settings.minScale = 1;
	settings.maxScale = 1.5f;
	settings.minEmitterRadius = .2f;
	settings.maxEmitterRadius = .5f;
	settings.color = { 1.f, 1.f, 1.f, .6f };

	const auto pObject = AddChild(new GameObject);
	pObject->GetTransform()->Translate(x, y, z);
	pObject->AddComponent(new ParticleEmitterComponent(L"Textures/Mario/Bubble/Bubble.png", settings, 10));
}

void MarioScene::MakeCoin(float x, float y, float z, physx::PxMaterial* pPhysicsMaterial)
{
	auto pCoin = AddChild(new Coin(pPhysicsMaterial, &m_NrCoinsPickedUp, m_Coins));
	m_Coins.emplace_back(pCoin);
	pCoin->GetTransform()->Translate(x, y, z);
	++m_NrTotalCoins;
}

void MarioScene::MakeGoomba(float x, float y, float z, physx::PxMaterial* physicsMaterial)
{
	auto pGoomba = AddChild(new Goomba(physicsMaterial, m_pMario));
	m_Goombas.emplace_back(pGoomba);
	pGoomba->GetTransform()->Translate(x, y, z);
	pGoomba->SetInitPos({ x, y, z });
}

void MarioScene::MakeLevel()
{
	const auto pCastleRoot = AddChild(new GameObject());
	auto pRootTransform = pCastleRoot->GetTransform();

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
	pGrass1RB->AddCollider(PxTriangleMeshGeometry{ pGrass1Mesh, PxMeshScale{{ CorrectScale.x, CorrectScale.y, CorrectScale.z }} }, *m_pDefaultPhysxMat);
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
	pGrass2RB->AddCollider(PxTriangleMeshGeometry{ pGrass2Mesh, PxMeshScale{{ CorrectScale.x, CorrectScale.y, CorrectScale.z }} }, *m_pDefaultPhysxMat);

	pGrass2Model->SetMaterial(pGrass2Mat);


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
	pSandRB->AddCollider(PxTriangleMeshGeometry{ pSandMesh, PxMeshScale{{ CorrectScale.x, CorrectScale.y, CorrectScale.z }} }, *m_pDefaultPhysxMat);

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
	pBridgeWalkWayRB->AddCollider(PxTriangleMeshGeometry{ pBridgeWalkWayMesh, PxMeshScale{{ CorrectScale.x, CorrectScale.y, CorrectScale.z }} }, *m_pDefaultPhysxMat);
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
	pDetailsRB->AddCollider(PxTriangleMeshGeometry{ pDetailsMesh, PxMeshScale{{ CorrectScale.x, CorrectScale.y, CorrectScale.z }} }, *m_pDefaultPhysxMat);

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
	pFencesRB->AddCollider(PxTriangleMeshGeometry{ pFencesMesh, PxMeshScale{{ CorrectScale.x, CorrectScale.y, CorrectScale.z }} }, *m_pDefaultPhysxMat);

	pFencesModel->SetMaterial(pFencesMat);


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
	pGratesRB->AddCollider(PxTriangleMeshGeometry{ pGratesMesh, PxMeshScale{{ CorrectScale.x, CorrectScale.y, CorrectScale.z }} }, *m_pDefaultPhysxMat);

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
	pRocksRB->AddCollider(PxTriangleMeshGeometry{ pRocksMesh, PxMeshScale{{ CorrectScale.x, CorrectScale.y, CorrectScale.z }} }, *m_pSlipperyPhysxMat);

	pRocksModel->SetMaterial(pRocksMat);


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
	pRocksUnderWaterRB->AddCollider(PxTriangleMeshGeometry{ pRocksUnderWaterMesh, PxMeshScale{{ CorrectScale.x, CorrectScale.y, CorrectScale.z }} }, *m_pSlipperyPhysxMat);

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
	pUnderWaterSandRB->AddCollider(PxTriangleMeshGeometry{ pUnderWaterSandMesh, PxMeshScale{{ CorrectScale.x, CorrectScale.y, CorrectScale.z }} }, *m_pDefaultPhysxMat);

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
	pCobbleRB->AddCollider(PxTriangleMeshGeometry{ pCobbleMesh, PxMeshScale{{ CorrectScale.x, CorrectScale.y, CorrectScale.z }} }, *m_pDefaultPhysxMat);

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
	pBouldersRB->AddCollider(PxTriangleMeshGeometry{ pBouldersMesh, PxMeshScale{{ CorrectScale.x, CorrectScale.y, CorrectScale.z }} }, *m_pSlipperyPhysxMat);

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
	pWindowsRB->AddCollider(PxTriangleMeshGeometry{ pWindowsMesh, PxMeshScale{{ CorrectScale.x, CorrectScale.y, CorrectScale.z }} }, *m_pDefaultPhysxMat);

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
	pUnderWaterBricksRB->AddCollider(PxTriangleMeshGeometry{ pUnderWaterBricksMesh, PxMeshScale{{ CorrectScale.x, CorrectScale.y, CorrectScale.z }} }, *m_pDefaultPhysxMat);

	pUnderWaterBricksModel->SetMaterial(pUnderWaterBricksMat);



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
	pCastleRB->AddCollider(PxTriangleMeshGeometry{ pCastleMesh, PxMeshScale{{ CorrectScale.x, CorrectScale.y, CorrectScale.z }} }, *m_pDefaultPhysxMat);

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
	pRoofRB->AddCollider(PxTriangleMeshGeometry{ pRoofMesh, PxMeshScale{{ CorrectScale.x, CorrectScale.y, CorrectScale.z }} }, *m_pDefaultPhysxMat);

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
	pCarpetRB->AddCollider(PxTriangleMeshGeometry{ pCarpetMesh, PxMeshScale{{ CorrectScale.x, CorrectScale.y, CorrectScale.z }} }, *m_pDefaultPhysxMat);

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
	pBridgeTopRB->AddCollider(PxTriangleMeshGeometry{ pBridgeTopMesh, PxMeshScale{{ CorrectScale.x, CorrectScale.y, CorrectScale.z }} }, *m_pDefaultPhysxMat);

	pBridgeTopModel->SetMaterial(pBridgeTopMat);

	//BridgeSide
	auto pBridgeSideMat = MaterialManager::Get()->CreateMaterial<UberMaterial>();
	pBridgeSideMat->SetDiffuseTexture(L"Textures/Mario/Castle/BridgeSide.png");
	pBridgeSideMat->SetUseNormalMaps(false);
	pBridgeSideMat->SetUseSpecularTexture(false);

	const auto pBridgeSide = pCastleRoot->AddChild(new GameObject());
	const auto pBridgeSideModel = pBridgeSide->AddComponent(new ModelComponent(L"Meshes/Mario/CastleModels/BridgeSide.ovm"));
	pBridgeSide->GetTransform()->Scale({ CorrectScale.x, CorrectScale.y, CorrectScale.z });


	const auto pBridgeSideMesh = ContentManager::Load<PxTriangleMesh>(L"Meshes/Mario/CastleModels/BridgeSide.ovpt");

	auto pBridgeSideRB = pBridgeSide->AddComponent(new RigidBodyComponent(true));
	pBridgeSideRB->AddCollider(PxTriangleMeshGeometry{ pBridgeSideMesh, PxMeshScale{{ CorrectScale.x, CorrectScale.y, CorrectScale.z }} }, *m_pDefaultPhysxMat);

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
	pPeachTopRB->AddCollider(PxTriangleMeshGeometry{ pPeachTopMesh, PxMeshScale{{ CorrectScale.x, CorrectScale.y, CorrectScale.z }} }, *m_pDefaultPhysxMat);

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
	pPeachBottomRB->AddCollider(PxTriangleMeshGeometry{ pPeachBottomMesh, PxMeshScale{{ CorrectScale.x, CorrectScale.y, CorrectScale.z }} }, *m_pDefaultPhysxMat);

	pPeachBottomModel->SetMaterial(pPeachBottomMat);



	//Water
	auto pWaterMat = MaterialManager::Get()->CreateMaterial<FlowingWater>();
	pWaterMat->SetDiffuseTexture(L"Textures/Mario/Castle/Water.png");
	pWaterMat->SetUseNormalMaps(false);
	pWaterMat->SetUseSpecularTexture(false);
	pWaterMat->SetOpacityIntensity(0.75f);
	pWaterMat->SetFlowDir({ 0.5,0.5 });
	pWaterMat->SetPerlinValues(L"Textures/Mario/Water/perlin.png", 0.010f, 0.6f);

	const auto pWater = pCastleRoot->AddChild(new GameObject());

	const auto pWaterModel = pWater->AddComponent(new ModelComponent(L"Meshes/Mario/CastleModels/Water.ovm", false));
	pWater->GetTransform()->Scale({ CorrectScale.x, CorrectScale.y, CorrectScale.z });





	//const auto pWaterMesh = ContentManager::Load<PxTriangleMesh>(L"Meshes/Mario/CastleModels/Water.ovpt");
	//
	//auto pWaterRB = pWater->AddComponent(new RigidBodyComponent(true));
	//pWaterRB->AddCollider(PxTriangleMeshGeometry{ pWaterMesh, PxMeshScale{{ CorrectScale.x, CorrectScale.y, CorrectScale.z }} }, *pPhysicsMaterial);

	pWaterModel->SetMaterial(pWaterMat);
	pWaterModel->DrawPost(false);

	m_pDebugMat = pWaterMat;

	//Waterfall
	auto pWaterfallMat = MaterialManager::Get()->CreateMaterial<FlowingWater>();
	pWaterfallMat->SetDiffuseTexture(L"Textures/Mario/Castle/Water.png");
	pWaterfallMat->SetUseNormalMaps(false);
	pWaterfallMat->SetUseSpecularTexture(false);
	pWaterfallMat->SetOpacityIntensity(0.75f);
	pWaterfallMat->SetFlowDir({ 2.5,0 });

	const auto pWaterfall = pCastleRoot->AddChild(new GameObject());
	const auto pWaterfallModel = pWaterfall->AddComponent(new ModelComponent(L"Meshes/Mario/CastleModels/Waterfall.ovm", false));
	pWaterfall->GetTransform()->Scale({ CorrectScale.x, CorrectScale.y, CorrectScale.z });

	//const auto pWaterfallMesh = ContentManager::Load<PxTriangleMesh>(L"Meshes/Mario/CastleModels/Waterfall.ovpt");

	//auto pWaterfallRB = pWaterfall->AddComponent(new RigidBodyComponent(true));
	//pWaterfallRB->AddCollider(PxTriangleMeshGeometry{ pWaterfallMesh, PxMeshScale{{ CorrectScale.x, CorrectScale.y, CorrectScale.z }} }, *pPhysicsMaterial);

	pWaterfallModel->SetMaterial(pWaterfallMat);
	pWaterfallModel->DrawPost(false);


	pRootTransform->Translate(0, 0.25f, 0);
}

void MarioScene::MakeUnderwater(physx::PxMaterial* physicsMaterial)
{
	auto pUnderwater = AddChild(new GameObject());
	pUnderwater->SetTag(L"UnderWater");

	PxVec3 scale{ 1000, 400, 1000 };
	pUnderwater->GetTransform()->Scale(scale.x, scale.y, scale.z);

	pUnderwater->GetTransform()->Rotate(4.532f, 0.001f, 1.611f, false);
	pUnderwater->GetTransform()->Translate(4.9f, -511.9f, 12.4f);

	auto pUnderwaterRB = pUnderwater->AddComponent(new RigidBodyComponent());
	pUnderwaterRB->SetKinematic(true);

	pUnderwaterRB->AddCollider(PxBoxGeometry{ scale / 2.0f }, *physicsMaterial, true);

	m_pPostUnderwater = MaterialManager::Get()->CreateMaterial<PostUnderwater>();

	pUnderwater->SetOnTriggerCallBack([&](GameObject*, GameObject* pOther, PxTriggerAction action) {


		if (action == PxTriggerAction::ENTER && pOther->GetTag() == L"Camera")
		{
			AddPostProcessingEffect(m_pPostUnderwater);
		}


		if (action == PxTriggerAction::LEAVE && pOther->GetTag() == L"Camera")
		{
			RemovePostProcessingEffect(m_pPostUnderwater);
		}

		});
}

void MarioScene::MakeUI()
{
	auto pCoin = AddChild(new GameObject);
	pCoin->AddComponent(new SpriteComponent(L"Textures/Mario/GameUI/CoinHUD.png"));
	pCoin->GetTransform()->Translate(m_SceneContext.windowWidth - 100, 15, 0);
	m_CoinsGottenPosition = { m_SceneContext.windowWidth - 70, 12.5f };


	auto pHead = AddChild(new GameObject);
	pHead->AddComponent(new SpriteComponent(L"Textures/Mario/GameUI/MarioHeadHUD.png"));
	pHead->GetTransform()->Translate(m_SceneContext.windowWidth - 500, 15, 0);
	m_LivesLeftPosition = { m_SceneContext.windowWidth - 450, 12.5f };

	MakeHearts();

}

void MarioScene::MakeHearts()
{
	static const PxVec2 healthStartPos{ m_SceneContext.windowWidth - 300,12.5 };
	constexpr float widthOffset{ 35 };
	m_pHeartsHud = AddChild(new GameObject);
	for (int i = 0; i < 3; ++i)
	{
		auto pHeart = m_pHeartsHud->AddChild(new GameObject);
		m_pHearts.emplace_back(pHeart);
		pHeart->AddComponent(new SpriteComponent(L"Textures/Mario/Mario/Heart.png"));
		pHeart->GetTransform()->Translate(healthStartPos.x + widthOffset * i, healthStartPos.y, 0.35f);
	}
}

void MarioScene::MakePauseMenu()
{
	m_pPostBlur = MaterialManager::Get()->CreateMaterial<PostBlur>();


	auto pMainMenu = AddChild(new Button(L"Textures/Mario/Menu/ToMainMenu.png", L"Textures/Mario/Menu/ToMainMenuActive.png", [&]() {
		Reset();
		SceneManager::Get()->SetActiveGameScene(L"MainMenuScene");
		}, "Resources/Sounds/Mario/ToMainMenu.wav"));
	pMainMenu->GetTransform()->Translate(290, 550, 0.5f);
	m_Buttons.emplace_back(pMainMenu);

	m_PauseMenuSprites.emplace_back(pMainMenu->GetComponent<SpriteComponent>());
	m_PauseMenuSprites.back()->SetActive(false);


	auto pQuit = AddChild(new Button(L"Textures/Mario/Menu/Quit.png", L"Textures/Mario/Menu/QuitActive.png", [&]() { OverlordGame::Quit(); }, "Resources/Sounds/Mario/Exit.wav"));
	pQuit->GetTransform()->Translate(820, 550, 0.5f);
	m_Buttons.emplace_back(pQuit);

	m_PauseMenuSprites.emplace_back(pQuit->GetComponent<SpriteComponent>());
	m_PauseMenuSprites.back()->SetActive(false);

	auto pRestart = AddChild(new Button(L"Textures/Mario/Menu/Restart.png", L"Textures/Mario/Menu/RestartActive.png", [&]() { Reset(); }, "Resources/Sounds/Mario/Start-Restart.wav"));
	pRestart->GetTransform()->Translate(550, 230, 0.5f);
	m_Buttons.emplace_back(pRestart);

	m_PauseMenuSprites.emplace_back(pRestart->GetComponent<SpriteComponent>());
	m_PauseMenuSprites.back()->SetActive(false);

}

void MarioScene::Pause()
{
	AddPostProcessingEffect(m_pPostBlur);
	m_CurButton = 0;
	for (auto& button : m_Buttons)
	{
		button->Activate();
	}
	m_Buttons[m_CurButton]->SetSelected(true);
	m_SceneContext.pGameTime->Stop();
	m_IsPaused = true;
}

void MarioScene::Resume()
{
	RemovePostProcessingEffect(m_pPostBlur);
	for (auto& button : m_Buttons)
	{
		button->Deactivate();
	}
	m_IsPaused = false;

	m_SceneContext.pGameTime->Start();
}

void MarioScene::MakeSkybox()
{
	const auto pSkyBoxMat = MaterialManager::Get()->CreateMaterial<SkyboxMaterial>();
	pSkyBoxMat->SetDiffuseTexture(L"Textures/Mario/Skybox/Skybox.dds");

	const auto pSkyboxGO = AddChild(new GameObject);
	const auto pSkyboxModel = pSkyboxGO->AddComponent(new ModelComponent(L"Meshes/Box.ovm"));
	pSkyboxModel->SetMaterial(pSkyBoxMat);
}

void MarioScene::Reset()
{
	m_pMario->ResetCamera();
	m_pMario->GetTransform()->Translate(0, 5, 0);


	for (auto& goomba : m_Goombas)
	{
		goomba->Reset();
	}


	for (auto& heart : m_pHearts)
	{
		m_pHeartsHud->RemoveChild(heart, true);
	}
	m_pHearts.clear();

	MakeHearts();
	m_IsDead = false;

	Resume();

}

void MarioScene::MakeCoins()
{
	MakeCoin(0.247314f, 2.855949f, -7.74701f, m_pDefaultPhysxMat);
	MakeCoin(149.275f, -6.60035f, -85.2508f, m_pDefaultPhysxMat);
	MakeCoin(-122.595f, -39.6861f, -66.3375f, m_pDefaultPhysxMat);
	MakeCoin(47.5955f, -9.1152f, -139.091f, m_pDefaultPhysxMat);
	MakeCoin(7.43575f, -35.258f, -258.486f, m_pDefaultPhysxMat);
}

void MarioScene::MakeGoombas()
{
	MakeGoomba(151.087f, -3.44418f, -79.8373f, m_pDefaultPhysxMat);
	MakeGoomba(138.239f, -5.43421f, -85.2993f, m_pDefaultPhysxMat);
	MakeGoomba(44.2151f, -6.4628f, -135.365f, m_pDefaultPhysxMat);
	MakeGoomba(-76.8296f, -21.4486f, -192.854f, m_pDefaultPhysxMat);
	MakeGoomba(-46.2544f, -24.3595f, -237.877f, m_pDefaultPhysxMat);
	MakeGoomba(-4.4778f, -30.2403f, -257.665f, m_pDefaultPhysxMat);
}

void MarioScene::MakeBubbles()
{
	//Waterfall
	MakeBubbleEmitter(167.285f, -29.8695f, -118.827f);
	MakeBubbleEmitter(166.264f, -32.2084f, -130.228f);
	MakeBubbleEmitter(167.547f, -34.3052f, -140.876f);
	MakeBubbleEmitter(175.725f, -29.4026f, -108.575f);
	MakeBubbleEmitter(166.628f, -33.4221f, -132.119f);
	MakeBubbleEmitter(167.604f, -30.6923f, -113.641f);
	MakeBubbleEmitter(167.223f, -33.0672f, -125.368f);
	MakeBubbleEmitter(165.928f, -33.259f, -127.836f);
	MakeBubbleEmitter(166.367f, -35.5401f, -138.801f);
	MakeBubbleEmitter(166.517f, -35.8113f, -144.458f);
	MakeBubbleEmitter(166.843f, -35.004f, -141.548f);
	MakeBubbleEmitter(168.335f, -33.9655f, -125.831f);
	MakeBubbleEmitter(168.891f, -33.2353f, -118.82f);


	//UnderWater
	MakeBubbleEmitter(-115.053f, -39.1606f, -57.6249f);
	MakeBubbleEmitter(-123.415f, -39.7363f, -60.9125f);
	MakeBubbleEmitter(16.579f, -36.8913f, -139.643f);
	MakeBubbleEmitter(24.3567f, -28.9532f, -94.3045f);
}
