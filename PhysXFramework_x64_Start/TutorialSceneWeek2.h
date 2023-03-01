#pragma once
#include "GameScene.h"
class TutorialSceneWeek2 final : public GameScene
{
public:
	TutorialSceneWeek2() :GameScene(L"TutorialSceneWeek2") {}
	~TutorialSceneWeek2() override = default;
	TutorialSceneWeek2(const TutorialSceneWeek2& other) = delete;
	TutorialSceneWeek2(TutorialSceneWeek2&& other) noexcept = delete;
	TutorialSceneWeek2& operator=(const TutorialSceneWeek2& other) = delete;
	TutorialSceneWeek2& operator=(TutorialSceneWeek2&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() const override;
	void OnSceneActivated() override;
	void OnSceneDeactivated() override;

private:

	GameObject* m_pChairConvex{ nullptr };

	GameObject* m_pChairTriangle{ nullptr };

	PxVec3 m_KinematicPostion{};
};

