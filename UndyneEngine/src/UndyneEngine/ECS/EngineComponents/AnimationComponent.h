#ifndef ANIMATION_COMPONENT_H
#define ANIMATION_COMPONENT_H
#include "../../Core/Core.h"
#include "../BaseComponent.h"

namespace UndyneEngine
{
	class TextureComponent;
	class UNDYNE_API AnimationComponent final : public BaseComponent
	{
	public:
		explicit AnimationComponent(int columnCount, float framesPerSecond = 8.0f);
		void start() override;
		void update(float deltaTime) override;

	private:
		int m_ColumnCount;
		float m_SecondsPerFrame;
		float m_ElapsedFrameTime{ 0.0f };
		int m_CurrentColumn{ 0 };

		TextureComponent* m_TextureComponent{ nullptr };
		float m_FrameWidth{ 0.0f };
		float m_FrameHeight{ 0.0f };
	};
}
#endif