#include "Shader_Defines.hlsli"

matrix			g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D		g_DiffuseTexture;
float			g_CameraFar;


struct VS_IN
{
	float3		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float4		vColor : COLOR;
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

	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
	Out.vNormal = mul(vector(In.vNormal, 0.f), g_WorldMatrix);
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

//struct PS_OUT
//{
//	float4		vColor : SV_TARGET0;
//};
//
//PS_OUT PS_MAIN(PS_IN In)
//{
//	PS_OUT			Out = (PS_OUT)0;
//
//
//	//Out.vColor = In.vColor;
//	vector		vMtrlDiffuse = In.vColor;
//
//	float		fShade = max(dot(normalize(g_vLightDir) * -1.f, normalize(In.vNormal)), 0.f);
//
//	vector		vReflect = reflect(normalize(g_vLightDir), normalize(In.vNormal));
//	vector		vLook = In.vWorldPos - g_vCamPosition;
//	float		fSpecular = pow(max(dot(normalize(vReflect) * -1.f, normalize(vLook)), 0.f), 30);
//
//	Out.vColor = (g_vLightDiffuse * vMtrlDiffuse) * saturate(fShade + (g_vLightAmbient * g_vMtrlAmbient))
//		+ (g_vLightSpecular * g_vMtrlSpecular) * fSpecular;
//
//	return Out;
//}

struct PS_OUT
{
	float4		vDiffuse : SV_TARGET0;
	float4		vNormal : SV_TARGET1;
	float4		vDepth : SV_TARGET2;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	vector		vMtrlDiffuse = In.vColor;

	if (vMtrlDiffuse.a < 0.1f)
		discard;

	Out.vDiffuse = vMtrlDiffuse;
	// -1 ~ 1인 노말값을 부호가 없는 데이터에 던져주기위해 0~1로 보정해줌
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);

	// x : z / w 투영기준 z나누기 한 값
	// y : 뷰스페이스 공간에서의 데이터를 디퍼드에 전달해주기위한 값, 디퍼드에서 다시 카메라의 Far를 곱함
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


}