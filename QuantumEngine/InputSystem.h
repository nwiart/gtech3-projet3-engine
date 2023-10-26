#pragma once
#include <vector>
#include "InputCallback.h"
#include "stdafx.h"

class InputSystem
{
public:
	InputSystem();
	~InputSystem();

	static inline InputSystem& Get() { static InputSystem m_inputSys; return m_inputSys; }

	void KeyDown(WPARAM wparam);
	void MouseDown(int button);

	void RegisterCallback(InputCallback* callback);
	void ExecuteCallbacks();
	std::vector <InputCallback*> m_callbacklist;

	WPARAM m_wparam;
private:
};