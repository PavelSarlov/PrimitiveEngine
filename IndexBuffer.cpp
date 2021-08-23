#include "IndexBuffer.h"
#include "RenderSystem.h"

#include <exception>

IndexBuffer::IndexBuffer(void *list_indices, UINT size_list, RenderSystem *system) : m_system(system)
{
	if(m_buffer) m_buffer->Release();

	D3D11_BUFFER_DESC buff_desc = {};
	buff_desc.Usage = D3D11_USAGE_DEFAULT;
	buff_desc.ByteWidth = 4 * size_list;
	buff_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	buff_desc.CPUAccessFlags = 0;
	buff_desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA init_data = {};
	init_data.pSysMem = list_indices;

	this->m_size_list = size_list;

	if(FAILED(this->m_system->m_d3d_device->CreateBuffer(&buff_desc, &init_data, &this->m_buffer)))
	{
		throw std::exception("IndexBuffer creation failed");
	}
}

IndexBuffer::~IndexBuffer()
{
	if(this->m_buffer) this->m_buffer->Release();
}

UINT IndexBuffer::getSizeIndexList()
{
	return this->m_size_list;
}
