#pragma once
class Button final : public GameObject
{

public:
	Button(std::wstring texturePath, std::wstring activeTexturePath, const std::function<void()> func, const std::string& sfxPath);


	~Button() override = default;
	Button(const Button& other) = delete;
	Button(Button&& other) noexcept = delete;
	Button& operator=(const Button& other) = delete;
	Button& operator=(Button&& other) noexcept = delete;



	void OnClicked(float xPos, float yPos);
	void SetSelected(bool isSelected) {
		m_pTexture->SetActive(!isSelected);
		m_pActiveTexture->SetActive(isSelected);
	};

	void DoOnClick();

	void Deactivate();
	void Activate();

protected:

	void Initialize(const SceneContext&) override;

private:

	std::function<void()> m_OnClick;

	SpriteComponent* m_pTexture;
	SpriteComponent* m_pActiveTexture;

	std::wstring m_FilePath;
	std::wstring m_ActiveFilePath;

	FMOD::Sound* m_pOnClick2D{ nullptr };

	std::string m_SfxPath{};
};

