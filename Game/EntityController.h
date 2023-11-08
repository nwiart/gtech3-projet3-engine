#pragma once

#include "QuEntityCamera.h"
#include "InputCallback.h"

class QuEntityRenderModel;


class EntityController : public QuEntityCamera
{
public:

	EntityController();

	virtual void OnSpawn(QuWorld* world) override;
	virtual void OnUpdate(const Timer& timer) override;

	void SetPosition(DirectX::XMVECTOR Position);
	DirectX::XMVECTOR GetPosition();

	DirectX::XMVECTOR m_Position;


private:

	void UpdateCamera(float dt);


	class ControllerInputCallback : public InputCallback
	{
	public:

		virtual void OnMouseDown(unsigned short btn) override;
		virtual void OnMouseUp(unsigned short btn) override;
	};

	ControllerInputCallback* m_inputCallback;

	QuEntityRenderModel* leftGunModel;
	QuEntityRenderModel* rightGunModel;

	int mouseLastStateX;
	int mouseLastStateY;
};
