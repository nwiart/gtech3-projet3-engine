#pragma once
#include <vector>
#include <Windows.h>
#include "InputCallback.h"

class InputSystem
{
public:
	InputSystem();
	~InputSystem();

	int cursorX;
	int cursorY;

	void FireKeyDown(WPARAM wparam);
	void FireKeyUp(WPARAM wparam);

	inline bool isKeyDown(int vkCode) const { return m_keyState[vkCode]; }

	static inline InputSystem& Get() { static InputSystem m_inputSys; return m_inputSys; }

	void MouseDown(int button);

	void RegisterCallback(InputCallback* callback);
	void ExecuteCallbacks(WPARAM wparam);
	std::vector <InputCallback*> m_callbacklist;

	WPARAM m_wparam;

private:

	bool m_keyState[256];
};