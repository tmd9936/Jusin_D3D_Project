#include "Shader_Defines.hlsli"

bool ShouldDiscard(float2 coords, float2 dimensions, float radius)
{
	float2 circle_center = float2(radius, radius);

	if (length(coords - circle_center) > radius
		&& coords.x < circle_center.x && coords.y < circle_center.y) return true; //first circle

	circle_center.x += dimensions.x - 2 * radius;

	if (length(coords - circle_center) > radius
		&& coords.x > circle_center.x && coords.y < circle_center.y) return true; //second circle

	circle_center.y += dimensions.y - 2 * radius;

	if (length(coords - circle_center) > radius
		&& coords.x > circle_center.x && coords.y > circle_center.y) return true; //third circle

	circle_center.x -= dimensions.x - 2 * radius;

	if (length(coords - circle_center) > radius
		&& coords.x < circle_center.x && coords.y > circle_center.y) return true; //fourth circle

	return false;

}

matrix			g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

matrix			g_BoneMatrices[256]; /* 메시에 영향을 주는 뼈들이다.  VTF */

texture2D		g_Texture;

texture2D		g_DiffuseTexture[2];
texture2D		g_MaskTexture;
texture2D		g_BrushTexture;
vector			g_vBrushPos = vector(10.f, 0.f, 10.f, 1.f);
float			g_fBrushRange = 1.f;


struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
	uint4		vBlendIndex : BLENDINDEX;
	float4		vBlendWeight : BLENDWEIGHT;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
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

	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;
	Out.vWorldPos = mul(float4(In.vPosition, 1.f), g_WorldMatrix);

	return Out;
}

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
	float4		vProjPos: TEXCOORD1;
	float4		vWorldPos : TEXCOORD2;
};

struct PS_OUT
{
	float4		vColor : SV_TARGET0;
};

/* 픽셀의 색을 결정한다. */
PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_Texture.Sample(PointSampler, In.vTexUV);

	if (Out.vColor.a < 0.1)
		discard;

	float2 coords = In.vTexUV * 5.f;
	if (ShouldDiscard(coords, 5.f, 1.f))
		discard;

	return Out;
}

technique11		DefaultTechnique
{
	pass Button_Idle
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
}