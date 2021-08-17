#pragma once
#ifndef PREREQUISITES_H
#define PREREQUISITES_H

#include <memory>

class SwapChain;
class DeviceContext;
class VertexBuffer;
class ConstantBuffer;
class IndexBuffer;
class VertexShader;
class PixelShader;
class RenderSystem;
class GraphicsEngine;

typedef std::shared_ptr<SwapChain> SwapChainPtr;
typedef std::shared_ptr<DeviceContext> DeviceContextPtr;
typedef std::shared_ptr<VertexBuffer> VertexBufferPtr;
typedef std::shared_ptr<ConstantBuffer> ConstantBufferPtr;
typedef std::shared_ptr<IndexBuffer> IndexBufferPtr;
typedef std::shared_ptr<VertexShader> VertexShaderPtr;
typedef std::shared_ptr<PixelShader> PixelShaderPtr;

#endif // !PREREQUISITES_H