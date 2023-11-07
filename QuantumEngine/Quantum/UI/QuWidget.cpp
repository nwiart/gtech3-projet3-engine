#include "QuWidget.h"

QuWidget::QuWidget()
{
	m_position = DirectX::XMFLOAT2(0, 0);
	m_size = DirectX::XMFLOAT2(0, 0);
}

QuWidget::~QuWidget()
{
}

void QuWidget::attachChild(QuWidget* child)
{
	// Undo previous attachments.
	if (child->m_Parent) {
		child->DetachFromParent();
	}

	// Prepend child.
	child->m_Parent = this;
	child->m_Sibling = this->m_FirstChild;
	this->m_FirstChild = child;
}

void QuWidget::AttachToParent(QuWidget* Parent)
{
	if (m_Parent != NULL)
	{
		DetachFromParent();
	}

	m_Parent = Parent;
	this->m_Sibling = m_Parent->m_FirstChild;
	Parent->m_FirstChild = this;
}

void QuWidget::DetachFromParent()
{
	QuWidget* PreviousChild = m_Parent->m_FirstChild;
	if (m_Parent->m_FirstChild == this)
	{
		m_Parent->m_FirstChild = this->m_Sibling;
		this->m_Sibling = NULL;

	}
	else
	{
		while (this != PreviousChild->m_Sibling)
		{
			PreviousChild = PreviousChild->m_Sibling;
		}
		PreviousChild = this->m_Sibling;
		this->m_Sibling = NULL;
	}

	this->m_Parent = NULL;
}
