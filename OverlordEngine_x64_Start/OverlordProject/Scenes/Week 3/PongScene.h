#pragma once

class CubePrefab;
class SpherePrefab;
class  PongScene final : public GameScene
{
public:
	PongScene();
	~PongScene() override = default;

	PongScene(const  PongScene& other) = delete;
	PongScene(PongScene&& other) noexcept = delete;
	PongScene& operator=(const  PongScene& other) = delete;
	PongScene& operator=(PongScene&& other) noexcept = delete;

protected:
	void Initialize() override;

	void Update() override;

private:
	CubePrefab* m_pLeftPadle{ nullptr };
	CubePrefab* m_pRightPadle{ nullptr };

	FixedCamera* m_pCamera{ nullptr };

	SpherePrefab* m_pPongBall{ nullptr };
};
