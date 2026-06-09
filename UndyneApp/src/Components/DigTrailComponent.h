#ifndef DIG_TRAIL_COMPONENT_H
#define DIG_TRAIL_COMPONENT_H
#include <UndyneEngine.h>
#include <glm/glm.hpp>
#include "UndyneEngine/Utils/Texture2D.h"

//std
#include <memory>
#include <string>
#include <vector>


namespace Digger
{
	class LevelGridComponent;

	class DigTrailComponent final : public UndyneEngine::BaseComponent
	{
	public:
		void start() override;
		void update(float elapsedSec) override;
		void render() const override;

		void addFollower(const std::string& name);
		void resetStamp() noexcept;

	private:
		struct Follower
		{
			std::string name;
			glm::vec2 lastStampPos{ 0.0f, 0.0f };
			bool hasStamped{ false };
		};

		void drawBrush(glm::vec2 worldPos) const;
		void stampLine(glm::vec2 from, glm::vec2 to) const;
		void updateFollower(Follower& follower);

		float m_BrushScale = 0.95f;
		float m_StampSpacing = .25f;

		LevelGridComponent* m_Grid{ nullptr };
		std::unique_ptr<UndyneEngine::Texture2D> m_Canvas;
		std::shared_ptr<UndyneEngine::Texture2D> m_Hole;
		std::vector<Follower> m_Followers;
	};
}

#endif
