// MIT License
// 
// C++ 3D Game Tutorial Series (https://github.com/PardCode/CPP-3D-Game-Tutorial-Series)
// 
// Copyright (c) 2019-2021, PardCode
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <DeviceContext.h>
#include <SwapChain.h>
#include <VertexBuffer.h>
#include <ConstantBuffer.h>
#include <IndexBuffer.h>
#include <VertexShader.h>
#include <PixelShader.h>
#include <Texture.h>

#include <exception>

DeviceContext::DeviceContext(ID3D11DeviceContext *device_context, RenderSystem *system) : m_device_context(device_context), m_system(system)
{}

DeviceContext::~DeviceContext()
{
	if(this->m_device_context) this->m_device_context->Release();
}

void DeviceContext::setVertexBuffer(const VertexBufferPtr &vertex_buffer)
{
	UINT stride = vertex_buffer->m_size_vertex;
	UINT offset = 0;

	this->m_device_context->IASetVertexBuffers(0, 1, &vertex_buffer->m_buffer, &stride, &offset);
	this->m_device_context->IASetInputLayout(vertex_buffer->m_layout);
}

void DeviceContext::setViewPortSize(UINT width, UINT height)
{
	D3D11_VIEWPORT vp = {};
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	this->m_device_context->RSSetViewports(1, &vp);
}

void DeviceContext::setRenderTarget(const TexturePtr &render_target, const TexturePtr &depth_stencil)
{
	if(render_target->m_type != Texture::Type::RENDER_TARGET) return;
	if(depth_stencil->m_type != Texture::Type::DEPTH_STENCIL) return;

	this->m_device_context->OMSetRenderTargets(1, &render_target->m_render_target_view, depth_stencil->m_depth_stencil_view);
}

void DeviceContext::setVertexShader(const VertexShaderPtr &vertex_shader)
{
	this->m_device_context->VSSetShader(vertex_shader->m_vs, nullptr, 0);
}

void DeviceContext::setPixelShader(const PixelShaderPtr &pixel_shader)
{
	this->m_device_context->PSSetShader(pixel_shader->m_ps, nullptr, 0);
}

void DeviceContext::setTexture(const VertexShaderPtr &pixel_shader, const TexturePtr *texture, UINT num_textures)
{
	// for now won't use more than 32
	ID3D11ShaderResourceView *list_res[32];
	ID3D11SamplerState *list_samplers[32];
	for(UINT i = 0; i < num_textures; i++)
	{
		list_res[i] = texture[i]->m_shader_res_view;
		list_samplers[i] = texture[i]->m_sampler_state;
	}
	this->m_device_context->VSSetShaderResources(0, num_textures, list_res);
	this->m_device_context->PSSetSamplers(0, num_textures, list_samplers);
}

void DeviceContext::setTexture(const PixelShaderPtr &pixel_shader, const TexturePtr *texture, UINT num_textures)
{
	// for now won't use more than 32
	ID3D11ShaderResourceView *list_res[32];
	ID3D11SamplerState *list_samplers[32];
	for(UINT i = 0; i < num_textures; i++)
	{
		list_res[i] = texture[i]->m_shader_res_view;
		list_samplers[i] = texture[i]->m_sampler_state;
	}
	this->m_device_context->PSSetShaderResources(0, num_textures, list_res);
	this->m_device_context->PSSetSamplers(0, num_textures, list_samplers);
}

void DeviceContext::setConstantBuffer(const VertexShaderPtr &vertex_shader, const  ConstantBufferPtr &buffer)
{
	this->m_device_context->VSSetConstantBuffers(0, 1, &buffer->m_buffer);
}

void DeviceContext::setConstantBuffer(const PixelShaderPtr &pixel_shader, const ConstantBufferPtr &buffer)
{
	this->m_device_context->PSSetConstantBuffers(0, 1, &buffer->m_buffer);
}

void DeviceContext::setIndexBuffer(const IndexBufferPtr &index_buffer)
{
	this->m_device_context->IASetIndexBuffer(index_buffer->m_buffer, DXGI_FORMAT_R32_UINT, 0);
}

void DeviceContext::clearRenderTargetColor(const SwapChainPtr &swap_chain, float red, float green, float blue, float alpha)
{
	FLOAT clear_color[] = { red,green,blue,alpha };
	this->m_device_context->ClearRenderTargetView(swap_chain->m_rtv, clear_color);
	this->m_device_context->ClearDepthStencilView(swap_chain->m_dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	this->m_device_context->OMSetRenderTargets(1, &swap_chain->m_rtv, swap_chain->m_dsv);
}

void DeviceContext::clearDepthStencil(const SwapChainPtr &swap_chain)
{
	this->m_device_context->ClearDepthStencilView(swap_chain->m_dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
}

void DeviceContext::clearRenderTargetColor(const TexturePtr &render_target, float red, float green, float blue, float alpha)
{
	if(render_target->m_type != Texture::Type::RENDER_TARGET) return;
	FLOAT clear_color[] = { red,green,blue,alpha };
	this->m_device_context->ClearRenderTargetView(render_target->m_render_target_view, clear_color);
}

void DeviceContext::clearDepthStencil(const TexturePtr &depth_stencil)
{
	if(depth_stencil->m_type != Texture::Type::DEPTH_STENCIL) return;
	this->m_device_context->ClearDepthStencilView(depth_stencil->m_depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
}

void DeviceContext::drawTriangleList(UINT vertex_count, UINT start_vertex_index)
{
	this->m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	this->m_device_context->Draw(vertex_count, start_vertex_index);
}

void DeviceContext::drawIndexedTriangleList(UINT index_count, UINT start_index_location, UINT start_vertex_index)
{
	this->m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	this->m_device_context->DrawIndexed(index_count, start_index_location, start_vertex_index);
}

void DeviceContext::drawTriangleStrip(UINT vertex_count, UINT start_vertex_index)
{
	this->m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	this->m_device_context->Draw(vertex_count, start_vertex_index);
}