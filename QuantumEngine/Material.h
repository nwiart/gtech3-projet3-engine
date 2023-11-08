#pragma once

#include "Quantum/Types/Color.h"

#define MATERIAL_MAX_TEXTURES 8

class TextureBase;



class Material
{
public:

	Material();

	inline TextureBase* getTexture(int index) const { return m_textures[index]; }
	inline void setTexture(int index, TextureBase* t) { m_textures[index] = t; }

	inline Quantum::Color getBaseColor() const { return m_baseColor; }
	inline Quantum::Color getSpecularColor() const { return m_specularColor; }
	inline Quantum::Color getEmissiveColor() const { return m_emissiveColor; }
	inline float getSpecularPower() const { return m_specularPower; }

	inline void setBaseColor(Quantum::Color c) { m_baseColor = c; }
	inline void setSpecularColor(Quantum::Color c) { m_specularColor = c; }
	inline void setEmissiveColor(Quantum::Color c) { m_emissiveColor = c; }
	inline void setSpecularPower(float f) { m_specularPower = f; }

	inline bool isTransparent() const { return m_transparent; }
	inline void setTransparent(bool b) { m_transparent = b; }



private:

		/// Available texture slots.
	TextureBase* m_textures[MATERIAL_MAX_TEXTURES];

		/// Blinn-Phong material values.
	Quantum::Color m_baseColor;
	Quantum::Color m_specularColor;
	Quantum::Color m_emissiveColor;
	float m_specularPower;
	float m_specularIntensity;

		/// Is this material capable of transparency (active blending, slowest rendering).
	bool m_transparent;
};
