#ifndef DIG_TRAIL_COMPONENT_H
#define DIG_TRAIL_COMPONENT_H
#include <UndyneEngine.h>
#include <glm/glm.hpp>
#include "UndyneEngine/Utils/Texture2D.h"

//std
#include <memory>


namespace Digger
{
	class LevelGridComponent;

	class DigTrailComponent final : public UndyneEngine::BaseComponent
	{
	public:
		void start() override;
		void update(float elapsedSec) override;
		void render() const override;

	private:
		void drawBrush(glm::vec2 worldPos) const;
		void stampLine(glm::vec2 from, glm::vec2 to) const;
		float m_BrushScale = 0.95f;
		float m_StampSpacing = .25f;

		LevelGridComponent* m_Grid{ nullptr };
		UndyneEngine::GameObject* m_Player{ nullptr };
		std::unique_ptr<UndyneEngine::Texture2D> m_Canvas;
		std::shared_ptr<UndyneEngine::Texture2D> m_Hole;
		glm::vec2 m_LastStampPos{ 0.0f, 0.0f };
		bool m_HasStamped{ false };
	};
}

#endif
