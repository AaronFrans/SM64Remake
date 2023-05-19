#include "stdafx.h"
#include "SpriteComponent.h"

SpriteComponent::SpriteComponent(const std::wstring& spriteAsset, const XMFLOAT2& pivot, const XMFLOAT4& color) :
	m_SpriteAsset(spriteAsset),
	m_Pivot(pivot),
	m_Color(color)
{}

void SpriteComponent::Initialize(const SceneContext& /*sceneContext*/)
{
	m_pTexture = ContentManager::Load<TextureData>(m_SpriteAsset);
}

void SpriteComponent::SetTexture(const std::wstring& spriteAsset)
{
	m_SpriteAsset = spriteAsset;
	m_pTexture = ContentManager::Load<TextureData>(m_SpriteAsset);
}

XMFLOAT2 SpriteComponent::GetDimensions()
{
	return m_pTexture->GetDimension(); ;
}

void SpriteComponent::Draw(const SceneContext& sceneContext)
{
	if (!m_pTexture)
		return;

	TransformComponent* pObjectTrans = GetGameObject()->GetTransform();
	const XMFLOAT3& pos = pObjectTrans->GetWorldPosition();
	const XMFLOAT3& scale = pObjectTrans->GetWorldScale();

	const float& rot = MathHelper::QuaternionToEuler(pObjectTrans->GetRotation()).z;


	SpriteRenderer::Get()->DrawImmediate(
		sceneContext.d3dContext, m_pTexture->GetShaderResourceView(),
		XMFLOAT2(pos.x, pos.y), m_Color, 
		m_Pivot, XMFLOAT2{scale.x, scale.y}, rot);
}
