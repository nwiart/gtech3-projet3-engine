#pragma once

#include "QuEntity.h"

class TextureCube;



class QuEntityRenderSkybox : public QuEntity
{
public:

	QuEntityRenderSkybox();

	inline TextureCube* getTexture() const { return m_texture; }
	inline void setTexture(TextureCube* tex) { m_texture = tex; }

private:

	virtual void ExecuteProcedure() override;


private:

	TextureCube* m_texture;	
};
