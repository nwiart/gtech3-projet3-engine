#pragma once

class VertexBuffer;
class IndexBuffer;


class Model
{
public :
	Model();
	~Model();

	inline VertexBuffer* GetVertexBuffer() { return VB; }
	inline IndexBuffer* GetIndexBuffer() { return IB; }
	inline unsigned int GetNumberTriangle() { return numTris; }

	void SetNumTriangle(unsigned int numTriangle);

private:
	unsigned int numTris;
	VertexBuffer* VB;
	IndexBuffer* IB;
};

