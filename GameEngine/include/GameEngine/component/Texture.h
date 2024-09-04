#ifndef TEXTURE_H
#define TEXTURE_H

#include "GameEngine/ECS/Component.h"

namespace Gen
{
	class Texture : public Component
	{
	public:
		void setTexture(std::string&& name)
		{
			texture = AssetManager::Texture::getTexture(name);
			textureSet = true;
		}

		uint32_t getTextureObject()
		{
			assertNoAbort(textureSet, "texture not set");
			return texture;
		}

	private:

		uint32_t texture;
		bool textureSet = false;
	};
}

#endif 
