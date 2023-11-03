#pragma once

typedef struct HWND__* HWND;



class Window
{
public:

	Window();

	void initialize(int width, int height, const char* title, bool fullscreen);

	void pollEvents();

	void setTitle(const char* title);

	void getClientSize(int& outWidth, int& outHeight) const;

	inline HWND getHwnd() const { return m_hwnd; }

	inline bool wantsToClose() const { return m_wantsToClose; }
	inline void setWantsToClose(bool b) { m_wantsToClose = b; }


private:

	HWND m_hwnd;
	bool m_wantsToClose;
};
