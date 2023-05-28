#pragma once


class UberMaterial;
class Coin;
class ParticleEmmiter;
class PostUnderwater;
class ThirdPersonCharacter;
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
		CharacterJump
	};

	void MakeMario(physx::PxMaterial* physicsMaterial);

	void MakeBubbleEmitter(float x, float y, float z);

	void MakeCoin(float x, float y, float z, physx::PxMaterial* physicsMaterial);

	void MakeLevel(physx::PxMaterial* physicsMaterial);

	void MakeUnderwater(physx::PxMaterial* physicsMaterial);

	//Character
	ThirdPersonCharacter* m_pMario{};


	//Underwater Volume
	unsigned m_NrCoinsPickedUp{};
	PostUnderwater* m_pPostUnderwater;



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


	float m_ShadowMapScale{ 0.3f };
};