#pragma once


class UberMaterial;
class Button;
class Coin;
class Goomba;
class ThirdPersonCharacter;
class ParticleEmmiter;
class PostUnderwater;
class PostBlur;
class MarioScene final : public GameScene
{
public:
	MarioScene() : GameScene(L"MarioScene") {}
	~MarioScene() override;
	MarioScene(const MarioScene& other) = delete;
	MarioScene(MarioScene&& other) noexcept = delete;
	MarioScene& operator=(const MarioScene& other) = delete;
	MarioScene& operator=(MarioScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void OnGUI() override;


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

	void MakeLevel(physx::PxMaterial* physicsMaterial);

	void MakeUnderwater(physx::PxMaterial* physicsMaterial);

	void MakeUI();

	void MakePauseMenu();

	void Pause();
	void Resume();

	void MakeSkybox();

	void Reset();

	//Character
	ThirdPersonCharacter* m_pMario{};


	//Underwater Volume
	unsigned m_NrCoinsPickedUp{};
	PostUnderwater* m_pPostUnderwater;

	physx::PxMaterial* pDefaultPhysxMat{};
	std::vector<Coin*> m_Coins{};
	std::vector<Goomba*> m_Goombas{};


	//Current Material Being Debugged
	UberMaterial* m_pDebugMat{};

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

	std::string m_CoinsGotten{ "Nr of Coins %d", m_NrCoinsPickedUp };
	XMFLOAT2 m_CoinsGottenPosition{};
	XMFLOAT4 m_TextColor{ 189 / 255.0f, 183 / 255.0f, 107 / 255.0f, 1 };

	std::vector<SpriteComponent*> m_PauseMenuSprites{};
	std::vector<Button*> m_Buttons{};
	PostBlur* m_pPostBlur{};
	bool m_IsPaused{ false };


	float m_ShadowMapScale{ 0.3f };
};