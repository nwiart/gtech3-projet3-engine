#pragma once
#include "QuWidget.h"

class VertexBuffer;



class QuWidgetText : public QuWidget
{
public:
	QuWidgetText();
	~QuWidgetText();

	inline void SetAlignement(DirectX::XMFLOAT2 alignement) { m_alignement = alignement; }
	inline DirectX::XMFLOAT2 GetAlignement() { return m_alignement; }

	void SetText(std::string text);

	inline int GetNumOfVertices() { return numbOfVertices; }

	inline VertexBuffer* GetVertexBuffer() { return m_vBuffer; }


private:
	DirectX::XMFLOAT2 m_alignement;

	std::string m_text;

	VertexBuffer* m_vBuffer;

	int numbOfVertices;



};

