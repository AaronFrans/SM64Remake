#pragma once
class ParticleEmmiter final : public GameObject
{

public:
	ParticleEmmiter(std::wstring particlePath, ParticleEmitterSettings settings, UINT nrParticles = 300U);

	bool IsDone() { return m_IsDone; };
protected:

	void Initialize(const SceneContext& scene) override;

	void Update(const SceneContext&) override;


private:

	float m_TimeAlive{ 0 };
	bool m_IsDone{ false };

	std::wstring m_ParticlePath{};
	ParticleEmitterSettings m_Settings{};
	UINT m_NrParticles{};
};

