#pragma once
#include "stdafx.h"
#include "QuWidget.h"
#include "Texture2D.h"

class QuWidgetText;


class QuWidgetButton : public QuWidget
{
public:
	QuWidgetButton();

	typedef void (QuWidget::*OnButtonClickCallback)(int, int);

	inline OnButtonClickCallback GetCallBack() { return m_callback; }
	inline void SetCallBack(OnButtonClickCallback callback) { m_callback = callback; }

	inline void setTexture(Texture2D* texture) { m_texture = texture; }
	inline Texture2D* GetTexture() { return m_texture; }

	void createText(std::string text);

private:
	OnButtonClickCallback m_callback;

	Texture2D* m_texture;

	QuWidgetText* m_text;

};

