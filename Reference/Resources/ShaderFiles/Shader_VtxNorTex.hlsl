matrix			g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

vector			g_vLightDir = vector(1.f, -1.f, 1.f, 0.f);
vector			g_vLightDiffuse = vector(1.f, 1.f, 1.f, 1.f);
vector			g_vLightAmbient = vector(1.f, 1.f, 1.f, 1.f);

texture2D		g_DiffuseTexture;
vector			g_vMtrlAmbient = vector(0.4f, 0.4f, 0.4f, 1.f);

sampler PointSampler = sampler_state
{
	filter = min_mag_mip_point;
	AddressU = wrap;
	AddressV = wrap;
};

sampler LinearSampler = sampler_state
{
	filter = min_mag_mip_linear;
	AddressU = wrap;
	AddressV = wrap;
};

struct VS_IN
{
	float3		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vProjPos: TEXCOORD1;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
	Out.vNormal = mul(vector(In.vNormal, 0.f), g_WorldMatrix);
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;

	return Out;
}

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vProjPos: TEXCOORD1;
};

struct PS_OUT
{
	float4		vColor : SV_TARGET0;
};

/* 픽셀의 색을 결정한다. */
PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV * 20.f);

	float		fShade = max(dot(normalize(g_vLightDir) * -1.f, normalize(In.vNormal)), 0.f);

	Out.vColor = (g_vLightDiffuse * vMtrlDiffuse) * saturate(fShade + (g_vLightAmbient * g_vMtrlAmbient));

	return Out;
}

technique11		DefaultTechnique
{
	pass BackGround
	{
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}
}