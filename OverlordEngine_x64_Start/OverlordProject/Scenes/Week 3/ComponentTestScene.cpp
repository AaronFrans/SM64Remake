
#include "stdafx.h"
#include "ComponentTestScene.h"

#include "Prefabs/CubePrefab.h"
#include "Prefabs/SpherePrefab.h"
#include "Prefabs/TorusPrefab.h"


ComponentTestScene::ComponentTestScene() :
	GameScene(L"Component Test Scene") {}

void ComponentTestScene::Initialize()
{
	auto& physX = PxGetPhysics();
	auto pBouncyMat = physX.createMaterial(.5f, .5f, 0.9999f);


	//ground plane
	GameSceneExt::CreatePhysXGroundPlane(*this, pBouncyMat);

	//Sphere red (Group 0)

	auto pSphere = new SpherePrefab(1, 10, XMFLOAT4{ Colors::Red });
	AddChild(pSphere);
	pSphere->GetTransform()->Translate(0, 30, 0);

	auto pRigidBody = pSphere->AddComponent(new RigidBodyComponent());

	pRigidBody->AddCollider(PxSphereGeometry{ 1 }, *pBouncyMat);

	pRigidBody->SetCollisionGroup(CollisionGroup::Group0);
	pRigidBody->SetCollisionIgnoreGroups(CollisionGroup::Group1 | CollisionGroup::Group2);
	//Sphere green (Group 1)

	pSphere = new SpherePrefab(1, 10, XMFLOAT4{ Colors::Green });
	AddChild(pSphere);
	pSphere->GetTransform()->Translate(0.5f, 60, 0);

	pRigidBody = pSphere->AddComponent(new RigidBodyComponent());

	pRigidBody->AddCollider(PxSphereGeometry{ 1 }, *pBouncyMat);

	pRigidBody->SetCollisionGroup(CollisionGroup::Group1);
	pRigidBody->SetConstraint(RigidBodyConstraint::TransX | RigidBodyConstraint::TransZ, false);

	//Sphere blue (Group 2)

	pSphere = new SpherePrefab(1, 10, XMFLOAT4{ Colors::Blue });
	AddChild(pSphere);
	pSphere->GetTransform()->Translate(0, 90, 0);

	pRigidBody = pSphere->AddComponent(new RigidBodyComponent());

	pRigidBody->AddCollider(PxSphereGeometry{ 1 }, *pBouncyMat);

	pRigidBody->SetCollisionGroup(CollisionGroup::Group2);

}