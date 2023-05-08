#pragma once


class UberMaterial;
class MarioScene : public GameScene
{
public:
	MarioScene() : GameScene(L"MarioScene") {}
	~MarioScene() override = default;
	MarioScene(const MarioScene& other) = delete;
	MarioScene(MarioScene&& other) noexcept = delete;
	MarioScene& operator=(const MarioScene& other) = delete;
	MarioScene& operator=(MarioScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void OnGUI() override;

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


	void MakeLevel(physx::PxMaterial* physicsMaterial);


	//Current Material Being Debugged
	UberMaterial* m_pDebugMat{};

	const PxVec3 CorrectScale{ 20,20,20 };
};