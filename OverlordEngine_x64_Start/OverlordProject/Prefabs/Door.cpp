#include "stdafx.h"
#include "Door.h"
#include "Materials/EntityMaterial.h"

Door::Door(physx::PxMaterial* pPhysicsMaterial)
	:m_pPhysicsMaterial{ pPhysicsMaterial }
{

}
void Door::Initialize(const SceneContext&)
{

	const auto pDoorMat = MaterialManager::Get()->CreateMaterial<EntityMaterial>();

	pDoorMat->SetDiffuseTexture(L"Textures/Mario/Door/DoorDiffuse.png");
	pDoorMat->SetOpacityTexture(L"Textures/Mario/Door/DoorOpacity.png");



	const auto pModel = AddComponent(new ModelComponent(L"Meshes/Mario/Door/Door.ovm"));
	GetTransform()->Scale({CorrectScale.x, CorrectScale.y, CorrectScale.z});
	pModel->SetMaterial(pDoorMat);

	const auto pMesh = ContentManager::Load<PxTriangleMesh>(L"Meshes/Mario/Door/Door.ovpt");

	auto pRB = AddComponent(new RigidBodyComponent());
	pRB->SetKinematic(true);
	pRB->AddCollider(PxTriangleMeshGeometry{ pMesh, PxMeshScale{{ CorrectScale.x, CorrectScale.y, CorrectScale.z }} }, *m_pPhysicsMaterial);

}
