#pragma once
#include "stdafx.h"
#include "QuWidget.h"


class QuWidgetButton : public QuWidget
{
public:
	typedef void (QuWidget::* OnButtonClickCallback)(int, int);

	inline OnButtonClickCallback GetCallBack() { return m_callback; }
	inline void SetCallBack(OnButtonClickCallback callback) { m_callback = callback; }

private:
	OnButtonClickCallback m_callback;

};

