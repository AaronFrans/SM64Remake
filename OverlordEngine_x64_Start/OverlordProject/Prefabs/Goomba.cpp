#include "stdafx.h"
#include "Goomba.h"

#include "Materials/UberMaterial.h"
#include "ThirdPersonCharacter.h"
#include "ParticleEmmiter.h"

#include "Scenes/Exam/MarioScene.h"

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


	m_pMesh = AddChild(new GameObject());
	m_pMesh->SetTag(L"GoombaBody");
	const auto pModelComponent = m_pMesh->AddComponent(new ModelComponent(L"Meshes/Mario/Goomba/Goomba.ovm"));
	pModelComponent->SetMaterial(pGoombaMat);

	m_pMesh->GetTransform()->Translate(0, -1.3f, 0);
	m_pMesh->GetTransform()->Scale(goombaScale.x, goombaScale.y, goombaScale.z);


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

	pHitBoxRB->AddCollider(PxCapsuleGeometry{ 1.9f, 0.1f }, *m_pPhysxMat, true);

	if (!m_OwningScene)
	{
		m_OwningScene = static_cast<MarioScene*>(GetScene());
	}

	m_pHitBox->SetOnTriggerCallBack([&](GameObject*, GameObject* pOther, PxTriggerAction action) {

		if (!m_CanBeHit || m_HasDied)
			return;

		if (action == PxTriggerAction::ENTER && pOther->GetTag() == L"Mario")
		{
			m_OwningScene->RemoveHealth();
		}
		});




}

void Goomba::Update(const SceneContext& sceneContext)
{
	if (!sceneContext.pGameTime->IsRunning())
		return;

	

	if (m_pEmmiter)
	{
		if (m_pEmmiter->IsDone())
		{
			m_OwningVec.erase(std::remove(begin(m_OwningVec), end(m_OwningVec), this));
			m_OwningScene->RemoveChild(this, true);
		}
		return;
	}

	constexpr float minWaitTime{ 0.3f };

	m_TimeSinceSpawm += sceneContext.pGameTime->GetElapsed();
	if (m_TimeSinceSpawm < minWaitTime)
		return;


	m_CanBeHit = true;

	if (m_pController->GetCollisionFlags() & PxControllerCollisionFlag::eCOLLISION_UP)
	{
		MakeParticleEmmiter();

		//TODO: figure out why jump sometimes fails
		m_pMario->Jump();
		m_HasDied = true;
		return;
	}
	else if (m_HasDied)
	{

		MakeParticleEmmiter();
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

void Goomba::MakeParticleEmmiter()
{
	ParticleEmitterSettings settings{};
	settings.velocity = { 0,0,0 };
	settings.minSize = 0.5f;
	settings.maxSize = 1.1f;
	settings.minEnergy = 1;
	settings.maxEnergy = 1.5;
	settings.minScale = 3;
	settings.maxScale = 4.5f;
	settings.minEmitterRadius = 1;
	settings.maxEmitterRadius = 1.5f;
	settings.color = { 1.f, 1.f, 1.f, .6f };

	m_pEmmiter = AddChild(new ParticleEmmiter(L"Textures/Mario/Goomba/Smoke.png", settings, 50));

	RemoveChild(m_pMesh, true);
	RemoveComponent(m_pController, true);


}
