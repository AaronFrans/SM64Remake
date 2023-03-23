#include "stdafx.h"
#include "SoftwareSkinningScene_1.h"
#include "Prefabs/BoneObject.h"
#include "Materials/ColorMaterial.h"

SoftwareSkinningScene_1::SoftwareSkinningScene_1()
	:GameScene(L"SoftwareSkinningScene_1")
{
}

void SoftwareSkinningScene_1::Initialize()
{
	const auto pMat = MaterialManager::Get()->CreateMaterial<ColorMaterial>();

	pMat->SetColor({ 0.5f, 0.5f,0.5f, 1 });


	const auto pRoot = new GameObject();

	m_pBone0 = new BoneObject(pMat, 15.f);
	pRoot->AddChild(m_pBone0);

	m_pBone1 = new BoneObject(pMat, 15.f);

	m_pBone0->AddBone(m_pBone1);

	AddChild(pRoot);

	m_SceneContext.settings.enableOnGUI = true;


}

void SoftwareSkinningScene_1::Update()
{
	if (m_AutoRotate)
	{
		if (m_BoneRotation >= 45 || m_BoneRotation <= -45)
		{
			m_RotationSign *= -1;
		}

		const float rotSpeed = 45 * m_SceneContext.pGameTime->GetElapsed();

		m_BoneRotation += rotSpeed * m_RotationSign;

		m_Bone0Rotation = XMFLOAT3{ 0, 0, m_BoneRotation };
		m_Bone1Rotation = XMFLOAT3{ 0, 0,  -m_BoneRotation * 2 };
	}

	m_pBone0->GetTransform()->Rotate(m_Bone0Rotation);
	m_pBone1->GetTransform()->Rotate(m_Bone1Rotation);

}

void SoftwareSkinningScene_1::OnGUI()
{

	ImGui::DragFloat3("Bone 0 rotation", reinterpret_cast<float*>(&m_Bone0Rotation), 0.5f, -90, 90);
	ImGui::DragFloat3("Bone 1 rotation", reinterpret_cast<float*>(&m_Bone1Rotation), 0.5f, -90, 90);

	ImGui::Checkbox("AutoRotate", &m_AutoRotate);
}
