#ifndef TEXTURE_COMPONENT_H
#define TEXTURE_COMPONENT_H
#include <string>
#include <glm/vec2.hpp>

#include "BaseComponent.h"

namespace dae
{
	class Texture2D;
	class TextureComponent final : public BaseComponent
	{
	public:
		TextureComponent(const std::string& fileName);
		TextureComponent(const TextureComponent&) = delete;
		TextureComponent& operator=(const TextureComponent&) = delete;
		TextureComponent(TextureComponent&&) = delete;
		TextureComponent& operator=(TextureComponent&&) = delete;

		void Render() const override; 

		glm::vec2 getSize() const; 

	private:

		Texture2D* m_pTexture;
		std::string m_FileName;

	};
}

#endif