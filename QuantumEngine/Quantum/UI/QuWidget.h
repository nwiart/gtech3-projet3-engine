#pragma once
#include "stdafx.h"


class QuWidget
{
friend class Game;
public:
	QuWidget();


	void AttachToParent(QuWidget* Parent);
	void DetachFromParent();

	inline void SetPosition(DirectX::XMFLOAT2 position) { m_position = position; }
	inline DirectX::XMFLOAT2 GetPosition() { return m_position; }

	inline void SetSize(DirectX::XMFLOAT2 size) { m_size = size; }
	inline DirectX::XMFLOAT2 GetSize() { return m_size; }

protected :
	DirectX::XMFLOAT2 m_position;
	DirectX::XMFLOAT2 m_size;


	QuWidget* m_Parent;
	QuWidget* m_FirstChild;
	QuWidget* m_Sibling;




};

