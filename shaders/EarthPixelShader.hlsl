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

Texture2D EarthDay: register(t0);
sampler EarthDaySampler: register(s0);

Texture2D EarthSpecular: register(t1);
sampler EarthSpecularSampler: register(s1);

Texture2D Clouds: register(t2);
sampler CloudsSampler: register(s2);

Texture2D EarthNight: register(t3);
sampler EarthNightSampler: register(s3);

struct PS_INPUT
{
	float4 position: SV_POSITION;
	float2 texcoord: TEXCOORD0;
	float3 normal: NORMAL0;
	float3 dir_to_cam: TEXCOORD1;
};

cbuffer constant: register(b0)
{
	row_major float4x4 m_world;
	row_major float4x4 m_view;
	row_major float4x4 m_proj;
	float4 m_cam_pos;
	float4 m_light_dir;
	float4 m_light_pos;
	float m_light_radius;
	float m_time;
};
float4 psmain(PS_INPUT input) : SV_TARGET
{
	float4 earth_day = EarthDay.Sample(EarthDaySampler, 1.0f - input.texcoord);
	float earth_spec = EarthSpecular.Sample(EarthSpecularSampler, 1.0f - input.texcoord).r;
	float clouds = Clouds.Sample(CloudsSampler, 1.0f - input.texcoord + float2(m_time / 100.0, 0)).r;
	float4 earth_night = EarthNight.Sample(EarthNightSampler, 1.0f - input.texcoord);

	// ambient light
	float ka = 1.5f;
	float3 ia = float3(0.09f, 0.082f, 0.082f);
	ia *= (earth_day.rgb + clouds);
	float3 ambient_light = ka * ia;

	// diffuse light
	float kd = 0.7f;

	float3 id_day = float3(1.0f, 1.0f, 1.0f);
	id_day *= (earth_day.rgb + clouds);

	float3 id_night = float3(1.0f, 1.0f, 1.0f);
	id_night *= (earth_night.rgb + clouds);

	float amount_diffuse_light = dot(m_light_dir.xyz, input.normal);
	float3 id = lerp(id_night, id_day, (amount_diffuse_light + 1.0f) / 2.0f);
	float3 diffuse_light = kd * id;

	// specular light
	float ks = earth_spec;
	float3 is = float3(1.0f, 1.0f, 1.0f);
	float3 reflected_light = reflect(m_light_dir.xyz, input.normal);
	float shininess = 30.0f;
	float amount_specular_light = pow(max(0.0f, dot(reflected_light, input.dir_to_cam)), shininess);
	float3 specular_light = ks * amount_specular_light * is;

	float3 final_light = ambient_light + diffuse_light + specular_light;

	return float4(final_light, 1.0f);
}