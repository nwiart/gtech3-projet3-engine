#include "QuEntity.h"

QuEntity::QuEntity()
	: m_Parent(0), m_FirstChild(0), m_Sibling(0)
	, m_dirtyWorldMatrix(true)
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

void QuEntity::setPosition(DirectX::XMFLOAT3 positon) 
{
	m_Transform.setPosition(positon);
	m_dirtyWorldMatrix = true;
}

void QuEntity::setRotation(DirectX::XMFLOAT4 quat)
{
	m_Transform.setRotation(quat);
	m_dirtyWorldMatrix = true;
}

void QuEntity::setScale(DirectX::XMFLOAT3 scale)
{
	m_Transform.setScale(scale);
	m_dirtyWorldMatrix = true;
}

void QuEntity::applyRotation(DirectX::XMVECTOR quat)
{
	m_Transform.ApplyRotation(quat);
	m_dirtyWorldMatrix = true;
}


void QuEntity::updateWorldMatrix()
{
	if (!m_dirtyWorldMatrix && !m_Transform.isDirty()) return;

	m_dirtyWorldMatrix = false;

	DirectX::XMMATRIX res = DirectX::XMLoadFloat4x4(&m_Transform.toMatrix());
	DirectX::XMMATRIX tmp;

	// Compose parent transforms.
	for (QuEntity* parent = m_Parent; parent; parent = parent->m_Parent)
	{
		tmp = DirectX::XMLoadFloat4x4(&parent->m_Transform.toMatrix());
		res = DirectX::XMMatrixMultiply(res, tmp);
	}

	DirectX::XMStoreFloat4x4(&m_cachedWorldMatrix, res);
}
