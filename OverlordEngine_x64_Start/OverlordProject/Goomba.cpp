#include "stdafx.h"
#include "Goomba.h"

#include "Materials/Mario/EntityMaterial_Skinned.h"

Goomba::Goomba(physx::PxMaterial* physicsMaterial)
{
}

void Goomba::Initialize(const SceneContext& scene)
{
	const PxVec3 goombaScale{ 20, 20, 20 };
	auto pGoombaMat = MaterialManager::Get()->CreateMaterial<EntityMaterial_Skinned>();

	pGoombaMat->SetDiffuseTexture(L"Textures/Mario/Mario/MarioDiffuse.png");
	pGoombaMat->SetOpacityTexture(L"Textures/Mario/Mario/MarioOpacity.png");


}

void Goomba::Update(const SceneContext&)
{

}
