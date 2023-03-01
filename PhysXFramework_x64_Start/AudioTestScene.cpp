#include "stdafx.h"
#include "AudioTestScene.h"

#include "ContentManager.h"
#include "MeshObject.h"
#include "SpherePosColorNorm.h"

#include "Logger.h"
#include "SoundManager.h"

enum InputIds
{
	Play2D,
	Play3D,
	IncreaseVolume,
	DecreaseVolume,
};


void AudioTestScene::Initialize()
{
	EnablePhysxDebugRendering(true);
	const auto pPhysX = PhysxManager::GetInstance()->GetPhysics();
	const PxMaterial* pPhysXMat = pPhysX->createMaterial(0.5, 0.5, 0.2f);

	auto pGround = pPhysX->createRigidStatic(PxTransform{ PxQuat{PxPiDivTwo, PxVec3{0,0,1}} });
	PxRigidActorExt::createExclusiveShape(*pGround, PxPlaneGeometry{}, *pPhysXMat);
	m_pPhysxScene->addActor(*pGround);


	m_pSphere = new SpherePosColorNorm(1.f, 10, 10, XMFLOAT4{ Colors::Gray });
	AddGameObject(m_pSphere);

	const auto pSphereActor = pPhysX->createRigidDynamic(PxTransform{ PxIdentity });
	PxRigidActorExt::createExclusiveShape(*pSphereActor, PxSphereGeometry{ 1.f }, *pPhysXMat);

	m_pSphere->AttachRigidActor(pSphereActor);

	m_pSphere->Translate(0, 2, 0);

	//INPUT
	const auto pInput = m_SceneContext.GetInput();

	pInput->AddInputAction({ Play2D, InputTriggerState::pressed, VK_SPACE });
	pInput->AddInputAction({ Play3D, InputTriggerState::pressed,  'B' });
	pInput->AddInputAction({ IncreaseVolume, InputTriggerState::pressed, VK_UP });
	pInput->AddInputAction({ DecreaseVolume, InputTriggerState::pressed, VK_DOWN });



	const auto pFmod = SoundManager::GetInstance()->GetSystem();
	//2D
	FMOD::Sound* pSound2D{ nullptr };
	auto result = pFmod->createStream("Resources/Sound/wave.mp3", FMOD_2D | FMOD_LOOP_NORMAL, nullptr, &pSound2D);
	SoundManager::GetInstance()->ErrorCheck(result);

	result = pFmod->playSound(pSound2D, nullptr, false, &m_pChannel2D);
	SoundManager::GetInstance()->ErrorCheck(result);

	//3D
	FMOD::Sound* pSound3D{ nullptr };
	result = pFmod->createStream("Resources/Sound/wave.mp3", FMOD_3D | FMOD_LOOP_NORMAL | FMOD_3D_LINEARROLLOFF, nullptr, &pSound3D);
	SoundManager::GetInstance()->ErrorCheck(result);

	result = pFmod->playSound(pSound3D, nullptr, false, &m_pChannel3D);
	SoundManager::GetInstance()->ErrorCheck(result);


	m_pChannel3D->set3DMinMaxDistance(0, 100);


}

inline FMOD_VECTOR ToFmod(XMFLOAT3 v)
{
	return FMOD_VECTOR(v.x, v.y, v.z);
}

inline FMOD_VECTOR ToFmod(PxVec3 v)
{
	return FMOD_VECTOR(v.x, v.y, v.z);
}

void AudioTestScene::Update()
{
	constexpr float force{ 5 };

	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::down, VK_LEFT))
	{
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addTorque({ 0, 0, force });
	}
	if (m_SceneContext.GetInput()->IsKeyboardKey(InputTriggerState::down, VK_RIGHT))
	{
		m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->addTorque({ 0, 0, -force });
	}

	if (m_SceneContext.GetInput()->IsActionTriggered(Play2D))
	{
		bool bPaused{  };
		auto result = m_pChannel2D->isPlaying(&bPaused);

		m_pChannel2D->setPaused(!bPaused);
	}
	if (m_SceneContext.GetInput()->IsActionTriggered(Play3D))
	{
		bool bPaused{  };
		auto result = m_pChannel3D->isPlaying(&bPaused);

		m_pChannel2D->setPaused(!bPaused);
	}

	if (m_SceneContext.GetInput()->IsActionTriggered(IncreaseVolume))
	{
		float bVolume{  };
		auto result = m_pChannel2D->getVolume(&bVolume);

		bVolume += 0.1f;

		Clamp(bVolume, 1.f, 0.f);

		m_pChannel2D->setVolume(bVolume);
		Logger::GetInstance()->LogFormat(LogLevel::Info, L"Volume Changed >> %f", bVolume);

		result = m_pChannel3D->getVolume(&bVolume);

		bVolume += 0.1f;

		Clamp(bVolume, 1.f, 0.f);

		m_pChannel3D->setVolume(bVolume);
		Logger::GetInstance()->LogFormat(LogLevel::Info, L"Volume Changed >> %f", bVolume);

	}

	if (m_SceneContext.GetInput()->IsActionTriggered(DecreaseVolume))
	{
		float bVolume{  };
		auto result = m_pChannel2D->getVolume(&bVolume);

		bVolume -= 0.1f;

		Clamp(bVolume, 1.f, 0.f);

		m_pChannel2D->setVolume(bVolume);


		Logger::GetInstance()->LogFormat(LogLevel::Info, L"Volume Changed >> %f", bVolume);

		result = m_pChannel3D->getVolume(&bVolume);

		bVolume -= 0.1f;

		Clamp(bVolume, 1.f, 0.f);

		m_pChannel3D->setVolume(bVolume);
		Logger::GetInstance()->LogFormat(LogLevel::Info, L"Volume Changed >> %f", bVolume);
	}

	auto cam = m_SceneContext.GetCamera();
	auto pos = ToFmod(cam->GetPosition());
	auto foward = ToFmod(cam->GetForward());
	auto up = ToFmod(cam->GetUp());

	const float dT = m_SceneContext.GetGameTime()->GetElapsed();
	FMOD_VECTOR vel{ (pos.x - m_PrevPos.x) / dT, (pos.y - m_PrevPos.y) / dT, (pos.z - m_PrevPos.z) / dT, };
	m_PrevPos = pos;

	SoundManager::GetInstance()->GetSystem()->set3DListenerAttributes(0, &pos, &vel, &foward, &up);

	auto spherePos = ToFmod(m_pSphere->GetPosition());
	auto sphereVel = ToFmod(m_pSphere->GetRigidActor()->is<PxRigidDynamic>()->getLinearVelocity());

	m_pChannel3D->set3DAttributes(&spherePos, &sphereVel);
}

void AudioTestScene::Draw() const
{

}

void AudioTestScene::OnSceneActivated()
{
	Logger::GetInstance()->LogFormat(LogLevel::Info, L"Scene Activated > \"%ls\"", GetName().c_str());
}

void AudioTestScene::OnSceneDeactivated()
{
}
