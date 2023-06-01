#pragma once
struct CharacterDesc
{
	CharacterDesc(
		PxMaterial* pMaterial,
		float radius = 1,
		float height = 2.2f)
	{
		controller.setToDefault();
		controller.radius = radius;
		controller.height = height;
		controller.material = pMaterial;
	}

	float maxMoveSpeed{ 15.f };
	float maxFallSpeed{ 30.f };

	float JumpSpeed{ 50.f };

	float moveAccelerationTime{ .3f };
	float fallAccelerationTime{ 0.2f };

	PxCapsuleControllerDesc controller{};

	float rotationSpeed{ 60.f };

	int actionId_MoveLeft{ -1 };
	int actionId_MoveRight{ -1 };
	int actionId_MoveForward{ -1 };
	int actionId_MoveBackward{ -1 };
	int actionId_Jump{ -1 };
	int actionId_Punch{ -1 };
};


class ThirdPersonCharacter : public GameObject
{
public:
	ThirdPersonCharacter(const CharacterDesc& characterDesc, physx::PxMaterial* physicsMaterial);
	~ThirdPersonCharacter() override = default;

	ThirdPersonCharacter(const ThirdPersonCharacter& other) = delete;
	ThirdPersonCharacter(ThirdPersonCharacter&& other) noexcept = delete;
	ThirdPersonCharacter& operator=(const ThirdPersonCharacter& other) = delete;
	ThirdPersonCharacter& operator=(ThirdPersonCharacter&& other) noexcept = delete;

	void DrawImGui();

	void SetMinCameraAngle(float angle);
	void SetMaxCameraAngle(float angle);

	void SetModel(ModelComponent* modelComponent);
	void SetAnimator(ModelAnimator* modelAnimator);

	void Jump();

	void Damage();
	bool IsDamaged() { return m_IsDamaged; };

	void ResetCamera() { m_TotalPitch = 0; m_TotalYaw = 0; };

	const FixedCamera* GetCamera();

protected:
	void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override;

private:
	CameraComponent* m_pCameraComponent{};
	ControllerComponent* m_pControllerComponent{};

	ModelComponent* m_pModelComponent{};
	ModelAnimator* m_pModelAnimator{};

	GameObject* m_pPunchBox{};

	CharacterDesc m_CharacterDesc;
	float m_TotalPitch{}, m_TotalYaw{};				//Total camera Pitch(X) and Yaw(Y) rotation
	float m_MoveAcceleration{},						//Acceleration required to reach maxMoveVelocity after 1 second (maxMoveVelocity / moveAccelerationTime)
		m_FallAcceleration{},						//Acceleration required to reach maxFallVelocity after 1 second (maxFallVelocity / fallAccelerationTime)
		m_MoveSpeed{};								//MoveSpeed > Horizontal Velocity = MoveDirection * MoveVelocity (= TotalVelocity.xz)

	XMFLOAT3 m_TotalVelocity{};						//TotalVelocity with X/Z for Horizontal Movement AND Y for Vertical Movement (fall/jump)
	XMFLOAT3 m_CurrentDirection{};					//Current/Last Direction based on Camera forward/right (Stored for deacceleration)

	physx::PxMaterial* m_pPhysicsMaterial{};

	float m_MinCameraAngle{ 30 };
	float m_MaxCameraAngle{ 70 };

	bool m_IsPunching{ false };

	const static int PUNCH_TIME{ 1 };

	float m_CurPunchTime{ 0 };

	bool m_IsDamaged{ false };
	bool m_IsModelActive{ false };


	float m_CurDamageTime{ 0 };
	const static int DAMAGE_MIN_TIME{ 1 };
	float m_CurModelSwitchTime{ 0 };
	constexpr static float MODEL_SWITCH_MIN_TIME{ 0.25f };


	void HandleDamage(float elapsed);

};

