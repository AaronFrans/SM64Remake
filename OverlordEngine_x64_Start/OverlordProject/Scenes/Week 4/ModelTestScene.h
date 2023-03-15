#pragma once

class CubePrefab;
class SpherePrefab;
class  ModelTestScene final : public GameScene
{
public:
	enum Inputs
	{
		Start,
		Reset,
		LeftUp,
		LeftDown,
		RigthUp,
		RigthDown,
	};

	ModelTestScene();
	~ModelTestScene() override = default;

	ModelTestScene(const  ModelTestScene& other) = delete;
	ModelTestScene(ModelTestScene&& other) noexcept = delete;
	ModelTestScene& operator=(const  ModelTestScene& other) = delete;
	ModelTestScene& operator=(ModelTestScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void PostInitialize() override;

	void Update() override;

private:

	GameObject* m_pChair{ nullptr };

};
