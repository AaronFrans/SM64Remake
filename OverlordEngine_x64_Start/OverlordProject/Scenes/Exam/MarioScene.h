#pragma once


class UberMaterial;
class Button;
class Coin;
class Goomba;
class ThirdPersonCharacter;
class ParticleEmmiter;
class PostUnderwater;
class PostBlur;
class FlowingWater;
class MarioScene final : public GameScene
{
public:
	MarioScene() : GameScene(L"MarioScene") {}
	~MarioScene() override;
	MarioScene(const MarioScene& other) = delete;
	MarioScene(MarioScene&& other) noexcept = delete;
	MarioScene& operator=(const MarioScene& other) = delete;
	MarioScene& operator=(MarioScene&& other) noexcept = delete;


	void RemoveHealth();

protected:
	void Initialize() override;
	void OnGUI() override;
	void OnSceneActivated() override;
	void OnSceneDeactivated() override;

	void Update() override;

	void PostDraw() override;
private:


	enum InputIds
	{
		CharacterMoveLeft,
		CharacterMoveRight,
		CharacterMoveForward,
		CharacterMoveBackward,
		CharacterJump,
		CharacterPunch,
		PauseGame
	};

	void MakeMario(physx::PxMaterial* physicsMaterial);

	void MakeBubbleEmitter(float x, float y, float z);

	void MakeCoin(float x, float y, float z, physx::PxMaterial* physicsMaterial);
	void MakeGoomba(float x, float y, float z, physx::PxMaterial* physicsMaterial);

	void MakeLevel();

	void MakeUnderwater(physx::PxMaterial* physicsMaterial);

	void MakeUI();
	void MakeHearts();

	void MakePauseMenu();

	void Pause();
	void Resume();

	void MakeSkybox();

	void Reset();

	//Character
	ThirdPersonCharacter* m_pMario{};


	//Underwater Volume
	PostUnderwater* m_pPostUnderwater;

	physx::PxMaterial* m_pDefaultPhysxMat{};
	physx::PxMaterial* m_pSlipperyPhysxMat{};
	std::vector<Coin*> m_Coins{};
	std::vector<Goomba*> m_Goombas{};


	//Current Material Being Debugged
	FlowingWater* m_pDebugMat{};

	//Current GameObject Being Debugged
	GameObject* PositionTemp{};
	float rot[4]{ 0,0,0,0 };

	//Scales
	const PxVec3 CorrectScale{ 20,20,20 };
	const PxVec3 MarioScale{ 0.01f, 0.01f, 0.01f };


	//Animation
	ModelAnimator* m_pAnimator{};
	int m_AnimationClipId{ 0 };
	float m_AnimationSpeed{ 1.f };


	//Ui
	SpriteFont* m_pFont{};
	GameObject* m_pHeartsHud{};
	std::vector<GameObject*> m_pHearts{};
	XMFLOAT4 m_TextColor{ 189 / 255.0f, 183 / 255.0f, 107 / 255.0f, 1 };



	//Lives
	int m_NrOfLives{ 3 };
	std::string m_LivesLeft{ "Nr of Lives %d", m_NrOfLives };
	XMFLOAT2 m_LivesLeftPosition{};
	bool m_IsDead{ false };


	//Coins
	unsigned m_NrTotalCoins{};
	unsigned m_NrCoinsPickedUp{};
	std::string m_CoinsGotten{ "Nr of Coins %d", m_NrCoinsPickedUp };
	XMFLOAT2 m_CoinsGottenPosition{};


	//Pause
	std::vector<SpriteComponent*> m_PauseMenuSprites{};
	std::vector<Button*> m_Buttons{};
	PostBlur* m_pPostBlur{};
	bool m_IsPaused{ false };

	float m_ShadowMapScale{ 0.3f };


	float m_LightOffset[3]{ 0, 0,0 };
	XMFLOAT4 m_LightDirection{};


	FMOD::Channel* m_pChannel2D{ nullptr };
};