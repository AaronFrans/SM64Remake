#include "stdafx.h"
#include "Coin.h"

#include "Materials/ColorMaterial.h"
#include "Prefabs/ParticleEmmiter.h"

Coin::Coin(physx::PxMaterial* physicsMaterial, unsigned* coinCounter)
	: m_pPhysicsMaterial{ physicsMaterial }
	, m_pCoinCounter{ coinCounter }

{
}

void Coin::Initialize(const SceneContext&)
{
	const PxVec3 coinScale{ 0.015f, 0.015f, 0.015f };
	auto pCoinMat = MaterialManager::Get()->CreateMaterial<ColorMaterial>();
	pCoinMat->SetColor({ 189 / 255.0f, 183 / 255.0f, 107 / 255.0f, 1 });


	const auto pCoinModel = AddComponent(new ModelComponent(L"Meshes/Mario/Coin/Coin.ovm"));
	GetTransform()->Scale({ coinScale.x, coinScale.y, coinScale.z });


	const auto pCoinMesh = ContentManager::Load<PxConvexMesh>(L"Meshes/Mario/Coin/Coin.ovpc");

	m_pRB = AddComponent(new RigidBodyComponent());
	m_pRB->SetKinematic(true);

	m_ColliderId = m_pRB->AddCollider(
		PxConvexMeshGeometry{ pCoinMesh, PxMeshScale{{ coinScale.x, coinScale.y, coinScale.z }} },
		*m_pPhysicsMaterial, true);

	pCoinModel->SetMaterial(pCoinMat);


}

void Coin::Update(const SceneContext& sceneContext)
{
	constexpr int minWaitTime{ 1 };

	if (sceneContext.pGameTime->GetTotal() < minWaitTime)
		return;

	constexpr float coinsRotationSpeed{ 120 };
	m_CoinRotation += coinsRotationSpeed * sceneContext.pGameTime->GetElapsed();

	m_CanBeHit = true;

	GetTransform()->Rotate(0, m_CoinRotation, 0);

	if (m_WasHit && !m_pEmmiter)
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

		m_pEmmiter = AddChild(new ParticleEmmiter(L"Textures/Mario/Coins/Sparkle.png", settings, 50));

		m_pEmmiter->GetTransform()->Translate(GetTransform()->GetWorldPosition());

		auto pMesh = GetComponent<ModelComponent>();

		RemoveComponent(pMesh, true);

		return;
	}

	if (m_pEmmiter && m_pEmmiter->IsDone())
	{
		SceneManager::Get()->GetActiveScene()->RemoveChild(this, true);
	}



}

void Coin::PostInitialize(const SceneContext&)
{
	auto pCoinCollider = m_pRB->GetCollider(m_ColliderId);
	pCoinCollider.SetTrigger(true);


	SetOnTriggerCallBack([&](GameObject*, GameObject* pOther, PxTriggerAction action) {

		if (!m_CanBeHit || m_WasHit)
			return;

		if (action != PxTriggerAction::ENTER && pOther->GetTag() == L"Mario")
			return;

		std::cout << "hit";

		m_WasHit = true;

		*m_pCoinCounter += 1;

		});
}
