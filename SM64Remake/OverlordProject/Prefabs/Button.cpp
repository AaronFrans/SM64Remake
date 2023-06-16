#include "stdafx.h"
#include "Button.h"

Button::Button(std::wstring texturePath, std::wstring activeTexturePath, const std::function<void()> func, const std::string& sfxPath)
	: m_FilePath{ texturePath }
	, m_ActiveFilePath{ activeTexturePath }
	, m_OnClick{ func }
	, m_SfxPath{ sfxPath }
{
}

void Button::OnClicked(float xPos, float yPos)
{

	


	if (!m_pTexture->IsActive() && !m_pActiveTexture->IsActive())
		return;

	const auto pos = GetTransform()->GetWorldPosition();
	const auto dimensions = m_pTexture->GetDimensions();
	if (xPos >= pos.x && xPos <= pos.x + dimensions.x &&
		yPos >= pos.y && yPos <= pos.y + dimensions.y)
	{

		const auto pFmod = SoundManager::Get()->GetSystem();
		FMOD::Channel* pChannel{ nullptr };
		pFmod->playSound(m_pOnClick2D, nullptr, false, &pChannel);

		m_OnClick();
	}
}


void Button::DoOnClick()
{
	const auto pFmod = SoundManager::Get()->GetSystem();
	FMOD::Channel* pChannel{ nullptr };
	pFmod->playSound(m_pOnClick2D, nullptr, false, &pChannel);
	m_OnClick();
}

void Button::Deactivate()
{
	m_pTexture->SetActive(false);
	m_pActiveTexture->SetActive(false);
}

void Button::Activate()
{
	m_pTexture->SetActive(true);
	m_pActiveTexture->SetActive(false);
}

void Button::Initialize(const SceneContext&)
{
	const auto pFmod = SoundManager::Get()->GetSystem();

	pFmod->createStream(m_SfxPath.c_str(), FMOD_2D, nullptr, &m_pOnClick2D);


	m_pTexture = AddComponent(new SpriteComponent(m_FilePath));
	m_pActiveTexture = AddComponent(new SpriteComponent(m_ActiveFilePath));
	m_pActiveTexture->SetActive(false);
}
