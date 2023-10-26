#pragma once

class InputSystem
{
public:
	InputSystem();
	~InputSystem();

	static inline InputSystem& Get() { static InputSystem m_inputSys; return m_inputSys; }

	void KeyDown(WPARAM wparam);
	void MouseDown(int button);

private:

};