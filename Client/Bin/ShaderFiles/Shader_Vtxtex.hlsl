matrix			g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

texture2D		g_Texture;


sampler PointSampler = sampler_state
{
	filter = min_mag_mip_point;
};

sampler LinearSampler = sampler_state
{
	filter = min_mag_mip_linear;
};

struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	/* SV_POSITION: ��� ��ȯ( ���庯ȯ, �亯ȯ, ������ȯ )�� �� ���ƴ�. */
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
	float4		vProjPos : TEXCOORD1;
};

VS_OUT VS_MAIN(VS_IN In)
{
	/* 

	1 / tan(fov/2)		0				0				0
	0			1 / tan(fov/2)			0				0
	0					0		far / (far - near)		1
	0					0	(far * near) / (far - near)	0
	
	*/

	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;

	return Out;
}

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
	float4		vProjPos: TEXCOORD1;
};

struct PS_OUT
{
	float4	vColor : SV_TARGET0;
};


PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_Texture.Sample(LinearSampler, In.vTexUV);

	return Out;
};

technique11		DefaultTechnique
{
	pass BackGround
	{
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
		//PixelShader = NULL;
	}

	//pass Effect
	//{
	//	VertexShader = compile vs_5_0 VS_MAIN();
	//	PixelShader = compile ps_5_0 PS_MAIN_EFFECT();
	//}

	//pass UI
	//{
	//	VertexShader = compile vs_5_0 VS_MAIN();
	//	PixelShader = compile ps_5_0 PS_MAIN();
	//}
}