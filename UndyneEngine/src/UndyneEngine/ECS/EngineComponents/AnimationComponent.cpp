#include "AnimationComponent.h"
#include "TextureComponent.h"
#include "../GameObject.h"
#include "../../Log/Log.h"
#include <glm/vec2.hpp>

namespace UndyneEngine
{
	AnimationComponent::AnimationComponent(int columnCount, float framesPerSecond, bool loop)
		: m_ColumnCount{ columnCount }
		, m_SecondsPerFrame{ framesPerSecond > 0.0f ? 1.0f / framesPerSecond : 0.0f }
		, m_Loop{ loop }
		, m_Playing{ loop }
	{
		if (columnCount <= 0)
			UDE_CORE_ERROR("AnimationComponent: columnCount must be greater than 0");
	}

	void AnimationComponent::start()
	{
		m_TextureComponent = getOwner()->getComponent<TextureComponent>();
		if (!m_TextureComponent)
		{
			UDE_CORE_ERROR("AnimationComponent requires a TextureComponent on the same GameObject");
			return;
		}

		const glm::vec2 textureSize = m_TextureComponent->getTextureSize();
		m_FrameWidth = textureSize.x / static_cast<float>(m_ColumnCount);
		m_FrameHeight = textureSize.y;
		m_TextureComponent->setSourceRect({ 0.0f, 0.0f, m_FrameWidth, m_FrameHeight });
	}

	void AnimationComponent::update(float deltaTime)
	{
		if (not m_TextureComponent or m_ColumnCount <= 1 or m_SecondsPerFrame <= 0.0f)
			return;
		if (not m_Playing)
			return;

		m_ElapsedFrameTime += deltaTime;
		bool frameChanged = false;
		while (m_ElapsedFrameTime >= m_SecondsPerFrame)
		{
			m_ElapsedFrameTime -= m_SecondsPerFrame;
			const int nextColumn = m_CurrentColumn + 1;
			if (nextColumn >= m_ColumnCount)
			{
				if (m_Loop)
				{
					m_CurrentColumn = 0;
					frameChanged = true;
					continue;
				}
				m_CurrentColumn = m_ColumnCount - 1;
				m_Playing = false;
				m_Finished = true;
				frameChanged = true;
				break;
			}
			m_CurrentColumn = nextColumn;
			frameChanged = true;
		}

		if (frameChanged)
			m_TextureComponent->setSourceRect(
				{ m_CurrentColumn * m_FrameWidth, 0.0f, m_FrameWidth, m_FrameHeight });
	}

	void AnimationComponent::play()
	{
		m_CurrentColumn = 0;
		m_ElapsedFrameTime = 0.0f;
		m_Playing = true;
		m_Finished = false;
		if (m_TextureComponent)
			m_TextureComponent->setSourceRect({ 0.0f, 0.0f, m_FrameWidth, m_FrameHeight });
	}
}