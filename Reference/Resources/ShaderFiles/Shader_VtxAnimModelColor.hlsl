#include "Shader_Defines.hlsli"

matrix			g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
matrix			g_BoneMatrices[256]; /* 메시에 영향을 주는 뼈들이다.  VTF */

float4			g_vColor = float4(1.f, 1.f, 1.f, 1.f);

float			g_Ratio;

float			g_CameraFar;


struct VS_IN
{
	float3		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float4		vColor : COLOR;
	uint4		vBlendIndex : BLENDINDEX;
	float4		vBlendWeight : BLENDWEIGHT;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float4		vColor : COLOR;
	float4		vProjPos: TEXCOORD1;
	float4		vWorldPos : TEXCOORD2;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	/* 하드웨어 스키닝. w를 0으로 세팅하지 않으려는 보정 */
	float		fWeightW = 1.f - (In.vBlendWeight.x + In.vBlendWeight.y + In.vBlendWeight.z);

	matrix		BoneMatrix = g_BoneMatrices[In.vBlendIndex.x] * In.vBlendWeight.x +
		g_BoneMatrices[In.vBlendIndex.y] * In.vBlendWeight.y +
		g_BoneMatrices[In.vBlendIndex.z] * In.vBlendWeight.z +
		g_BoneMatrices[In.vBlendIndex.w] * fWeightW;

	vector		vPosition = mul(float4(In.vPosition, 1.f), BoneMatrix);
	vector		vNormal = normalize(mul(float4(In.vNormal, 0.f), BoneMatrix));

	Out.vPosition = mul(vPosition, matWVP);
	Out.vNormal = mul(vNormal, g_WorldMatrix);
	Out.vColor = In.vColor;
	Out.vProjPos = Out.vPosition;
	Out.vWorldPos = mul(float4(In.vPosition, 1.f), g_WorldMatrix);

	return Out;
}

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float4		vColor : COLOR;
	float4		vProjPos: TEXCOORD1;
	float4		vWorldPos : TEXCOORD2;
};

struct PS_OUT_COLOR

{
	float4		vColor : SV_TARGET0;
};

struct PS_OUT_COLOR_BRIGHT

{
	float4		vDiffuse : SV_TARGET0;
	float4		vBrightColor : SV_TARGET1;

};

PS_OUT_COLOR PS_MAIN_EFFECT(PS_IN In)
{
	PS_OUT_COLOR			Out = (PS_OUT_COLOR)0;

	//Out.vColor = In.vColor;
	vector		vMtrlDiffuse = In.vColor;

	if (vMtrlDiffuse.a < 0.1f)
		discard;

	/*vector		vReflect = reflect(normalize(g_vLightDir), normalize(In.vNormal));
	vector		vLook = In.vWorldPos - g_vCamPosition;
	float		fSpecular = pow(max(dot(normalize(vReflect) * -1.f, normalize(vLook)), 0.f), 30);*/

	Out.vColor = vMtrlDiffuse;

	return Out;
}

PS_OUT_COLOR_BRIGHT PS_MAIN_EFFECT_DEFERRED(PS_IN In)
{
	PS_OUT_COLOR_BRIGHT			Out = (PS_OUT_COLOR_BRIGHT)0;

	vector		vMtrlDiffuse = In.vColor;

	if (vMtrlDiffuse.a < 0.1f)
		discard;

	float4 BrightColor = float4(0.f, 0.f, 0.f, 0.f);
	float brightness = dot(In.vColor.rgb, normalize(float3(1.f, -1.f, 1.f)));
	if (brightness > 0.99)
		BrightColor = float4(In.vColor.rgb, 1.0);

	Out.vDiffuse = vMtrlDiffuse;
	Out.vBrightColor = BrightColor;

	return Out;
}

struct PS_OUT_DEFERRED
{
	float4		vDiffuse : SV_TARGET0;
	float4		vNormal : SV_TARGET1;
	float4		vDepth : SV_TARGET2;
};

struct PS_OUT_DEFERRED_EFFECT
{
	float4		vDiffuse : SV_TARGET0;
	float4		vXBlur : SV_TARGET1;
	float4		vYBlur : SV_TARGET2;
};


PS_OUT_DEFERRED PS_MAIN(PS_IN In)
{
	PS_OUT_DEFERRED			Out = (PS_OUT_DEFERRED)0;

	vector		vMtrlDiffuse = In.vColor;

	if (vMtrlDiffuse.a < 0.1f)
		discard;

	Out.vDiffuse = vMtrlDiffuse;
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f); // 디퍼드 셰이더에서 노말의 값을 0~1로 받기 때문에 이와같이 노말의 값을 변경함
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_CameraFar, 0.f, 0.f);

	return Out;
}

PS_OUT_DEFERRED PS_MAIN_TOON(PS_IN In)
{
	PS_OUT_DEFERRED			Out = (PS_OUT_DEFERRED)0;

	vector		vMtrlDiffuse = In.vColor;

	if (vMtrlDiffuse.a < 0.1f)
		discard;

	float4 diffuse = saturate(vMtrlDiffuse);

	diffuse = ceil(diffuse * 5) / 5.0f;

	Out.vDiffuse = In.vColor * diffuse;

	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f); // 디퍼드 셰이더에서 노말의 값을 0~1로 받기 때문에 이와같이 노말의 값을 변경함
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_CameraFar, 0.f, 0.f);

	return Out;
}


PS_OUT_DEFERRED PS_MAIN_COLOR(PS_IN In)
{
	PS_OUT_DEFERRED			Out = (PS_OUT_DEFERRED)0;

	Out.vDiffuse = g_vColor;
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_CameraFar, 0.f, 0.f);



	return Out;
}

PS_OUT_DEFERRED PS_MAIN_COLOR_RATIO(PS_IN In)
{
	PS_OUT_DEFERRED			Out = (PS_OUT_DEFERRED)0;

	//In.vColor = In.vColor + g_vColor * g_Ratio;

	Out.vDiffuse = saturate(In.vColor + g_vColor * g_Ratio);
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / g_CameraFar, 0.f, 0.f);

	return Out;
}


technique11		DefaultTechnique
{
	pass Model
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass Model_Color
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_COLOR();
	}

	pass Model_Color_Ratio
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_COLOR_RATIO();
	}

	pass Model_Color_Effect
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Enable_ZTest_Disable_ZWrite, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_EFFECT();
	}

	pass Model_Toon
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Default, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_TOON();
	}

	pass Model_Color_Effect_Deferred
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Enable_ZTest_Disable_ZWrite, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_EFFECT_DEFERRED();
	}
}