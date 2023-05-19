#pragma once
class MainMenuScene : public GameScene
{
public:
	MainMenuScene() : GameScene(L"MarioScene") {}
	~MainMenuScene() override = default;
	MainMenuScene(const MainMenuScene& other) = delete;
	MainMenuScene(MainMenuScene&& other) noexcept = delete;
	MainMenuScene& operator=(const MainMenuScene& other) = delete;
	MainMenuScene& operator=(MainMenuScene&& other) noexcept = delete;

protected:
	void Initialize() override;

	void Update() override;
	void OnGUI() override;

private:

	SpriteFont* m_pFont{};

	std::string m_Text{ "Hello World! (Change Text/Position/Color in Scene Parameters)" };
	XMFLOAT2 m_TextPosition{};
	XMFLOAT4 m_TextColor{ 1.f,1.f,1.f,1.f };

};

