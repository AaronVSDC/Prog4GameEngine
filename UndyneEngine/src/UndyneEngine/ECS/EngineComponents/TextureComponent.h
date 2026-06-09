#ifndef TEXTURE_COMPONENT_H
#define TEXTURE_COMPONENT_H
#include "../BaseComponent.h"
#include "../../Renderer/Renderer.h"
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

//std
#include <string>
#include <memory>
#include <optional>
 
namespace UndyneEngine
{
	class Texture2D;
	class TextureComponent final : public BaseComponent
	{
	public:
		TextureComponent(const std::string& file);
		void render() const override;

		void setTexture(const std::string& file);

		void setSourceRect(const glm::vec4& sourceRect) noexcept { m_SourceRect = sourceRect; }
		void clearSourceRect() noexcept { m_SourceRect.reset(); }
		glm::vec2 getTextureSize() const;
		void setScale(float uniformScale) noexcept { m_Scale = { uniformScale, uniformScale }; }
		void setFlip(Renderer::Flip flip) noexcept { m_Flip = flip; }
		void setRotation(float degrees) noexcept { m_Rotation = degrees; }
		void setCentered(bool centered) noexcept { m_Centered = centered; }// When centred, the sprite is drawn around the transform position instead of from top left corner
		void setVisible(bool visible) noexcept { m_Visible = visible; }
		Renderer::Flip getFlip() const noexcept { return m_Flip; }
		float getRotation() const noexcept { return m_Rotation; }

	private:
		std::shared_ptr<Texture2D> m_Texture;
		std::optional<glm::vec4> m_SourceRect;
		glm::vec2 m_Scale{ 1.0f, 1.0f };
		Renderer::Flip m_Flip{ Renderer::Flip::None };
		float m_Rotation{ 0.0f };
		bool m_Centered{ false };
		bool m_Visible{ true };
	};
}
#endif