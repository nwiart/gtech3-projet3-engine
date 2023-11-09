#pragma once

#include "TextureBase.h"



class TextureCube : public TextureBase
{
public:

	TextureCube();
	TextureCube(const char* filePath);

	void load(const char* filePath);
};
