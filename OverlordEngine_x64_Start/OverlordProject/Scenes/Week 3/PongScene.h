#pragma once

class CubePrefab;
class SpherePrefab;
class  PongScene final : public GameScene
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

	PongScene();
	~PongScene() override = default;

	PongScene(const  PongScene& other) = delete;
	PongScene(PongScene&& other) noexcept = delete;
	PongScene& operator=(const  PongScene& other) = delete;
	PongScene& operator=(PongScene&& other) noexcept = delete;

protected:
	void Initialize() override;
	void PostInitialize() override;

	void Update() override;

private:
	CubePrefab* m_pLeftPadle{ nullptr };
	CubePrefab* m_pRightPadle{ nullptr };

	FixedCamera* m_pCamera{ nullptr };

	SpherePrefab* m_pPongBall{ nullptr };

	bool m_HasStarted{ nullptr };

	const float m_MaxY{ 40 };
	const float m_MinY{ 0 };

	bool m_HasHitWall{ false };

	//Positions
	const XMFLOAT3 ORIGINAL_BALL_POSS{ 0, 20, 0 };
	const XMFLOAT3 ORIGINAL_LEFT_PADDLE_POSS{ -25, 20, 0 };
	const XMFLOAT3 ORIGINAL_RIGHT_PADDLE_POSS{ 25, 20, 0 };


	void PushBall();

	void ResetScene();


};
