#pragma once



class Graphics
{
public:

	static inline Graphics* getInstance() { Graphics g_graphics; return &g_graphics; }
};
