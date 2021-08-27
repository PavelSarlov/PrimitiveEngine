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

Texture2D Texture: register(t0);
sampler TextureSampler: register(s0);

struct PS_INPUT
{
	float4 position: SV_POSITION;
	float2 texcoord: TEXCOORD0;
};

cbuffer constant: register(b0)
{
	float m_distortion_level;
}

float2 distort(float2 p)
{
	float theta = atan2(p.y, p.x);
	float radius = length(p);

	radius = pow(radius, m_distortion_level);

	p.x = radius * cos(theta);
	p.y = radius * sin(theta);

	return 0.5 * (p + 1.0);
}

float4 psmain(PS_INPUT input) : SV_TARGET
{
	float2 xy = 2.0 * input.texcoord.xy - 1.0;
	float d = length(xy);
	float2 uv = (d < 1.0 ? distort(xy) : input.texcoord.xy);

	float4 color = (Texture.Sample(TextureSampler, uv));
	return color;
}