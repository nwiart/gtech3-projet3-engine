#pragma once
#include "InputCallback.h"

class EntityController : public InputCallback
{
public:

	void OnKeyDown(WPARAM wparam) override;
	void SetCamera(Graphics* graphics);
	EntityController();

private:

	int mouseLastStateX;
	int mouseLastStateY;

	Graphics* m_graphics;
	TestConstantBuffer* m_testConstantBuffer;
};

