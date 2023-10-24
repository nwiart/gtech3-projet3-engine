#pragma once

class VertexBuffer;
class IndexBuffer;



namespace Quantum
{



class SphereGenerator
{
public:

	static void generate(VertexBuffer& outVB, IndexBuffer& outIB);
};



} // namespace Quantum
