#pragma once

class Button;
class DeathScene : public GameScene
{
public:
	DeathScene() : GameScene(L"DeathScene") {}
	~DeathScene() override = default;
	DeathScene(const DeathScene& other) = delete;
	DeathScene(DeathScene&& other) noexcept = delete;
	DeathScene& operator=(const DeathScene& other) = delete;
	DeathScene& operator=(DeathScene&& other) noexcept = delete;

protected:
	void Initialize() override;

	void Update() override;
	void OnGUI() override;
	void OnSceneActivated() override;
	void OnSceneDeactivated() override;

private:


	std::vector<Button*> m_Buttons;
	FMOD::Channel* m_pChannel2D{ nullptr };

};
