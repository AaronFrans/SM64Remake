#include "stdafx.h"
#include "Button.h"

Button::Button(std::wstring texturePath, const std::function<void()> func, const std::string& sfxPath)
	: m_FilePath{ texturePath }
	, m_OnClick{ func }
	, m_SfxPath{ sfxPath }
{
}

void Button::OnClicked(float xPos, float yPos)
{

	const auto pFmod = SoundManager::Get()->GetSystem();
	FMOD::Sound* pSound2D{ nullptr };
	auto result = pFmod->createStream(m_SfxPath.c_str(), FMOD_2D, nullptr, &pSound2D);
	result = pFmod->playSound(pSound2D, nullptr, false, &m_pChannel2D);
	m_pChannel2D->setPaused(true);


	if (!m_pTexture->IsActive())
		return;

	const auto pos = GetTransform()->GetWorldPosition();
	const auto dimensions = m_pTexture->GetDimensions();
	if (xPos >= pos.x && xPos <= pos.x + dimensions.x &&
		yPos >= pos.y && yPos <= pos.y + dimensions.y)
	{

		m_pChannel2D->setPaused(false);
		m_OnClick();
	}
}


void Button::Initialize(const SceneContext&)
{
	m_pTexture = AddComponent(new SpriteComponent(m_FilePath));
}
