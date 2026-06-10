#ifndef CRUSHED_STATE_H
#define CRUSHED_STATE_H
#include <UndyneEngine/StateMachine/State.h>
#include <glm/vec2.hpp>

//std
#include <memory>

namespace UndyneEngine { class GameObject; }

namespace Digger
{
	class LevelGridComponent;

	class CrushedState final : public UndyneEngine::State
	{
	public:
		void onEnter(UndyneEngine::GameObject& owner) override;
		std::unique_ptr<UndyneEngine::State> update(UndyneEngine::GameObject& owner, float deltaTime) override;

	private:
		LevelGridComponent* m_Grid{ nullptr };
		glm::ivec2 m_Cell{ 0, 0 };
		float m_FallY{ 0.0f };
		float m_FallSpeed{ 0.0f };
	};
}
#endif
