#include "stdafx.h"
#include "ParticleEmmiter.h"

ParticleEmmiter::ParticleEmmiter(std::wstring particlePath, ParticleEmitterSettings settings, UINT nrParticles)
	: m_ParticlePath{ particlePath }
	, m_Settings{ settings }
	, m_NrParticles{ nrParticles }
{}

void ParticleEmmiter::Initialize(const SceneContext&)
{
	AddComponent(new ParticleEmitterComponent(m_ParticlePath, m_Settings, m_NrParticles));
}

void ParticleEmmiter::Update(const SceneContext& scene)
{
	m_TimeAlive += scene.pGameTime->GetElapsed();
	constexpr int MinTimeAlive{ 1 };

	if (MinTimeAlive < m_TimeAlive)
		m_IsDone = true;
}
