struct VS_INPUT
{
	float4 position: POSITION0;
	float2 texcoord: TEXCOORD0;
	float3 normal: NORMAL0;
};

struct VS_OUTPUT
{
	float4 position: SV_POSITION;
	float2 texcoord: TEXCOORD0;
	float3 normal: NORMAL0;
	float3 dir_to_camera: TEXCOORD1;
};

cbuffer constant: register(b0)
{
	row_major float4x4 m_world;
	row_major float4x4 m_view;
	row_major float4x4 m_proj;
	float4 m_light_dir;
	float4 m_cam_pos;
};

VS_OUTPUT vsmain(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	// world space
	output.position = mul(input.position, m_world);
	output.dir_to_camera = normalize(output.position.xyz - m_cam_pos.xyz);
	// view space
	output.position = mul(output.position, m_view);
	// screen space
	output.position = mul(output.position, m_proj);

	output.texcoord = input.texcoord;
	output.normal = input.normal;
	
	return output;
}