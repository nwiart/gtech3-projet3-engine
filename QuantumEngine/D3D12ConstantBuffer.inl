template<class T> D3D12ConstantBuffer<T>::D3D12ConstantBuffer()
{

}

template<class T> void D3D12ConstantBuffer<T>::init(SIZE_T numElems)
{
	D3D12ConstantBufferBase::init(sizeof(T), numElems);
}

template<class T> void D3D12ConstantBuffer<T>::destroy()
{
	D3D12ConstantBufferBase::destroy();
}

template<class T> void D3D12ConstantBuffer<T>::update(int elemIndex, const T& data)
{
	D3D12ConstantBufferBase::update(elemIndex, &data, sizeof(T));
}

template<class T> void D3D12ConstantBuffer<T>::updateRange(int elemIndexStart, int elemCount, const T* data)
{
	D3D12ConstantBufferBase::updateRange(elemIndexStart, elemCount, data, elemCount * sizeof(T));
}
