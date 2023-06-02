#pragma once




class Button;
class MainMenuScene : public GameScene
{
public:
	MainMenuScene() : GameScene(L"MainMenuScene") {}
	~MainMenuScene() override = default;
	MainMenuScene(const MainMenuScene& other) = delete;
	MainMenuScene(MainMenuScene&& other) noexcept = delete;
	MainMenuScene& operator=(const MainMenuScene& other) = delete;
	MainMenuScene& operator=(MainMenuScene&& other) noexcept = delete;

protected:
	void Initialize() override;

	void Update() override;
	void OnGUI() override;
	void OnSceneActivated() override;
	void OnSceneDeactivated() override;

private:

	
	FMOD::Channel* m_pChannel2D{ nullptr };


	unsigned m_CurButton{0};
	std::vector<Button*> m_Buttons;

};

