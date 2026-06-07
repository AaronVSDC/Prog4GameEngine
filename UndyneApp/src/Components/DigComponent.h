#ifndef DIG_COMPONENT_H
#define DIG_COMPONENT_H
#include <UndyneEngine.h>
#include <glm/glm.hpp>

namespace Digger
{
	class DigField;

	// Lives on the player. As the player moves it carves the tunnel: it marks the
	// cells it passes through as dug (logical truth) and stamps a round hole onto the
	// DigField canvas every few pixels (cosmetic) so the dug ribbon stays continuous.
	class DigComponent final : public UndyneEngine::BaseComponent
	{
	public:
		void setField(DigField* field) noexcept { m_Field = field; }

		void update(float elapsedSec) override;

	private:
		DigField* m_Field{ nullptr };
		glm::vec2 m_PreviousCenter{};
		float m_NativeDistanceAccumulator{ 0.0f };
		bool m_Initialized{ false };

		static constexpr float k_StampSpacingNative{ 4.0f };
	};
}

#endif
