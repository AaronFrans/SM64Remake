#include "stdafx.h"
#include "Goomba.h"

#include "Materials/Mario/EntityMaterial.h"

Goomba::Goomba(physx::PxMaterial*)
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

	SetTag(L"Goomba");


	pModel->SetMaterial(pGoombaMat);

}

void Goomba::Update(const SceneContext&)
{

}
