#include "stdafx.h"
#include "Goomba.h"

#include "Materials/UberMaterial.h"
#include "ThirdPersonCharacter.h"

Goomba::Goomba(physx::PxMaterial* physicsMaterial, ThirdPersonCharacter* pMario, std::vector<Goomba*>& owningCollection)
	: m_pPhysxMat{ physicsMaterial }
	, m_pMario{ pMario }
	, m_OwningVec{ owningCollection }
{
}

void Goomba::SetDeath()
{
	m_HasDied = true;
}

void Goomba::Initialize(const SceneContext&)
{
	const PxVec3 goombaScale{ 20, 20, 20 };
	SetTag(L"Goomba");


	//Visuals
	const auto pGoombaMat = MaterialManager::Get()->CreateMaterial<UberMaterial>();

	pGoombaMat->SetUseOpacity(true);
	pGoombaMat->SetDiffuseTexture(L"Textures/Mario/Goomba/GoombaDiffuse.png");
	pGoombaMat->SetOpacityTexture(L"Textures/Mario/Goomba/GoombaOpacity.png");
	pGoombaMat->SetUseSpecularTexture(false);


	const auto pModelGO = AddChild(new GameObject());
	pModelGO->SetTag(L"GoombaBody");
	const auto pModelComponent = pModelGO->AddComponent(new ModelComponent(L"Meshes/Mario/Goomba/Goomba.ovm"));
	pModelComponent->SetMaterial(pGoombaMat);

	pModelGO->GetTransform()->Translate(0, -1.3f, 0);
	pModelGO->GetTransform()->Scale(goombaScale.x, goombaScale.y, goombaScale.z);


	//Controller
	PxCapsuleControllerDesc controller{};
	controller.setToDefault();
	controller.radius = 1.5f;
	controller.height = 0.7f;
	controller.material = m_pPhysxMat;

	m_pController = AddComponent(new ControllerComponent({ controller }));

	//PlayerDamage

	m_pHitBox = AddChild(new GameObject);
	auto pHitBoxRB = m_pHitBox->AddComponent(new RigidBodyComponent());
	pHitBoxRB->SetKinematic(true);

	pHitBoxRB->AddCollider(PxCapsuleGeometry{ 1.6f, 0.15f }, *m_pPhysxMat, true);


	m_pHitBox->SetOnTriggerCallBack([&](GameObject*, GameObject* pOther, PxTriggerAction action) {

		if (!m_CanBeHit)
			return;
		if (action == PxTriggerAction::ENTER && pOther->GetTag() == L"Mario")
		{
			std::cout << "enter\n";
		}
		});




}

void Goomba::Update(const SceneContext& sceneContext)
{

	if (!sceneContext.pGameTime->IsRunning())
		return;

	constexpr float minWaitTime{ 0.3f };

	if (sceneContext.pGameTime->GetTotal() < minWaitTime)
		return;

	m_CanBeHit = true;
	if (m_pController->GetCollisionFlags() & PxControllerCollisionFlag::eCOLLISION_UP)
	{
		//TODO: figure out why jump sometimes fails
		m_pMario->Jump();
		m_OwningVec.erase(std::remove(begin(m_OwningVec), end(m_OwningVec), this));
		SceneManager::Get()->GetActiveScene()->RemoveChild(this, true);
		return;
	}
	else if (m_HasDied)
	{
		SceneManager::Get()->GetActiveScene()->RemoveChild(this, true);
		return;
	}


	constexpr float maxYRot = 10;
	constexpr float minYRot = -maxYRot;
	const float randomYRot = MathHelper::randF(minYRot, maxYRot);

	m_YRot += randomYRot;

	GetTransform()->Rotate(0, m_YRot, 0, true);


	auto forward = GetTransform()->GetForward();

	const float elapsed = sceneContext.pGameTime->GetElapsed();
	const float moveDistance = elapsed * m_MoveSpeed;
	constexpr float fallSpeed = 10;
	constexpr float maxFallSpeed = -30;

	forward = { forward.x * moveDistance,0, forward.z * moveDistance };

	if (!(m_pController->GetCollisionFlags() & PxControllerCollisionFlag::eCOLLISION_DOWN))
	{
		forward.y = std::max(forward.y - fallSpeed * elapsed, maxFallSpeed);
	}
	else
	{
		forward.y = 0;

	}

	m_pController->Move(forward);

	m_pHitBox->GetTransform()->Translate(m_pController->GetPosition());




}
