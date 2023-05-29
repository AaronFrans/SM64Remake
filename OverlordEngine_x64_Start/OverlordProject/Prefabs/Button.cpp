#include "stdafx.h"
#include "Button.h"

Button::Button(std::wstring texturePath, const std::function<void()> func)
	: m_FilePath{ texturePath }
	, m_OnClick{ func }
{

}

void Button::OnClicked(float xPos, float yPos)
{

	if (!m_pTexture->IsActive())
		return;

	const auto pos = GetTransform()->GetWorldPosition();
	const auto dimensions = m_pTexture->GetDimensions();
	if (xPos >= pos.x && xPos <= pos.x + dimensions.x &&
		yPos >= pos.y && yPos <= pos.y + dimensions.y)
	{
		m_OnClick();
	}
}


void Button::Initialize(const SceneContext&)
{
	m_pTexture = AddComponent(new SpriteComponent(m_FilePath));
}
