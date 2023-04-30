#include "Shader_Defines.hlsli"

matrix			g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

texture2D		g_Texture;
texture2D		g_MaskTexture;

float4			g_vColor;
float			g_Progress;

float2			g_Size;

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

VS_OUT VS_MAIN_TRAIL(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWVP = mul(g_ViewMatrix, g_ProjMatrix);

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

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_Texture.Sample(LinearSampler, In.vTexUV);

	if (Out.vColor.a < 0.1)
		discard;

	return Out;
}

PS_OUT PS_MAIN_ROUND(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_Texture.Sample(LinearSampler, In.vTexUV);

	if (Out.vColor.a < 0.1)
		discard;

	float2 coords = In.vTexUV * g_Size;
	if (ShouldDiscard(coords, g_Size, 10.f))
		discard;

	return Out;
}


PS_OUT PS_MAIN_ALPHA(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_Texture.Sample(LinearSampler, In.vTexUV);

	if (Out.vColor.a < 0.1)
		discard;

	Out.vColor.a = g_vColor.a;

	return Out;
}

PS_OUT PS_MAIN_TRAIL(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_Texture.Sample(LinearSampler, In.vTexUV);

	if (Out.vColor.a < 0.1)
		discard;

	//Out.vColor.a = g_vColor.a;

	return Out;
}

PS_OUT PS_MAIN_COOLTIME_ALPHAMASK(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vMask = g_MaskTexture.Sample(LinearSampler, In.vTexUV);

	if (vMask.a < 0.1)
		discard;

	Out.vColor = g_Texture.Sample(PointSampler, In.vTexUV);

	float angle = CalculateClockAngle_float(In.vTexUV);
	Out.vColor = (angle > g_Progress) ? float4(0.f, 0.f, 0.f, 0.4f) : float4(0.f, 0.f, 0.f, 0.f);

	return Out;
}

PS_OUT PS_MAIN_PROGRESS_ROUND(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_Texture.Sample(LinearSampler, In.vTexUV);

	if (Out.vColor.a > 0.3)
	{
		return Out;
	}
	else
	{

		bool mask = In.vTexUV.x < g_Progress;
		//float healthbarMask = floor(i.uv.x * 10)/10 < _Health; // health bar split up into 10 chunks


		if (mask)
		{
			float2 coords = In.vTexUV * g_Size;
			if (ShouldDiscard(coords, g_Size, 5.f))
				discard;

			if (In.vTexUV.x < 0.09f || In.vTexUV.x > 0.91f)
				discard;

			if (In.vTexUV.y < 0.09f || In.vTexUV.y > 0.91f)
				discard;
			Out.vColor = g_vColor;
		}
		else
		{
			Out.vColor = float4(0.f, 0.f, 0.f, 0.0f);
		}

	}


	return Out;
}

PS_OUT PS_MAIN_ALPHA_BLEND(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_Texture.Sample(LinearSampler, In.vTexUV);

	if (Out.vColor.a < 0.1)
		discard;

	return Out;
}

PS_OUT PS_MAIN_CORNER_INSIDE_ALPHA(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_Texture.Sample(LinearSampler, In.vTexUV);

	float2 coords = In.vTexUV * g_Size;
	if (ShouldDiscard(coords, g_Size, 5.f))
		discard;

	if (Out.vColor.a > 0.1)
	{
		return Out;
	}
	else
	{
		Out.vColor = g_vColor;
	}

	return Out;
}

technique11		DefaultTechnique
{
	pass BackGround //1
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

	pass WorldPartUI //2
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Enable_ZTest_Disable_ZWrite, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_ROUND();
	}

	pass Alpha //3
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Enable_ZTest_Disable_ZWrite, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_ALPHA();
	}

	pass CoolTimeAlphaMask //4
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Enable_ZTest_Disable_ZWrite, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_COOLTIME_ALPHAMASK();
	}

	pass Trail //5
	{
		SetRasterizerState(RS_Solid_NoCull);
		SetDepthStencilState(DSS_Enable_ZTest_Disable_ZWrite, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN_TRAIL();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_TRAIL();
	}

	pass ProgressUI //6
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Enable_ZTest_Disable_ZWrite, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_PROGRESS_ROUND();
	}

	pass Alpha_Blend //7
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Enable_ZTest_Disable_ZWrite, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_ALPHA_BLEND();
	}

	pass Corner_Inside_Alpha //8
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Enable_ZTest_Disable_ZWrite, 0);
		SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_CORNER_INSIDE_ALPHA();
	}
}
