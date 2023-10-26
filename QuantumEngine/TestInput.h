#pragma once
#include "InputCallback.h"

class Test : public InputCallback
{
public:
	Test();
	virtual ~Test();
	void RegisterInput() override;
};