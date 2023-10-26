#include "QuEntity.h"

QuEntity::QuEntity()
{
	//init to null
}

QuEntity::~QuEntity()
{
}

void QuEntity::AttachToParent(QuEntity* Parent)
{

	if (m_Parent != NULL)
	{
		DetachFromParent();
	}
	m_Parent = Parent;
	this->m_Sibling = m_Parent->m_FirstChild;
	Parent->m_FirstChild = this;

}

void QuEntity::DetachFromParent()
{
	QuEntity* PreviousChild = m_Parent->m_FirstChild;
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


