#ifndef SPRITE_COMPONENT_H
#define SPRITE_COMPONENT_H 
#include "../../Core/Core.h"
#include "../BaseComponent.h"
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

//std
#include <string>
#include <memory>


namespace UndyneEngine
{

	class Texture2D;
	class UNDYNE_API SpriteComponent final : public BaseComponent
	{
	public:
		SpriteComponent(const std::string& file,
			const glm::vec4& sourceRect,
			const glm::vec2& destinationSize);
		void render() const override;

		void setSourceRect(const glm::vec4& sourceRect) noexcept { m_SourceRect = sourceRect; }

	private:

		std::shared_ptr<Texture2D> m_Texture;
		glm::vec4 m_SourceRect;
		glm::vec2 m_DestinationSize;


	};


}

#endif