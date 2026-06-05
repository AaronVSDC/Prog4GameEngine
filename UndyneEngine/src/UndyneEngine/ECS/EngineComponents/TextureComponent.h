#ifndef TEXTURE_COMPONENT_H
#define TEXTURE_COMPONENT_H
#include "../../Core/Core.h"
#include "../BaseComponent.h"
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

//std
#include <string>
#include <memory>
#include <optional>
 
namespace UndyneEngine
{
	class Texture2D;
	class UNDYNE_API TextureComponent final : public BaseComponent
	{
	public:
		TextureComponent(const std::string& file);
		void render() const override;

		void setSourceRect(const glm::vec4& sourceRect) noexcept { m_SourceRect = sourceRect; }
		void clearSourceRect() noexcept { m_SourceRect.reset(); }
		glm::vec2 getTextureSize() const;
		void setScale(float uniformScale) noexcept { m_Scale = { uniformScale, uniformScale }; }

	private:
		std::shared_ptr<Texture2D> m_Texture;
		std::optional<glm::vec4> m_SourceRect;
		glm::vec2 m_Scale{ 1.0f, 1.0f };
	}; 
}
#endif