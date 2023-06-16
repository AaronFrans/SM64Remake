#pragma once

class CubePrefab;
class SpherePrefab;
class  ModelTestScene final : public GameScene
{
public:

	ModelTestScene();
	~ModelTestScene() override = default;

	ModelTestScene(const  ModelTestScene& other) = delete;
	ModelTestScene(ModelTestScene&& other) noexcept = delete;
	ModelTestScene& operator=(const  ModelTestScene& other) = delete;
	ModelTestScene& operator=(ModelTestScene&& other) noexcept = delete;

protected:
	void Initialize() override;

private:

	GameObject* m_pChair{ nullptr };

};