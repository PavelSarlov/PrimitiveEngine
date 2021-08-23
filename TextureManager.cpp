#include "TextureManager.h"
#include "Texture.h"

#include <iostream>

TextureManager::TextureManager() : ResourceManager()
{}

TextureManager::~TextureManager()
{}

TexturePtr TextureManager::createTextureFromFile(const wchar_t *file_path)
{
	return std::static_pointer_cast<Texture>(createResourceFromFile(file_path));
}

Resource *TextureManager::createResourceFromFileConcrete(const wchar_t *file_path)
{
	try
	{
		return new Texture(file_path);
	}
	catch(std::exception e)
	{
		std::cout << e.what() << std::endl;
	}

	return nullptr;
}
