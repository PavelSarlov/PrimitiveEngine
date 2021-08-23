Texture2D Texture: register(t0);
sampler TextureSampler: register(s0);

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
	float4 m_light_dir;
};

float4 psmain(PS_INPUT input) : SV_TARGET
{
	// ambient light
	float ka = 0.1f;
	float3 ia = float3(1.0f, 1.0f, 1.0f);
	float3 ambient_light = ka * ia;

	// diffuse light
	float kd = 0.7f;
	float3 id = float3(1.0f, 1.0f, 1.0f);
	float amount_diffuse_light = max(dot(m_light_dir.xyz, input.normal), 0.0f);
	float3 diffuse_light = kd * amount_diffuse_light * id;

	// specular light
	float ks = 1.0f;
	float3 is = float3(1.0f, 1.0f, 1.0f);
	float3 reflected_light = reflect(m_light_dir.xyz, input.normal);
	float shininess = 30.0f;
	float amount_specular_light = pow(max(dot(reflected_light, input.dir_to_cam), 0.0f), shininess);
	float3 specular_light = ks * amount_specular_light * is;

	float3 final_light = ambient_light + diffuse_light + specular_light;

	return (Texture.Sample(TextureSampler, input.texcoord * 0.5) * float4(final_light, 1.0f));
}