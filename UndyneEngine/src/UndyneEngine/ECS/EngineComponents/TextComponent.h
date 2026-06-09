#ifndef TEXT_COMPONENT_H
#define TEXT_COMPONENT_H
#include "../BaseComponent.h"
#include "../../Renderer/Renderer.h"

#include <string>
#include <memory>

namespace UndyneEngine
{
	class Font;
	class Texture2D;

	class TextComponent final : public BaseComponent
	{
	public:
		TextComponent(const std::string& text, std::shared_ptr<Font> font, Renderer::Color color = {});

		void update(float deltaTime) override;
		void render() const override;

		void setText(const std::string& text);
		void setColor(Renderer::Color color);

	private:
		std::string m_Text;
		std::shared_ptr<Font> m_Font;
		Renderer::Color m_Color;
		bool m_NeedsRebuild{ true };
		std::unique_ptr<Texture2D> m_Texture;
	};
}
#endif
