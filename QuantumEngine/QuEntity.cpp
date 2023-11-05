#include "Quantum/Math/Math.h"

#include "QuEntity.h"

using namespace DirectX;


QuEntity::QuEntity()
	: m_Parent(0), m_FirstChild(0), m_Sibling(0)
	, m_dirtyWorldMatrix(true)
{
	//init to null
}

QuEntity::~QuEntity()
{
}

void QuEntity::attachChild(QuEntity* child)
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


XMVECTOR QuEntity::getWorldPosition()
{
	// Extract translate component from the world matrix.
	const XMFLOAT4X4& mat = this->GetWorldTransformMatrix();
	const XMFLOAT4& pos = *((const XMFLOAT4*) mat.m[3]);

	return XMLoadFloat4(&pos);
}

XMVECTOR QuEntity::getWorldRotation()
{
	XMVECTOR quat = XMLoadFloat4(&m_Transform.getRotation());
	XMVECTOR pqua;

	for (QuEntity* parent = this->m_Parent; parent; parent = parent->m_Parent) {
		pqua = XMLoadFloat4(&parent->m_Transform.getRotation());
		quat = XMQuaternionMultiply(quat, pqua);
	}

	return quat;
}

XMVECTOR QuEntity::getForwardVector()
{
	XMVECTOR v = XMVectorSet(0, 0, 1, 0);
	XMMATRIX worldMat = XMLoadFloat4x4(&this->GetWorldTransformMatrix());

	return XMVector3Normalize(XMVector4Transform(v, worldMat));
}

XMVECTOR QuEntity::getRightVector()
{
	XMVECTOR v = XMVectorSet(1, 0, 0, 0);
	XMMATRIX worldMat = XMLoadFloat4x4(&this->GetWorldTransformMatrix());

	return XMVector3Normalize(XMVector4Transform(v, worldMat));
}

XMVECTOR QuEntity::getUpVector()
{
	XMVECTOR v = XMVectorSet(0, 1, 0, 0);
	XMMATRIX worldMat = XMLoadFloat4x4(&this->GetWorldTransformMatrix());

	return XMVector3Normalize(XMVector4Transform(v, worldMat));
}


void QuEntity::setPosition(DirectX::XMFLOAT3 positon) 
{
	m_Transform.setPosition(positon);
	m_dirtyWorldMatrix = true;
}

void QuEntity::setRotation(DirectX::XMFLOAT4 quat)
{
	m_Transform.setRotation(quat);
	this->setDirtyWorldMatrix();
}

void QuEntity::setScale(DirectX::XMFLOAT3 scale)
{
	m_Transform.setScale(scale);
	this->setDirtyWorldMatrix();
}

void QuEntity::applyRotation(DirectX::XMVECTOR quat)
{
	m_Transform.ApplyRotation(quat);
	this->setDirtyWorldMatrix();
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

void QuEntity::setDirtyWorldMatrix()
{
	m_dirtyWorldMatrix = true;

	for (QuEntity* c = m_FirstChild; c; c = c->m_Sibling) {
		c->setDirtyWorldMatrix();
	}
}
