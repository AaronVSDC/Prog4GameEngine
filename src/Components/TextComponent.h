#ifndef TEXTOBJECT_H
#define TEXTOBJECT_H
#include <string>
#include <memory>

#include "BaseComponent.h"
#include "GameObject.h"
#include "Transform.h"

namespace dae
{
	class Font;
	class Texture2D;
	class TextComponent final : public BaseComponent
	{
	public:
		void Update(float deltaTime) override;
		void Render() const override;

		void setText(const std::string& text);
		void setPosition(float x, float y);

		TextComponent(const std::string& text, std::shared_ptr<Font> font);
		virtual ~TextComponent() = default;
		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;
	private:
		bool m_needsUpdate;
		std::string m_text{};
		Transform m_transform{};
		Font* m_font;
		std::unique_ptr<Texture2D> m_textTexture;
	};
}
#endif