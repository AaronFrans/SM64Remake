#include "stdafx.h"
#include "Coin.h"

#include "Materials/ColorMaterial.h"

Coin::Coin(physx::PxMaterial* physicsMaterial, GameObject* mario)
	: m_pPhysicsMaterial{ physicsMaterial }
	, m_pMario{ mario }

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

void Coin::PostInitialize(const SceneContext&)
{
	auto pCoinCollider = m_pRB->GetCollider(m_ColliderId);
	pCoinCollider.SetTrigger(true);


	SetOnTriggerCallBack([&](GameObject*, GameObject* pOther, PxTriggerAction action) {


		if (action != PxTriggerAction::ENTER && pOther != m_pMario)
			return;

		std::cout << "hit";

		m_WasHit = true;

		});
}
