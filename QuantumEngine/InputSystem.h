#pragma once
#include <vector>
#include "InputCallback.h"

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
private:
	std::vector <InputCallback*> m_callbacks;
};