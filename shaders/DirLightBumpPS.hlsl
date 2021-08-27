Texture2D Color: register(t0);
sampler ColorSampler: register(s0);

Texture2D Normal: register(t1);
sampler NormalSampler: register(s1);

struct PS_INPUT
{
	float4 position: SV_POSITION;
	float2 texcoord: TEXCOORD0;
	float3 dir_to_cam: TEXCOORD1;
	row_major float3x3 tbn: TEXCOORD2;
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
	float4 color = Color.Sample(ColorSampler,float2(input.texcoord.x, 1.0 - input.texcoord.y));
	float4 normal = Normal.Sample(NormalSampler, float2(input.texcoord.x, 1.0 - input.texcoord.y));

	normal.xyz = (normal.xyz * 2.0) - 1.0;
	normal.xyz = mul(normal.xyz, input.tbn);

	float dot_nl = dot(m_light_dir.xyz, input.tbn[2]);

	// ambient light
	float ka = 8.5f;
	float3 ia = float3(0.09f, 0.082f, 0.082f);
	ia *= (color.rgb);
	float3 ambient_light = ka * ia;

	// diffuse light
	float kd = 0.7f;
	float amount_diffuse_light = dot(m_light_dir.xyz, normal.xyz);
	float3 id = float3(1,1,1);
	id *= (color.rgb);
	float3 diffuse_light = kd * id * amount_diffuse_light;

	// specular light
	float ks = 1.0;
	float3 is = float3(1.0f, 1.0f, 1.0f);
	float3 reflected_light = reflect(m_light_dir.xyz, normal.xyz);
	float shininess = 10.0f;
	float amount_specular_light = (dot_nl > 0 ? pow(max(0.0f, dot(reflected_light, input.dir_to_cam)), shininess) : 0);
	float3 specular_light = ks * amount_specular_light * is;

	float3 final_light = ambient_light + diffuse_light + specular_light;

	return float4(final_light, 1.0f);
}