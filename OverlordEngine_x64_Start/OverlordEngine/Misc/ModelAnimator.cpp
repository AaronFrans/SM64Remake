#include "stdafx.h"
#include "ModelAnimator.h"

ModelAnimator::ModelAnimator(MeshFilter* pMeshFilter) :
	m_pMeshFilter{ pMeshFilter }
{
	SetAnimation(0);
}

void ModelAnimator::Update(const SceneContext& sceneContext)
{
	//We only update the transforms if the animation is running and the clip is set
	if (m_IsPlaying && m_ClipSet)
	{
		//1. 
		//Calculate the passedTicks (see the lab document)
		//auto passedTicks = ...
		auto passedTicks = sceneContext.pGameTime->GetElapsed() * m_CurrentClip.ticksPerSecond * m_AnimationSpeed;
		//Make sure that the passedTicks stay between the m_CurrentClip.Duration bounds (fmod)
		if (passedTicks > m_CurrentClip.duration)
			passedTicks = fmod(passedTicks, m_CurrentClip.duration);
		//2. 
		//IF m_Reversed is true
		//	Subtract passedTicks from m_TickCount
		//	If m_TickCount is smaller than zero, add m_CurrentClip.Duration to m_TickCount
		if (m_Reversed)
		{
			m_TickCount -= passedTicks;
			if (m_TickCount < 0)
				m_TickCount += m_CurrentClip.duration;
		}
		//ELSE
		//	Add passedTicks to m_TickCount
		//	if m_TickCount is bigger than the clip duration, subtract the duration from m_TickCount
		else
		{
			m_TickCount += passedTicks;
			if (m_TickCount > m_CurrentClip.duration)
				m_TickCount -= m_CurrentClip.duration;
		}


		//3.
		//Find the enclosing keys
		AnimationKey keyA{}, keyB{};
		//Iterate all the keys of the clip and find the following keys:
		//keyA > Closest Key with Tick before/smaller than m_TickCount
		//keyB > Closest Key with Tick after/bigger than m_TickCount
		for (int i = 0; i < m_CurrentClip.keys.size(); ++i)
		{
			const auto& currentKey = m_CurrentClip.keys[i];
			if (currentKey.tick > m_TickCount)
			{
				keyB = currentKey;
				keyA = m_CurrentClip.keys[i - 1];
				break;
			}
		}


		//4.
		//Interpolate between keys
		//Figure out the BlendFactor (See lab document)
		float range = keyB.tick - keyA.tick;
		float distanceFromA = m_TickCount - keyA.tick;
		const auto blendFactor = distanceFromA / range;
		//Clear the m_Transforms vector
		//FOR every boneTransform in a key (So for every bone)
		//	Retrieve the transform from keyA (transformA)
		//	auto transformA = ...
		// 	Retrieve the transform from keyB (transformB)
		//	auto transformB = ...
		//	Decompose both transforms
		//	Lerp between all the transformations (Position, Scale, Rotation)
		//	Compose a transformation matrix with the lerp-results
		//	Add the resulting matrix to the m_Transforms vector

		for (int i = 0; i < m_pMeshFilter->m_BoneCount; ++i)
		{
			const auto& transformA = keyA.boneTransforms[i];
			const auto& transformB = keyB.boneTransforms[i];

			XMVECTOR scaleA{};
			XMVECTOR rotationA{};
			XMVECTOR translationA{};
			XMMatrixDecompose(&scaleA, &rotationA, &translationA, XMLoadFloat4x4(&transformA));

			XMVECTOR scaleB{};
			XMVECTOR rotationB{};
			XMVECTOR translationB{};
			XMMatrixDecompose(&scaleB, &rotationB, &translationB, XMLoadFloat4x4(&transformB));

			const auto scale = XMVectorLerp(scaleA, scaleB, blendFactor);
			const auto rotation = XMQuaternionSlerp(rotationA, rotationB, blendFactor);
			const auto translation = XMVectorLerp(translationA, translationB, blendFactor);

			const XMMATRIX newTransform{ XMMatrixScalingFromVector(scale) * XMMatrixRotationQuaternion(rotation)
				* XMMatrixTranslationFromVector(translation) };

			XMStoreFloat4x4(&m_Transforms[i], newTransform);
		}
	}
}

void ModelAnimator::SetAnimation(const std::wstring& clipName)
{
	//Set m_ClipSet to false
	m_ClipSet = false;
	//Iterate the m_AnimationClips vector and search for an AnimationClip with the given name (clipName)
	for (auto& clip : m_pMeshFilter->GetAnimationClips())
	{
		//If found,
		//Call SetAnimation(Animation Clip) with the found clip
		if (clip.name == clipName)
		{
			SetAnimation(clip);
			return;
		}
	}

	//Else
	//Call Reset
	//Log a warning with an appropriate message
	Logger::LogWarning(L"No animation with name {}", clipName);
	Reset();
}

void ModelAnimator::SetAnimation(UINT clipNumber)
{
	//Set m_ClipSet to false
	m_ClipSet = false;
	//Check if clipNumber is smaller than the actual m_AnimationClips vector size
	const auto& clips = m_pMeshFilter->GetAnimationClips();
	if (clipNumber >= clips.size() && clipNumber >= 0)
	{
		//	Call Reset
		//	Log a warning with an appropriate message
		//	return
		Logger::LogWarning(L"There are only {} clips", clips.size() - 1);
		Reset();
		return;
	}
	//If not,

	SetAnimation(clips[clipNumber]);
	//else
		//	Retrieve the AnimationClip from the m_AnimationClips vector based on the given clipNumber
		//	Call SetAnimation(AnimationClip clip)
}

void ModelAnimator::SetAnimation(const AnimationClip& clip)
{
	//Set m_ClipSet to true
	m_ClipSet = true;
	//Set m_CurrentClip
	m_CurrentClip = clip;
	//Call Reset(false)
	Reset(false);
}

void ModelAnimator::Reset(bool pause)
{
	//If pause is true, set m_IsPlaying to false
	if (pause)
		m_IsPlaying = false;
	//Set m_TickCount to zero
	m_TickCount = 0;
	//Set m_AnimationSpeed to 1.0f
	m_AnimationSpeed = 1.0f;

	//If m_ClipSet is true
	//	Retrieve the BoneTransform from the first Key from the current clip (m_CurrentClip)
	//	Refill the m_Transforms vector with the new BoneTransforms (have a look at vector::assign)
	if (m_ClipSet)
	{
		const auto& transfroms = m_CurrentClip.keys[0].boneTransforms;
		m_Transforms.assign(begin(transfroms), end(transfroms));

	}
	//Else
	//	Create an IdentityMatrix 
	//	Refill the m_Transforms vector with this IdenityMatrix (Amount = BoneCount) (have a look at vector::assign)
	XMFLOAT4X4 identityMatrix;
	XMStoreFloat4x4(&identityMatrix, XMMatrixIdentity());
	m_Transforms.assign(m_pMeshFilter->GetBoneCount(), identityMatrix);
}
