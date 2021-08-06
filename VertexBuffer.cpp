#include "VertexBuffer.h"

VertexBuffer::VertexBuffer()
{}

VertexBuffer::~VertexBuffer()
{}

bool VertexBuffer::Load(void *list_vertices, UINT size_vertex, UINT size_list, void *shader_byte_code, UINT size_byte_shader)
{
	if(m_buffer) m_buffer->Release();
	if(m_layout) m_layout->Release();

	D3D11_BUFFER_DESC buff_desc = {};
	buff_desc.Usage = D3D11_USAGE_DEFAULT;
	buff_desc.ByteWidth = size_vertex * size_list;
	buff_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buff_desc.CPUAccessFlags = 0;
	buff_desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA init_data = {};
	init_data.pSysMem = list_vertices;

	this->m_size_vertex = size_vertex;
	this->m_size_list = size_list;

	if(FAILED(PrimitiveEngine::Get()->m_d3d_device->CreateBuffer(&buff_desc, &init_data, &this->m_buffer)))
	{
		return false;
	}

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		// SEMANTIC NAME - SEMANTIC INDEX - FORMAT - INPUT SLOT - ALIGNED BYTE OFFSET - INPUT SLOT CLASS - INSTANCE DATA STEP RATE
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	UINT size_layout = ARRAYSIZE(layout);

	if(FAILED(PrimitiveEngine::Get()->m_d3d_device->CreateInputLayout(layout, size_layout, shader_byte_code, size_byte_shader, &this->m_layout)))
	{
		return false;
	}

	return true;
}

bool VertexBuffer::Release()
{
	this->m_layout->Release();
	this->m_buffer->Release();
	delete this;
	return true;
}

UINT VertexBuffer::GetSizeVertexList()
{
	return this->m_size_list;
}
