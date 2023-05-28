#include "stdafx.h"
#include "Goomba.h"

#include "Materials/Mario/EntityMaterial.h"

Goomba::Goomba(physx::PxMaterial* physicsMaterial)
	:m_pPhysxMat{ physicsMaterial }
{
}

void Goomba::Initialize(const SceneContext&)
{
	const PxVec3 goombaScale{ 20, 20, 20 };
	auto pGoombaMat = MaterialManager::Get()->CreateMaterial<EntityMaterial>();
	GetTransform()->Scale(goombaScale.x, goombaScale.y, goombaScale.z);

	pGoombaMat->SetDiffuseTexture(L"Textures/Mario/Goomba/GoombaDiffuse.png");
	pGoombaMat->SetOpacityTexture(L"Textures/Mario/Goomba/GoombaOpacity.png");

	const auto pModel = AddComponent(new ModelComponent(L"Meshes/Mario/Goomba/Goomba.ovm"));
	pModel->SetMaterial(pGoombaMat);

	SetTag(L"Goomba");


	//const auto pGoombaMesh = ContentManager::Load<PxConvexMesh>(L"Meshes/Mario/Goomba/Goomba.ovpc");

	//auto pRB = AddComponent(new RigidBodyComponent());
	//pRB->AddCollider(PxConvexMeshGeometry{ pGoombaMesh, PxMeshScale{{ goombaScale.x, goombaScale.y, goombaScale.z }} }, *m_pPhysxMat);
	//pRB->SetConstraint((RigidBodyConstraint::RotX | RigidBodyConstraint::RotZ), false);
	pModel->SetMaterial(pGoombaMat);

	PxCapsuleControllerDesc controller{};
	controller.setToDefault();
	controller.radius = 0.5f;
	controller.height = 0.5f;
	controller.material = m_pPhysxMat;

	m_pController = AddComponent(new ControllerComponent({ controller }));

}

void Goomba::Update(const SceneContext& sceneContext)
{
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
		//Decrease the y component of m_TotalVelocity with a fraction (ElapsedTime) of the Fall Acceleration (m_FallAcceleration)
		//Make sure that the minimum speed stays above -CharacterDesc::maxFallSpeed (negative!)
		forward.y = std::max(forward.y - fallSpeed * elapsed, maxFallSpeed);
	}
	else
	{

		//m_TotalVelocity.y is zero
		forward.y = 0;

	}

	//auto newPos = GetTransform()->GetWorldPosition();
	//
	//newPos = { newPos.x + forward.x, newPos.y + forward.y , newPos.z + forward.z };
	//
	//GetTransform()->Translate(newPos.x, newPos.y, newPos.z);

	m_pController->Move(forward);




}
