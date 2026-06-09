#ifndef GOLD_BAG_COMPONENT_H
#define GOLD_BAG_COMPONENT_H
#include <UndyneEngine.h>
#include <glm/vec2.hpp>

namespace Digger
{
	class LevelGridComponent;

	class GoldBagComponent final : public UndyneEngine::BaseComponent
	{
	public:
		explicit GoldBagComponent(glm::ivec2 cell) noexcept;

		void start() override;
		void update(float deltaTime) override;

		bool isRestingBag() const noexcept { return m_State == State::Resting; }
		bool isGold() const noexcept { return m_State == State::Gold; }
		void pushTo(glm::ivec2 cell);

	private:
		enum class State { Resting, Wobbling, Falling, Gold };

		bool isSupported() const;
		bool playerOccupies(glm::ivec2 cell) const;
		void enterWobbling();
		void enterFalling();
		void updateFalling(float deltaTime);
		void land();
		void becomeGold();
		void updateGold();
		void squashIfPlayerHit();
		void fitTextureToCell();
		void setCellPosition();

		State m_State{ State::Resting };
		glm::ivec2 m_Cell{ 0, 0 };
		int m_RowsFallen{ 0 };
		float m_WobbleDuration{ 1.0f };
		float m_WobbleTimer{ 0.0f };
		float m_FallY{ 0.0f };
		float m_FallSpeed{ 0.0f };

		UndyneEngine::TextureComponent* m_Texture{ nullptr };
		LevelGridComponent* m_Grid{ nullptr };
		UndyneEngine::GameObject* m_Player{ nullptr };
	};
}
#endif
