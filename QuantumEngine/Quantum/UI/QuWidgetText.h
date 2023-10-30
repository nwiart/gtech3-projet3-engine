#pragma once
#include "stdafx.h"
#include "QuWidget.h"


class QuWidgetText : public QuWidget
{
public:
	inline void SetAlignement(DirectX::XMFLOAT2 alignement) { m_alignement = alignement; }
	inline DirectX::XMFLOAT2 GetAlignement() { return m_alignement; }


	//inline set and get texture


private:
	//add pointer Texture class

	DirectX::XMFLOAT2 m_alignement;
};

