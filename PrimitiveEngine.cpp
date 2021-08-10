#include "PrimitiveEngine.h"

PrimitiveEngine::PrimitiveEngine() : m_d3d_device(0), m_dxgi_adapter(0), m_dxgi_device(0), m_dxgi_factory(0), m_feature_level(), m_imm_context(0), m_imm_device_context(0)
{}

PrimitiveEngine::~PrimitiveEngine()
{}

bool PrimitiveEngine::init()
{
	D3D_DRIVER_TYPE driver_types[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};
	UINT num_driver_types = ARRAYSIZE(driver_types);

	D3D_FEATURE_LEVEL feature_levels[] =
	{
		D3D_FEATURE_LEVEL_11_0
	};
	UINT num_feature_levels = ARRAYSIZE(feature_levels);

	HRESULT hrs = 0;

	for(UINT driver_type_index = 0; driver_type_index < num_driver_types; ++driver_type_index)
	{
		hrs = D3D11CreateDevice(nullptr, driver_types[driver_type_index], nullptr, NULL, feature_levels, num_feature_levels, D3D11_SDK_VERSION,
			&this->m_d3d_device, &this->m_feature_level, &this->m_imm_context);

		if(SUCCEEDED(hrs))
		{
			break;
		}
	}

	if(FAILED(hrs))
	{
		return false;
	}

	this->m_imm_device_context = new DeviceContext(this->m_imm_context);

	this->m_d3d_device->QueryInterface(__uuidof(IDXGIDevice), (void **)&m_dxgi_device);
	this->m_dxgi_device->GetParent(__uuidof(IDXGIAdapter), (void **)&m_dxgi_adapter);
	this->m_dxgi_adapter->GetParent(__uuidof(IDXGIFactory), (void **)&m_dxgi_factory);

	return true;
}

bool PrimitiveEngine::release()
{
	if(m_vs) m_vs->Release();
	if(m_ps) m_ps->Release();

	if(m_vsblob) m_vsblob->Release();
	if(m_psblob) m_psblob->Release();

	this->m_dxgi_device->Release();
	this->m_dxgi_adapter->Release();
	this->m_dxgi_factory->Release();

	this->m_d3d_device->Release();
	this->m_imm_device_context->release();

	return true;
}

PrimitiveEngine *PrimitiveEngine::get()
{
	static PrimitiveEngine pe;
	return &pe;
}

SwapChain *PrimitiveEngine::createSwapChain()
{
	return new SwapChain();
}

DeviceContext *PrimitiveEngine::getImmediateDeviceContext()
{
	return this->m_imm_device_context;
}

VertexBuffer *PrimitiveEngine::createVertexBuffer()
{
	return new VertexBuffer();
}

VertexShader *PrimitiveEngine::createVertexShader(const void *shader_byte_code, size_t byte_code_size)
{
	VertexShader *vs = new VertexShader();

	if(!vs->init(shader_byte_code, byte_code_size))
	{
		vs->release();
		return nullptr;
	}

	return vs;
}

PixelShader *PrimitiveEngine::createPixelShader(const void *shader_byte_code, size_t byte_code_size)
{
	PixelShader *ps = new PixelShader();

	if(!ps->init(shader_byte_code, byte_code_size))
	{
		ps->release();
		return nullptr;
	}

	return ps;
}

bool PrimitiveEngine::compileVertexShader(const wchar_t *file_name, const char *entry_point_name, void **shader_byte_code, size_t *byte_code_size)
{
	ID3DBlob *error_blob = nullptr;
	if(FAILED(::D3DCompileFromFile(file_name, nullptr, nullptr, entry_point_name, "vs_5_0", 0, 0, &this->m_blob, &error_blob)))
	{
		if(error_blob) error_blob->Release();
		return false;
	}

	*shader_byte_code = this->m_blob->GetBufferPointer();
	*byte_code_size = this->m_blob->GetBufferSize();

	return true;
}

bool PrimitiveEngine::compilePixelShader(const wchar_t *file_name, const char *entry_point_name, void **shader_byte_code, size_t *byte_code_size)
{
	ID3DBlob *error_blob = nullptr;
	if(FAILED(::D3DCompileFromFile(file_name, nullptr, nullptr, entry_point_name, "ps_5_0", 0, 0, &this->m_blob, &error_blob)))
	{
		if(error_blob) error_blob->Release();
		return false;
	}

	*shader_byte_code = this->m_blob->GetBufferPointer();
	*byte_code_size = this->m_blob->GetBufferSize();

	return true;
}

void PrimitiveEngine::releaseCompiledShader()
{
	if(this->m_blob) this->m_blob->Release();
}


//ECODE PrimitiveEngine::DrawPixel(HDC hdc, COORD x, COORD y, COLORREF c)
//{
//	if(!SetPixel(hdc, x, y, c))
//	{
//		return 1;
//	}
//	return 0;
//}
//
//ECODE PrimitiveEngine::DrawLine(HDC hdc, COORD x1, COORD y1, COORD x2, COORD y2, COLORREF c)
//{
//	COORD dx = x2 - x1,
//		dy = y2 - y1,
//		a, b, p;
//
//	if(abs(dx) >= abs(dy))
//	{
//		if(x1 > x2)
//		{
//			std::swap(x1, x2);
//			std::swap(y1, y2);
//			dx = -dx;
//			dy = -dy;
//		}
//
//		COORD yi = 1;
//		if(dy < 0)
//		{
//			yi = -1;
//			dy = -dy;
//		}
//
//		a = 2 * dy;
//		b = 2 * dx;
//		p = a - dx;
//
//		for(COORD x = x1 + 1, y = y1; x <= x2; x++)
//		{
//			if(DrawPixel(hdc, x, y, c))
//			{
//				return 1;
//			}
//
//			if(p >= 0)
//			{
//				y += yi;
//				p -= b;
//			}
//			p += a;
//		}
//	}
//	else
//	{
//		if(y1 > y2)
//		{
//			std::swap(x1, x2);
//			std::swap(y1, y2);
//			dx = -dx;
//			dy = -dy;
//		}
//
//		COORD xi = 1;
//		if(dx < 0)
//		{
//			xi = -1;
//			dx = -dx;
//		}
//
//		a = 2 * dx;
//		b = 2 * dy;
//		p = a - dy;
//
//		for(COORD x = x1, y = y1 + 1; y <= y2; y++)
//		{
//			if(DrawPixel(hdc, x, y, c))
//			{
//				return 1;
//			}
//
//			if(p >= 0)
//			{
//				x += xi;
//				p -= b;
//			}
//			p += a;
//		}
//	}
//
//	return 0;
//}
//
//ECODE PrimitiveEngine::DrawTriangle(HDC hdc, COORD x1, COORD y1, COORD x2, COORD y2, COORD x3, COORD y3, COLORREF c)
//{
//	if(DrawLine(hdc, x1, y1, x2, y2, c) ||
//		DrawLine(hdc, x2, y2, x3, y3, c) ||
//		DrawLine(hdc, x3, y3, x1, y1, c))
//	{
//		return 1;
//	}
//
//	return 0;
//}
//
//ECODE PrimitiveEngine::DrawCircle(HDC hdc, COORD x1, COORD y1, COORD r, COLORREF c)
//{
//	COORD e = -r,
//		x = r,
//		y = 0;
//
//	while(y <= x)
//	{
//		DrawPixel(hdc, x1 + x, y1 + y, c);
//		DrawPixel(hdc, x1 - x, y1 + y, c);
//		DrawPixel(hdc, x1 + x, y1 - y, c);
//		DrawPixel(hdc, x1 - x, y1 - y, c);
//		DrawPixel(hdc, x1 + y, y1 + x, c);
//		DrawPixel(hdc, x1 - y, y1 + x, c);
//		DrawPixel(hdc, x1 + y, y1 - x, c);
//		DrawPixel(hdc, x1 - y, y1 - x, c);
//
//		e += 2 * y + 1;
//		y++;
//
//		if(e >= 0)
//		{
//			e -= 2 * x - 1;
//			x--;
//		}
//	}
//
//	return 0;
//}
//
//ECODE PrimitiveEngine::FillCircle(HDC hdc, COORD x1, COORD y1, COORD r, COLORREF c)
//{
//	COORD e = -r,
//		x = r,
//		y = 0;
//
//	while(y <= x)
//	{
//		DrawLine(hdc, x1 - x, y1 + y, x1 + x, y1 + y, c);
//		DrawLine(hdc, x1 - x, y1 - y, x1 + x, y1 - y, c);
//		DrawLine(hdc, x1 - y, y1 + x, x1 + y, y1 + x, c);
//		DrawLine(hdc, x1 - y, y1 - x, x1 + y, y1 - x, c);
//
//		e += 2 * y + 1;
//		y++;
//
//		if(e >= 0)
//		{
//			e -= 2 * x - 1;
//			x--;
//		}
//	}
//
//	return 0;
//}
//
//ECODE PrimitiveEngine::FillTriangle(HDC hdc, COORD x1, COORD y1, COORD x2, COORD y2, COORD x3, COORD y3, COLORREF c)
//{
//	SortLeftRightTriangle(x1, y1, x2, y2, x3, y3);
//
//	COORD dx2 = x2 - x1,
//		dy2 = y2 - y1,
//		dx3 = x3 - x1,
//		dy3 = y3 - y1,
//		curY2 = y1,
//		curY3 = y1,
//		a2, b2, a3, b3, p2, p3, yi2 = 1, yi3 = 1;
//
//	if(dy2 < 0)
//	{
//		yi2 = -1;
//		dy2 = -dy2;
//	}
//
//	if(dy3 < 0)
//	{
//		yi3 = -1;
//		dy3 = -dy3;
//	}
//
//	if(abs(dx2) >= abs(dy2))
//	{
//		a2 = 2 * dy2;
//		b2 = 2 * dx2;
//		p2 = a2 - dx2;
//	}
//	else
//	{
//		a2 = 2 * dx2;
//		b2 = 2 * dy2;
//		p2 = a2 - dy2;
//	}
//
//	if(abs(dx3) >= abs(dy3))
//	{
//		a3 = 2 * dy3;
//		b3 = 2 * dx3;
//		p3 = a3 - dx3;
//	}
//	else
//	{
//		a3 = 2 * dx3;
//		b3 = 2 * dy3;
//		p3 = a3 - dy3;
//	}
//
//	for(COORD x = x1; x <= x3; x++)
//	{
//		if(DrawLine(hdc, x, curY2, x, curY3, c))
//		{
//			return 1;
//		}
//
//		if(x == x2 && curY2 == y2)
//		{
//			dx2 = x3 - x2;
//			dy2 = y3 - y2;
//			yi2 = 1;
//
//			if(dy2 < 0)
//			{
//				yi2 = -1;
//				dy2 = -dy2;
//			}
//
//			if(abs(dx2) >= abs(dy2))
//			{
//				a2 = 2 * dy2;
//				b2 = 2 * dx2;
//				p2 = a2 - dx2;
//			}
//			else
//			{
//				a2 = 2 * dx2;
//				b2 = 2 * dy2;
//				p2 = a2 - dy2;
//			}
//		}
//
//		if(abs(dx2) >= abs(dy2))
//		{
//			if(p2 >= 0)
//			{
//				while(p2 >= 0)
//				{
//					curY2 += yi2;
//					p2 -= b2 - a2;
//				}
//			}
//			else
//			{
//				p2 += a2;
//			}
//		}
//		else
//		{
//			if(p2 < 0)
//			{
//				curY2 += yi2;
//				p2 += a2;
//			}
//			else
//			{
//				while(p2 >= 0)
//				{
//					curY2 += yi2;
//					p2 -= b2 - a2;
//				}
//			}
//		}
//
//		if(abs(dx3) >= abs(dy3))
//		{
//			if(p3 >= 0)
//			{
//				while(p3 >= 0)
//				{
//					curY3 += yi3;
//					p3 -= b3 - a3;
//				}
//			}
//			else
//			{
//				p3 += a3;
//			}
//		}
//		else
//		{
//			if(p3 < 0)
//			{
//				curY3 += yi3;
//				p3 += a3;
//			}
//			else
//			{
//				while(p3 >= 0)
//				{
//					curY3 += yi3;
//					p3 -= b3 - a3;
//				}
//			}
//		}
//	}
//
//	return 0;
//}
//
//void PrimitiveEngine::SortLeftRightTriangle(COORD &x1, COORD &y1, COORD &x2, COORD &y2, COORD &x3, COORD &y3)
//{
//	if(x1 > x2 || (x1 == x2 && y1 > y2))
//	{
//		std::swap(x1, x2);
//		std::swap(y1, y2);
//	}
//
//	if(x1 > x3 || (x1 == x3 && y1 > y3))
//	{
//		std::swap(x1, x3);
//		std::swap(y1, y3);
//	}
//
//	if(x2 > x3 || (x2 == x3 && y2 > y3))
//	{
//		std::swap(x2, x3);
//		std::swap(y2, y3);
//	}
//}
//
//ECODE PrimitiveEngine::FillRectangle(HDC hdc, COORD x1, COORD y1, COORD x2, COORD y2, COLORREF c)
//{
//	if(x1 > x2)
//	{
//		std::swap(x1, x2);
//	}
//	if(y1 > y2)
//	{
//		std::swap(y1, y2);
//	}
//
//	for(COORD x = x1; x <= x2; x++)
//	{
//		for(COORD y = y1; y <= y2; y++)
//		{
//			SetPixel(hdc, x, y, c);
//		}
//	}
//
//	return 0;
//}