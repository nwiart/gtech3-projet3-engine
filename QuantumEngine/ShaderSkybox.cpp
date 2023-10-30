#include "stdafx.h"
#include "ShaderSkybox.h"

#include "resource.h"



void loadResource(std::string& out, UINT id);

ShaderSkybox::ShaderSkybox()
{
	
}

void ShaderSkybox::init()
{
	std::string source;
	loadResource(source, ID_SHADER_SKYBOX);

	this->compileShaderSource<SHADER_VS>(source.data(), source.length());
	this->compileShaderSource<SHADER_PS>(source.data(), source.length());
	this->compile();
}
