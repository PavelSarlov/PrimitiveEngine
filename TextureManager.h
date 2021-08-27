#pragma once
#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include "ResourceManager.h"
#include "Texture.h"

class TextureManager : public ResourceManager
{
public:
	TextureManager();
	~TextureManager();

public:
	TexturePtr createTextureFromFile(const wchar_t *file_path);
	TexturePtr createTexture(const Rect &size, Texture::Type type);

protected:
	virtual Resource *createResourceFromFileConcrete(const wchar_t *file_path);
};

#endif // !TEXTUREMANAGER_H