#include "Shader_Defines.hlsli"

matrix			g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

texture2D		g_Texture;
float4			g_vColor;

float2			g_Size;
float			g_HpRatio = 1.f;
float			g_Radius;
float			g_Progress;

struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
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
	float4		vColor : SV_TARGET0;
};

/* 픽셀의 색을 결정한다. */
PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_Texture.Sample(LinearSampler, In.vTexUV);

	if (Out.vColor.a < 0.1)
		discard;

	Out.vColor = g_vColor;
	
	return Out;
}

/* 픽셀의 색을 결정한다. */
PS_OUT PS_MAIN_CORNER_ROUND(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_Texture.Sample(LinearSampler, In.vTexUV);

	if (Out.vColor.a < 0.1)
		discard;

	float2 coords = In.vTexUV * 10.f;
	if (ShouldDiscard(coords, 10.f, 1.f))
		discard;

	Out.vColor = g_vColor;

	return Out;
}

/* 픽셀의 색을 결정한다. */
PS_OUT PS_MAIN_HP(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_Texture.Sample(LinearSampler, In.vTexUV);

	if (Out.vColor.a > 0.1f)
	{
		Out.vColor = g_vColor;
	}
	else
	{

		float2 coords = In.vTexUV * 5.f;
		if (ShouldDiscard(coords, 5.f, 1.f))
			discard;

		float _Health = g_HpRatio;
		float4 healthbarColor = lerp(g_vColor, float4(g_vColor.x, g_vColor.y, g_vColor.z - 0.2f, g_vColor.w), _Health);
		float4 bgColor = float4(g_vColor.xyz, 0.f);

		bool healthbarMask = In.vTexUV.x < _Health;
		//float healthbarMask = floor(i.uv.x * 10)/10 < _Health; // health bar split up into 10 chunks

		if (healthbarMask)
		{
			Out.vColor = float4(g_vColor.xyz, 0.75f);
		}
		else
		{
			Out.vColor = float4(0.f, 0.f, 0.f, 0.6f);
		}
		//float4 finalColor = lerp(bgColor, healthbarColor, healthbarMask);

		//Out.vColor = float4(finalColor.xyz, 0.8);
	}

	return Out;
}


PS_OUT PS_MAIN_CLOCKWISECOOLTIME(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	//Out.vColor = g_Texture.Sample(LinearSampler, In.vTexUV);

	float4 sourceColor = g_Texture.Sample(LinearSampler, In.vTexUV);

	float angle;

	angle = CalculateClockAngle_float(In.vTexUV);
	Out.vColor = (angle > g_Progress) ? float4(0.f, 0.f, 0.f, 0.4f) : float4(0.f, 0.f, 0.f, 0.f);

	float2 coords = In.vTexUV * 7.f;

	if (ShouldDiscard(coords, 7.f, 1.f))
		discard;

	return Out;

}

technique11		DefaultTechnique
{
	pass ColorBase
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Enable_ZTest_Disable_ZWrite, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass CornerRound
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Enable_ZTest_Disable_ZWrite, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_CORNER_ROUND();
	}

	pass ColorBase_Alpha
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Enable_ZTest_Disable_ZWrite, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Hp
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Enable_ZTest_Disable_ZWrite, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_HP();
	}

	pass ClockWiseCoolTime
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Enable_ZTest_Disable_ZWrite, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_CLOCKWISECOOLTIME();
	}

}