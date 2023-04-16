#include "stdafx.h"
#include "SceneSelector.h"
#include "SceneManager.h"

//Change this define to activate/deactive the corresponding scenes
// W01 - W02  (#define ...)

#define W01
#define W02

#ifdef W01
#include "TestScene.h"
#include "PhysXTestScene.h"
#include "W1_AssignmentScene.h"

#endif

#ifdef W02
#include "TutorialSceneWeek2.h"
#include "TriggerScene.h"
#include "AudioTestScene.h"
#include "W2_AssignmentScene.h"
#endif

namespace dae
{
	void AddScenes(SceneManager * pSceneManager)
	{

#ifdef W01
		pSceneManager->AddGameScene(new W1_AssignmentScene());
#endif

#ifdef W02
		pSceneManager->AddGameScene(new W2_AssignmentScene());
#endif

	}
}

