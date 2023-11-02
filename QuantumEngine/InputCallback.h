#pragma once
#include <iostream>


class InputCallback
{
public:

	virtual void OnKeyDown(unsigned short vkCode) { }

	virtual void OnMouseDown(int button) { }
};
