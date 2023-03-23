#include "stdafx.h"
#include "BoneObject.h"

BoneObject::BoneObject(BaseMaterial* pMaterial, float length)
	: GameObject()
	, m_pMaterial{ pMaterial }
	, m_Length{ length }
{}

void BoneObject::AddBone(BoneObject* pBone)
{
	pBone->GetTransform()->Translate(m_Length, 0, 0);
	AddChild(pBone);
}

void BoneObject::CalculateBindPose()
{
	XMFLOAT4X4 inverse4x4;
	XMStoreFloat4x4(&inverse4x4, XMMatrixInverse(nullptr, XMLoadFloat4x4(&GetTransform()->GetWorld())));

	m_BindPose = inverse4x4;


	for (BoneObject* pChild : GetChildren<BoneObject>())
	{
		pChild->CalculateBindPose();
	}
}

void BoneObject::Initialize(const SceneContext&)
{
	GameObject* pEmpty = new GameObject();
	AddChild(pEmpty);

	auto pModel = pEmpty->AddComponent(new ModelComponent(L"Meshes/Bone.ovm"));
	pModel->SetMaterial(m_pMaterial);

	const auto& transform = pEmpty->GetTransform();

	transform->Rotate(0, -90, 0);

	const auto& currentScale = transform->GetScale();

	transform->Scale(currentScale.x * m_Length, currentScale.y * m_Length, currentScale.z * m_Length);


}
