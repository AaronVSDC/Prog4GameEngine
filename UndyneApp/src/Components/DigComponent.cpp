#include "DigComponent.h"
#include "DigField.h"

namespace Digger
{
	void DigComponent::update(float)
	{
		if (!m_Field) return;

		// The player's transform stores its centre, which already rides the cell lane.
		const glm::vec3 position = getOwner()->getTransform().getLocalPosition();
		const glm::vec2 center{ position.x, position.y };

		if (!m_Initialized)
		{
			m_PreviousCenter = center;
			m_Initialized = true;
			m_Field->markDug(m_Field->worldToCell(center));
			m_Field->stampHole(m_Field->worldToNative(center));
			return;
		}

		const glm::vec2 delta = center - m_PreviousCenter;
		const float distance = glm::length(delta);
		if (distance > 0.0001f)
		{
			m_Field->markDug(m_Field->worldToCell(center));

			// Lay holes evenly along the distance travelled so the ribbon stays
			// continuous however far the player moved this frame.
			const glm::vec2 nativeCenter = m_Field->worldToNative(center);
			m_NativeDistanceAccumulator += distance / m_Field->pixelScale();
			while (m_NativeDistanceAccumulator >= k_StampSpacingNative)
			{
				m_Field->stampHole(nativeCenter);
				m_NativeDistanceAccumulator -= k_StampSpacingNative;
			}
		}

		m_PreviousCenter = center;
	}
}
