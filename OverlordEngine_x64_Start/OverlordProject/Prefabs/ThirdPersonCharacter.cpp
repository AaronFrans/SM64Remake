#include "stdafx.h"
#include "ThirdPersonCharacter.h"
#include "Goomba.h"

ThirdPersonCharacter::ThirdPersonCharacter(const CharacterDesc& characterDesc, physx::PxMaterial* physicsMaterial) :
	m_CharacterDesc{ characterDesc },
	m_MoveAcceleration(characterDesc.maxMoveSpeed / characterDesc.moveAccelerationTime),
	m_FallAcceleration(characterDesc.maxFallSpeed / characterDesc.fallAccelerationTime),
	m_pPhysicsMaterial{ physicsMaterial }
{
	m_CharacterDesc.actionId_Punch = characterDesc.actionId_Punch;
}

void ThirdPersonCharacter::Initialize(const SceneContext& /*sceneContext*/)
{
	const auto pFmod = SoundManager::Get()->GetSystem();

	auto result = pFmod->createStream("Resources/Sounds/Mario/Damage.wav", FMOD_2D, nullptr, &m_pDamage2D);


	result = pFmod->createStream("Resources/Sounds/Mario/Punch.wav", FMOD_2D, nullptr, &m_pPunch2D);


	result = pFmod->createStream("Resources/Sounds/Mario/Jump.wav", FMOD_2D, nullptr, &m_pJump2D);


	//Controller
	m_pControllerComponent = AddComponent(new ControllerComponent(m_CharacterDesc.controller));

	//Camera
	const auto pCamera = AddChild(new FixedCamera());
	m_pCameraComponent = pCamera->GetComponent<CameraComponent>();
	m_pCameraComponent->SetActive(true); //Uncomment to make this camera the active camera

	pCamera->GetTransform()->Translate(0, m_CharacterDesc.controller.height * .5f, -20);


	m_pPunchBox = AddChild(new GameObject);

	auto pPunchRB = m_pPunchBox->AddComponent(new RigidBodyComponent);
	pPunchRB->SetKinematic(true);
	PxVec3 scale{ 3, 2, 3 };

	pPunchRB->AddCollider(PxBoxGeometry{ scale / 2.0f }, *m_pPhysicsMaterial, true);

	m_pPunchBox->SetOnTriggerCallBack([&](GameObject*, GameObject* pOther, PxTriggerAction action) {
		if (!m_IsPunching)
			return;

		if (action == PxTriggerAction::ENTER && pOther->GetTag() == L"Goomba")
		{
			auto pGoomba = static_cast<Goomba*>(pOther);
			pGoomba->SetDeath();
		}

		});

}

void ThirdPersonCharacter::Update(const SceneContext& sceneContext)
{
	if (!m_pCameraComponent->IsActive())
		return;


	const auto& elapsedTime = sceneContext.pGameTime->GetElapsed();

	HandleDamage(elapsedTime);
	//constexpr float epsilon{ 0.01f }; //Constant that can be used to compare if a float is near zero

	//***************
	//HANDLE INPUT


	if (sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_Punch) && !m_IsPunching)
	{
		const auto pFmod = SoundManager::Get()->GetSystem();
		FMOD::Channel* pChannel{ nullptr };
		pFmod->playSound(m_pPunch2D, nullptr, false, &pChannel);
		m_IsPunching = true;
		m_pModelAnimator->SetAnimation(L"Punching");
	}



	if (m_IsPunching)
	{
		m_CurPunchTime += elapsedTime;
		if (m_CurPunchTime > PUNCH_TIME)
		{
			m_IsPunching = false;
			m_CurPunchTime = 0;
		}
	}



	//## Input Gathering (move)
	XMFLOAT2 move{ 0,0 };
	if (sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_MoveForward))
	{
		move.y = 1;
	}
	if (sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_MoveBackward))
	{
		move.y = -1;
	}

	//move.y should contain a 1 (Forward) or -1 (Backward) based on the active input (check corresponding actionId in m_CharacterDesc)
	//Optional: if move.y is near zero (abs(move.y) < epsilon), you could use the ThumbStickPosition.y for movement

	//move.x should contain a 1 (Right) or -1 (Left) based on the active input (check corresponding actionId in m_CharacterDesc)
	if (sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_MoveRight))
	{
		move.x = 1;
	}
	if (sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_MoveLeft))
	{
		move.x = -1;
	}
	//Optional: if move.x is near zero (abs(move.x) < epsilon), you could use the Left ThumbStickPosition.x for movement

	//## Input Gathering (look)
	XMFLOAT2 look{ 0.f, 0.f }; //Uncomment
	if (sceneContext.pInput->IsMouseButton(InputState::down, VK_LBUTTON))
	{
		const auto mousePos = sceneContext.pInput->GetMouseMovement();
		look = { static_cast<float>(mousePos.x), static_cast<float>(mousePos.y) };
	}
	//Only if the Left Mouse Button is Down >
		// Store the MouseMovement in the local 'look' variable (cast is required)
	//Optional: in case look.x AND look.y are near zero, you could use the Right ThumbStickPosition for look

	//************************
	//GATHERING TRANSFORM INFO

	//Retrieve the TransformComponent
	const auto pTransform = m_pCameraComponent->GetTransform();
	//Retrieve the forward & right vector (as XMVECTOR) from the TransformComponent
	const XMVECTOR forward = XMLoadFloat3(&pTransform->GetForward()),
		right = XMLoadFloat3(&pTransform->GetRight());

	//***************
	//CAMERA ROTATION

	//Adjust the TotalYaw (m_TotalYaw) & TotalPitch (m_TotalPitch) based on the local 'look' variable
	m_TotalYaw += look.x * m_CharacterDesc.rotationSpeed * elapsedTime;
	m_TotalPitch -= look.y * m_CharacterDesc.rotationSpeed * elapsedTime;
	//Make sure this calculated on a framerate independent way and uses CharacterDesc::rotationSpeed.
	//Rotate this character based on the TotalPitch (X) and TotalYaw (Y)
	m_TotalPitch = std::clamp(m_TotalPitch, -15.f, 50.f);
	GetTransform()->Rotate(m_TotalPitch, m_TotalYaw, 0);
	m_pModelComponent->GetTransform()->Rotate(m_TotalPitch, 180, 0);



	//********
	//MOVEMENT

	//## Horizontal Velocity (Forward/Backward/Right/Left)
	//Calculate the current move acceleration for this frame (m_MoveAcceleration * ElapsedTime)
	const auto curMoveAccel = m_MoveAcceleration * elapsedTime;
	//If the character is moving (= input is pressed)
	if (move.x != 0 || move.y != 0)
	{
		//Calculate & Store the current direction (m_CurrentDirection) >> based on the forward/right vectors and the pressed input
		XMStoreFloat3(&m_CurrentDirection, { forward * move.y + right * move.x });

		//Increase the current MoveSpeed with the current Acceleration (m_MoveSpeed)
		//Make sure the current MoveSpeed stays below the maximum MoveSpeed (CharacterDesc::maxMoveSpeed)
		m_MoveSpeed = std::min(m_MoveSpeed + curMoveAccel, m_CharacterDesc.maxMoveSpeed);

		if (m_TotalVelocity.y == 0 && !m_IsPunching)
		{

			m_pModelAnimator->SetAnimation(L"Running");
		}

	}
	//Else (character is not moving, or stopped moving)
	else
	{
		//Decrease the current MoveSpeed with the current Acceleration (m_MoveSpeed)
		//Make sure the current MoveSpeed doesn't get smaller than zero
		m_MoveSpeed = std::max(m_MoveSpeed - curMoveAccel, 0.f);
	}



	//Now we can calculate the Horizontal Velocity which should be stored in m_TotalVelocity.xz
	//Calculate the horizontal velocity (m_CurrentDirection * MoveSpeed)]

	//Set the x/z component of m_TotalVelocity (horizontal_velocity x/z)
	m_TotalVelocity.x = m_CurrentDirection.x * m_MoveSpeed;
	m_TotalVelocity.z = m_CurrentDirection.z * m_MoveSpeed;
	//It's important that you don't overwrite the y component of m_TotalVelocity (contains the vertical velocity)

	//## Vertical Movement (Jump/Fall)
	//If the Controller Component is NOT grounded (= freefall)
	if (!(m_pControllerComponent->GetCollisionFlags() & PxControllerCollisionFlag::eCOLLISION_DOWN))
	{
		//Decrease the y component of m_TotalVelocity with a fraction (ElapsedTime) of the Fall Acceleration (m_FallAcceleration)
		//Make sure that the minimum speed stays above -CharacterDesc::maxFallSpeed (negative!)
		m_TotalVelocity.y = std::max(m_TotalVelocity.y - m_FallAcceleration * elapsedTime, -m_CharacterDesc.maxFallSpeed);
	}
	//Else If the jump action is triggered
	else if (sceneContext.pInput->IsActionTriggered(m_CharacterDesc.actionId_Jump)) {
		//Set m_TotalVelocity.y equal to CharacterDesc::JumpSpeed
		m_TotalVelocity.y = m_CharacterDesc.JumpSpeed;
		if (!m_IsPunching)
		{
			const auto pFmod = SoundManager::Get()->GetSystem();
			FMOD::Channel* pChannel{ nullptr };
			pFmod->playSound(m_pJump2D, nullptr, false, &pChannel);
			m_pModelAnimator->SetAnimation(L"Jumping");
		}

	}
	//Else (=Character is grounded, no input pressed)
	else
	{
		//m_TotalVelocity.y is zero
		m_TotalVelocity.y = 0;

	}


	if (m_MoveSpeed == 0 && m_TotalVelocity.y == 0
		&& !m_IsPunching)
	{
		m_pModelAnimator->SetAnimation(L"Idle");
	}


	//************
	//DISPLACEMENT

	//The displacement required to move the Character Controller (ControllerComponent::Move) can be calculated using our TotalVelocity (m/s)
	//Calculate the displacement (m) for the current frame and move the ControllerComponent


	XMVECTOR orignalTotalVelocity = XMLoadFloat3(&m_TotalVelocity);
	orignalTotalVelocity = XMVectorScale(orignalTotalVelocity, elapsedTime);
	XMFLOAT3 curVelocity;
	XMStoreFloat3(&curVelocity, orignalTotalVelocity);
	m_pControllerComponent->Move(curVelocity);


	//The above is a simple implementation of Movement Dynamics, adjust the code to further improve the movement logic and behaviour.
	//Also, it can be usefull to use a seperate RayCast to check if the character is grounded (more responsive)

	//Move Hitbox
	auto playerPos = m_pControllerComponent->GetPosition();
	XMFLOAT3 moveDir{};
	XMStoreFloat3(&moveDir, { forward * 1.5f + right * 0 });
	m_pPunchBox->GetTransform()->Translate(playerPos.x + moveDir.x, playerPos.y, playerPos.z + moveDir.z);


	m_pPunchBox->GetTransform()->Rotate(0, m_TotalYaw, 0);



}

void ThirdPersonCharacter::HandleDamage(float elapsed)
{
	if (!m_IsDamaged)
		return;

	m_CurDamageTime += elapsed;

	if (m_CurModelSwitchTime >= MODEL_SWITCH_MIN_TIME)
	{
		m_pModelComponent->SetActive(!m_pModelComponent->IsAcive());
		m_CurModelSwitchTime = 0;
	}

	m_CurModelSwitchTime += elapsed;
	if (m_CurDamageTime >= DAMAGE_MIN_TIME)
	{
		m_IsDamaged = false;
		m_CurDamageTime = 0;
		m_CurModelSwitchTime = 0;

		m_pModelComponent->SetActive(true);
	}


}

void ThirdPersonCharacter::DrawImGui()
{
	if (ImGui::CollapsingHeader("Character"))
	{
		ImGui::Text(std::format("Move Speed: {:0.1f} m/s", m_MoveSpeed).c_str());
		ImGui::Text(std::format("Fall Speed: {:0.1f} m/s", m_TotalVelocity.y).c_str());

		ImGui::Text(std::format("Move Acceleration: {:0.1f} m/s2", m_MoveAcceleration).c_str());
		ImGui::Text(std::format("Fall Acceleration: {:0.1f} m/s2", m_FallAcceleration).c_str());

		const float jumpMaxTime = m_CharacterDesc.JumpSpeed / m_FallAcceleration;
		const float jumpMaxHeight = (m_CharacterDesc.JumpSpeed * jumpMaxTime) - (0.5f * (m_FallAcceleration * powf(jumpMaxTime, 2)));
		ImGui::Text(std::format("Jump Height: {:0.1f} m", jumpMaxHeight).c_str());

		ImGui::Dummy({ 0.f,5.f });
		if (ImGui::DragFloat("Max Move Speed (m/s)", &m_CharacterDesc.maxMoveSpeed, 0.1f, 0.f, 0.f, "%.1f") ||
			ImGui::DragFloat("Move Acceleration Time (s)", &m_CharacterDesc.moveAccelerationTime, 0.1f, 0.f, 0.f, "%.1f"))
		{
			m_MoveAcceleration = m_CharacterDesc.maxMoveSpeed / m_CharacterDesc.moveAccelerationTime;
		}

		ImGui::Dummy({ 0.f,5.f });
		if (ImGui::DragFloat("Max Fall Speed (m/s)", &m_CharacterDesc.maxFallSpeed, 0.1f, 0.f, 0.f, "%.1f") ||
			ImGui::DragFloat("Fall Acceleration Time (s)", &m_CharacterDesc.fallAccelerationTime, 0.1f, 0.f, 0.f, "%.1f"))
		{
			m_FallAcceleration = m_CharacterDesc.maxFallSpeed / m_CharacterDesc.fallAccelerationTime;
		}

		ImGui::Dummy({ 0.f,5.f });
		ImGui::DragFloat("Jump Speed", &m_CharacterDesc.JumpSpeed, 0.1f, 0.f, 0.f, "%.1f");
		ImGui::DragFloat("Rotation Speed (deg/s)", &m_CharacterDesc.rotationSpeed, 0.1f, 0.f, 0.f, "%.1f");

		bool isActive = m_pCameraComponent->IsActive();
		if (ImGui::Checkbox("Character Camera", &isActive))
		{
			m_pCameraComponent->SetActive(isActive);
		}
	}
}

void ThirdPersonCharacter::SetMinCameraAngle(float angle)
{
	m_MinCameraAngle = angle;
}

void ThirdPersonCharacter::SetMaxCameraAngle(float angle)
{
	m_MaxCameraAngle = angle;
}

void ThirdPersonCharacter::SetModel(ModelComponent* modelComponent)
{
	m_pModelComponent = modelComponent;
}

void ThirdPersonCharacter::SetAnimator(ModelAnimator* modelAnimator)
{
	m_pModelAnimator = modelAnimator;
}

void ThirdPersonCharacter::Jump()
{
	m_TotalVelocity.y = m_CharacterDesc.JumpSpeed;
}

void ThirdPersonCharacter::Damage()
{
	m_IsDamaged = true;
	m_CurDamageTime = 0;
	m_CurModelSwitchTime = 0;
	m_pModelComponent->SetActive(false);
	const auto pFmod = SoundManager::Get()->GetSystem();
	FMOD::Channel* pChannel{ nullptr };
	pFmod->playSound(m_pDamage2D, nullptr, false, &pChannel);

}

const FixedCamera* ThirdPersonCharacter::GetCamera()
{
	return static_cast<FixedCamera*>(m_pCameraComponent->GetGameObject());
}
