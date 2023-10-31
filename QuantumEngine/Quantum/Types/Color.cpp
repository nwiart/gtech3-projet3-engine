#include "Color.h"

using namespace Quantum;



const Color Color::BLACK = 0x000000ff;
const Color Color::BLUE  = 0x0000FFff;
const Color Color::GREEN = 0x00FF00ff;
const Color Color::RED   = 0xFF0000ff;
const Color Color::WHITE = 0xFFFFFFff;



void Color::fromFloat4(const float c[4])
{
	m_bytes[3] = (unsigned char) (c[0] * 255.0F);
	m_bytes[2] = (unsigned char) (c[1] * 255.0F);
	m_bytes[1] = (unsigned char) (c[2] * 255.0F);
	m_bytes[0] = (unsigned char) (c[3] * 255.0F);
}

void Color::fromFloat4(float r, float g, float b, float a)
{
	m_bytes[3] = (unsigned char) (r * 255.0F);
	m_bytes[2] = (unsigned char) (g * 255.0F);
	m_bytes[1] = (unsigned char) (b * 255.0F);
	m_bytes[0] = (unsigned char) (a * 255.0F);
}

void Color::toFloat4(float out[4]) const
{
	out[0] = m_bytes[3] / 255.0F;
	out[1] = m_bytes[2] / 255.0F;
	out[2] = m_bytes[1] / 255.0F;
	out[3] = m_bytes[0] / 255.0F;
}
