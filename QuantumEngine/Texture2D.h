#pragma once

#include "TextureBase.h"


class Texture2D : public TextureBase
{
public:

	Texture2D();
	Texture2D(const char* filePath);

	void load(const char* filePath);
};
