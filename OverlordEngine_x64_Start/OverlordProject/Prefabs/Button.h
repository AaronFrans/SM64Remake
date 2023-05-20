#pragma once
class Button final : public GameObject
{

public:
	Button(std::wstring texturePath, const std::function<void()> func);


	~Button() override = default;
	Button(const Button& other) = delete;
	Button(Button&& other) noexcept = delete;
	Button& operator=(const Button& other) = delete;
	Button& operator=(Button&& other) noexcept = delete;



	void OnClicked(float xPos, float yPos);

protected:

	void Initialize(const SceneContext&) override;

private:

	std::function<void()> m_OnClick;

	SpriteComponent* m_pTexture;

	std::wstring m_FilePath;

};

