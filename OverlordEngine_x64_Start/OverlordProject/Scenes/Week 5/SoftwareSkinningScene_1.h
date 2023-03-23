#pragma once

class BoneObject;

class SoftwareSkinningScene_1 final : public GameScene
{
public:

	SoftwareSkinningScene_1();
	~SoftwareSkinningScene_1() override = default;

	SoftwareSkinningScene_1(const  SoftwareSkinningScene_1& other) = delete;
	SoftwareSkinningScene_1(SoftwareSkinningScene_1&& other) noexcept = delete;
	SoftwareSkinningScene_1& operator=(const  SoftwareSkinningScene_1& other) = delete;
	SoftwareSkinningScene_1& operator=(SoftwareSkinningScene_1&& other) noexcept = delete;

protected:
	void Initialize() override;

	void Update() override;

	void OnGUI() override;

private:
	BoneObject* m_pBone0{}, * m_pBone1{};

	float m_BoneRotation{};
	int m_RotationSign{ 1 };

	XMFLOAT3 m_Bone0Rotation{ 0, 0, 0 };
	XMFLOAT3 m_Bone1Rotation{ 0, 0, 0 };


	bool m_AutoRotate{};
};
