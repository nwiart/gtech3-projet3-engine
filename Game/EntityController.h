#pragma once
#include "QuEntityCamera.h"

class EntityController : public QuEntityCamera
{
public:

	void SetCamera();
	EntityController();

	void OnUpdate(Timer timer) override;

private:

	int mouseLastStateX;
	int mouseLastStateY;
};

