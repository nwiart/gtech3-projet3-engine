#pragma once

#include "Texture2D.h"
#include "TextureCube.h"


class ResourceLibrary
{
public:

	ResourceLibrary();

	static ResourceLibrary& Get() { static ResourceLibrary i; return i; }


	Texture2D smoke;
	TextureCube milkyway;
	Texture2D metalic;
	Texture2D asteroid;
	Texture2D neptune;
	Texture2D mercure;
	Texture2D mars;
	Texture2D alien;

	Texture2D button;
	Texture2D backgroundMainMenu;
	Texture2D crosshair;

	TextureCube skybox;

};

