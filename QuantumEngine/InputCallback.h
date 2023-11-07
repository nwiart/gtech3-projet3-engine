#pragma once
#include <iostream>


class InputCallback
{
public:

	virtual void OnKeyDown(unsigned short vkCode) { }

	virtual void OnMouseDown(unsigned short vkCode) { }
	virtual void OnMouseUp(unsigned short vkCode) { }
};
