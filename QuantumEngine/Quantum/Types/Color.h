#pragma once



namespace Quantum
{



class Color
{
public:

	static const Color BLACK;
	static const Color BLUE;
	static const Color GREEN;
	static const Color RED;
	static const Color WHITE;

public:

	inline Color()
		: m_rgba(0x000000ff)
	{

	}

	inline Color(unsigned int rgba)
		: m_rgba(rgba)
	{

	}

	inline Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255)
	{
		m_bytes[3] = r;
		m_bytes[2] = g;
		m_bytes[1] = b;
		m_bytes[0] = a;
	}


	void fromFloat4(const float c[4]);
	void fromFloat4(float r, float g, float b, float a);

	void toFloat4(float out[4]) const;



private:

	union
	{
		unsigned int m_rgba;
		unsigned char m_bytes[4];
	};
};



} // namespace Quantum
