#pragma once


class UberMaterial;
class Coin;
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


	//Coins
	std::vector<Coin*> m_Coins{};
	float m_CoinsRotations{ 0 };

	int m_NrCoins{};

	//Current Material Being Debugged
	UberMaterial* m_pDebugMat{};

	GameObject* m_pMario{};
	GameObject* PositionTemp{};

	const PxVec3 CorrectScale{ 20,20,20 };
	const PxVec3 MarioScale{ 0.01f, 0.01f, 0.01f };

	float rot[4]{ };


	//Animation
	ModelAnimator* m_pAnimator{};
	int m_AnimationClipId{ 0 };
	float m_AnimationSpeed{ 1.f };

	char** m_ClipNames{};
	UINT m_ClipCount{};
};