
#pragma once
#include "GameScene.h"
class TriggerScene final : public GameScene
{
public:
	TriggerScene() :GameScene(L"TriggerScene") {}
	~TriggerScene() override = default;
	TriggerScene(const TriggerScene& other) = delete;
	TriggerScene(TriggerScene&& other) noexcept = delete;
	TriggerScene& operator=(const TriggerScene& other) = delete;
	TriggerScene& operator=(TriggerScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() const override;
	void OnSceneActivated() override;
	void OnSceneDeactivated() override;

	void onTrigger(PxTriggerPair* pairs, PxU32 count) override;

private:

	GameObject* m_pSphere{ nullptr };
	PxRigidStatic* m_pTriggerLeft{ nullptr };
	PxRigidStatic* m_pTriggerRight{ nullptr };

	bool m_IsTriggerFound{ false };
};

