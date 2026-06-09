#include "DeadState.h"
#include <UndyneEngine.h>

namespace Digger
{
	void DeadState::onEnter(UndyneEngine::GameObject& owner)
	{
		if (auto* texture = owner.getComponent<UndyneEngine::TextureComponent>())
			texture->setVisible(false);
		UDE_INFO("[{}] -> Dead", owner.getName());
	}

	std::unique_ptr<UndyneEngine::State> DeadState::update(UndyneEngine::GameObject& owner, float deltaTime)
	{
		m_Timer -= deltaTime;
		if (m_Timer <= 0.0f)
			owner.markForRemoval();
		return nullptr;
	}
}
