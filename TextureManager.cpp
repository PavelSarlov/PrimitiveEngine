#include "TextureManager.h"

#include <iostream>

TextureManager::TextureManager() : ResourceManager()
{}

TextureManager::~TextureManager()
{}

TexturePtr TextureManager::createTextureFromFile(const wchar_t *file_path)
{
	return std::static_pointer_cast<Texture>(createResourceFromFile(file_path));
}

TexturePtr TextureManager::createTexture(const Rect &size, Texture::Type type)
{
	try
	{
		return TexturePtr(new Texture(size, type));
	}
	catch(std::exception e)
	{
		std::cout << e.what() << std::endl;
	}

	return TexturePtr(nullptr);
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
