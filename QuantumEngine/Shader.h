#pragma once

#include <d3d12.h>

#include <vector>



enum ShaderParameterType
{
	TYPE_CONSTANT_BUFFER = 0,
	TYPE_TEXTURE_2D,

	TYPE_UNKNOWN = 0xFFFFFFFF,
};

enum ShaderParameterUpdate
{
	UPDATE_PER_INSTANCE = 0,
	UPDATE_PER_MATERIAL,
	UPDATE_PER_FRAME,
	UPDATE_NUM_UPDATES,

	UPDATE_UNKNOWN = 0xFFFFFFFF
};

struct ShaderParameter
{
	const char* name;
	int slot;
	ShaderParameterType type;
	ShaderParameterUpdate updatePolicy;
};



class Shader
{
public:

	enum Type
	{
		SHADER_VS,
		SHADER_PS,
		SHADER_NUM_TYPES
	};

	Shader();
	~Shader();

	void destroy();

	template<Type T> inline void setShaderSource(const char* source, SIZE_T size) { setShaderSource(T, source, size); }

	bool compile();

		/// Root signature must be valid, and shader must have at least a vertex and a pixel shader.
	bool isReady() const;

	void setConstantBuffer(int shaderRegister, int offset);
	void setTexture2D(int shaderRegister, int offset);


	inline ID3D12RootSignature* getRootSignature() const { return m_rootSignature; }

	template<Type T> inline ID3DBlob* getShaderBlob() const { return m_shaderBytecodes[T]; }
	template<Type T> inline D3D12_SHADER_BYTECODE getShaderBytecode() const { return {m_shaderBytecodes[T]->GetBufferPointer(), m_shaderBytecodes[T]->GetBufferSize()}; }


private:

	void setShaderSource(Type shaderType, const char* source, SIZE_T size);


private:

	std::vector<ShaderParameter> m_parameters;

		/// Root signature.
	ID3D12RootSignature* m_rootSignature;
	ID3DBlob* m_serializedRootSig;

		/// Compiled shader bytecodes.
	ID3DBlob* m_shaderBytecodes[SHADER_NUM_TYPES];
};
