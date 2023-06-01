#pragma once

class Button;
class VictoryScene : public GameScene
{
public:
	VictoryScene() : GameScene(L"VictoryScene") {}
	~VictoryScene() override = default;
	VictoryScene(const VictoryScene& other) = delete;
	VictoryScene(VictoryScene&& other) noexcept = delete;
	VictoryScene& operator=(const VictoryScene& other) = delete;
	VictoryScene& operator=(VictoryScene&& other) noexcept = delete;

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
