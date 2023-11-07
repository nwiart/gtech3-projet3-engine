#pragma once

#include <vector>

#include "InputCallback.h"


class InputSystem
{
public:
	inline bool isMouseDown(int vkCode) const { return m_keyState[vkCode]; }

	static inline InputSystem& Get() { static InputSystem m_inputSys; return m_inputSys; }

	bool mouseUp;

	InputSystem();
	~InputSystem();

	void FireKeyDown(unsigned short vkCode);
	void FireKeyUp(unsigned short vkCode);

	void FireMouseMove(int newX, int newY);
	void FireMouseDown(int button);
	void FireMouseUp(int button);

	inline bool isKeyDown(int vkCode) const { return m_keyState[vkCode]; }

	inline int getMouseX() const { return m_cursorX; }
	inline int getMouseY() const { return m_cursorY; }

	inline int getMouseDX() const { return m_cursorX - m_lastCursorX; }
	inline int getMouseDY() const { return m_cursorY - m_lastCursorY; }

	void RegisterCallback(InputCallback* callback);

	int lMouseEvent;
	int rMouseEvent;

private:
	bool m_mouseState[3];

	std::vector<InputCallback*> m_callbacklist;

	int m_cursorX;
	int m_cursorY;
	int m_lastCursorX;
	int m_lastCursorY;

	bool m_keyState[256];
};
