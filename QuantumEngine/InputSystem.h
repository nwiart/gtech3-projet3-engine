#pragma once
#include <vector>
#include "InputCallback.h"

class InputSystem
{
public:
	InputSystem();
	~InputSystem();

	void FireKeyDown(WPARAM wparam);
	void FireKeyUp(WPARAM wparam);

	inline bool isKeyDown(int vkCode) const { return m_keyState[vkCode]; }
	inline bool isMouseDown(int vkCode) const { return m_mouseState[vkCode]; }

	static inline InputSystem& Get() { static InputSystem m_inputSys; return m_inputSys; }

	void MouseDown(WPARAM wparam);
	void MouseUp(WPARAM wparam);

	void RegisterCallback(InputCallback* callback);
	void ExecuteCallbacks(WPARAM wparam);
	std::vector <InputCallback*> m_callbacklist;

	WPARAM m_wparam;

private:
	bool m_mouseState[3];
	bool m_keyState[256];
};