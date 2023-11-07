#include "Material.h"

#include <string.h>

using namespace Quantum;



Material::Material()
	: m_baseColor(Color::WHITE)
	, m_specularColor(Color::WHITE)
	, m_emissiveColor(Color::BLACK)
	, m_specularPower(10.0F)
	, m_specularIntensity(0.1F)
	, m_transparent(false)
{
	memset(m_textures, 0, MATERIAL_MAX_TEXTURES * sizeof(TextureBase*));
}
