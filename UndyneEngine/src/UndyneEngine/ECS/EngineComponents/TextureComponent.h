#ifndef TEXTURE_COMPONENT_H
#define TEXTURE_COMPONENT_H
#include "..\..\Core\Core.h"
#include "..\BaseComponent.h"

//std
#include <string>
#include <memory>

namespace UndyneEngine
{
	class Texture2D; 
	class UNDYNE_API TextureComponent final : public BaseComponent
	{
	public: 
		TextureComponent(const std::string& file); 

		void render() const override;


	private: 
		std::shared_ptr<Texture2D> m_pTexture; 
	};
}




#endif