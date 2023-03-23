#pragma once
class BoneObject;

class SoftwareSkinningScene_3 final : public GameScene
{
public:

	SoftwareSkinningScene_3();
	~SoftwareSkinningScene_3() override = default;

	SoftwareSkinningScene_3(const  SoftwareSkinningScene_3& other) = delete;
	SoftwareSkinningScene_3(SoftwareSkinningScene_3&& other) noexcept = delete;
	SoftwareSkinningScene_3& operator=(const  SoftwareSkinningScene_3& other) = delete;
	SoftwareSkinningScene_3& operator=(SoftwareSkinningScene_3&& other) noexcept = delete;

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

	struct VertexSoftwareSkinned
	{
		VertexSoftwareSkinned(XMFLOAT3 position, XMFLOAT3 normal, XMFLOAT4 color, float blendWeightBone0 = 0, float blendWeightBone1 = 0) :
			transformedVertex{ position, normal, color },
			originalVertex{ position, normal, color },
			blendWeight0{ blendWeightBone0 },
			blendWeight1{ blendWeightBone1 } {}


		VertexPosNormCol transformedVertex{};
		VertexPosNormCol originalVertex{};

		float blendWeight0{};
		float blendWeight1{};
	};

	void InitializeVertices(float length);

	MeshDrawComponent* m_pMeshDrawer{};

	std::vector<VertexSoftwareSkinned> m_SkinnedVertices{};
};
