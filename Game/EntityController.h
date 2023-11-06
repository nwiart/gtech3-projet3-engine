#pragma once

#include "QuEntityCamera.h"
#include "QuEntityRenderModel.h"

class EntityController : public QuEntityCamera
{
public:
	EntityController();

private:

	QuEntityRenderModel* leftGunModel = new QuEntityRenderModel;
	QuEntityRenderModel* rightGunModel = new QuEntityRenderModel;
	virtual void OnUpdate(const Timer& timer) override;

	void OnSpawn(QuWorld* world);

	void UpdateCamera(float dt);

	int mouseLastStateX;
	int mouseLastStateY;
};
