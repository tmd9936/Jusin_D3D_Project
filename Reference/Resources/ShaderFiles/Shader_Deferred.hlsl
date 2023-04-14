matrix			g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
matrix			g_ViewMatrixInv, g_ProjMatrixInv;

vector			g_vCamPosition;

float			g_CameraFar;

vector			g_vLightDir;
vector			g_vLightDiffuse;
vector			g_vLightAmbient;
vector			g_vLightSpecular;


vector			g_vMtrlAmbient = vector(1.f, 1.f, 1.f, 1.f);
vector			g_vMtrlSpecular = vector(1.f, 1.f, 1.f, 1.f);

texture2D		g_Texture;
texture2D		g_NormalTexture;
texture2D		g_DiffuseTexture;
texture2D		g_ShadeTexture;
texture2D		g_DepthTexture;
texture2D		g_SpecularTexture;


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
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	float4		vColor : SV_TARGET0;
};

/* �ȼ��� ���� �����Ѵ�. */
PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	Out.vColor = g_Texture.Sample(LinearSampler, In.vTexUV);

	return Out;
}

struct PS_OUT_LIGHT
{
	float4		vShade : SV_TARGET0;
	//float4		vSpecular : SV_TARGET1;
};

PS_OUT_LIGHT PS_MAIN_LIGHT_DIRECTIONAL(PS_IN In)
{
	PS_OUT_LIGHT			Out = (PS_OUT_LIGHT)0;

	vector	vNormalDesc = g_NormalTexture.Sample(LinearSampler, In.vTexUV);
	//vector	vDepthDesc = g_DepthTexture.Sample(LinearSampler, In.vTexUV);
	//float	fViewZ = vDepthDesc.y * g_CameraFar;
	
	vector	vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);

	Out.vShade = g_vLightDiffuse * (saturate(dot(normalize(g_vLightDir) * -1.f, vNormal)) +
		(g_vLightAmbient * g_vMtrlAmbient));

	Out.vShade.a = 1.f; 

	//vector	vWorldPos;

	// uv��ǥ�� ���������̽��� ���輺���� uv�� �̿��ؼ� ������ xy ��ǥ�� ����� z������ ����Ÿ�ٿ��� ������
	
	///* ������ġ * ����� * ������� * 1/z */
	//vWorldPos.x = In.vTexUV.x * 2.f - 1.f;
	//vWorldPos.y = In.vTexUV.y * -2.f + 1.f;
	//vWorldPos.z = vDepthDesc.x;
	//vWorldPos.w = 1.f;

	///* ������ġ * ����� * ������� */
	//vWorldPos = vWorldPos * fViewZ;

	///* ������ġ * ����� */
	//vWorldPos = mul(vWorldPos, g_ProjMatrixInv);

	///* ������ġ */
	//vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

	//vector	vReflect = reflect(normalize(g_vLightDir), vNormal);
	//vector	vLook = vWorldPos - g_vCamPosition;
	//Out.vSpecular.xyz = (g_vLightSpecular * g_vMtrlSpecular) * pow(saturate(dot(normalize(vReflect) * -1.f, normalize(vLook))), 30);

	return Out;
}

PS_OUT PS_MAIN_DEFERRED_BLEND(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	vector		vShade = g_ShadeTexture.Sample(LinearSampler, In.vTexUV);
	//vector		vSpecular = g_SpecularTexture.Sample(LinearSampler, In.vTexUV);


/* ��ī�̹ڽ� ���Ŀ� ���۵带 �׸���
���İ� 0�̵Ǹ� 0�̸� discard�ؼ� �̸� �׷ȴ� ��ī�̹ڽ��� ���� ������ ���� ������ �ߴ� �۾���
����ŧ���� ���� 0.f�̻��� �Ǹ� �����Ⱚ�� ���鼭 
�¾簰�� ���� ���̴� ����� ������ ���� �̰��� �������� 
1. Renderer���� ����ŧ���� ���ĸ� 0���� ������ֱ�
2. Out.vSpecular.xyz <- a�� ���� �ƿ� �����ؼ� Out�� ��������*/
//Out.vColor = vDiffuse * vShade + vSpecular;

	Out.vColor = vDiffuse * vShade;

	if (Out.vColor.a == 0.f)
		discard;

	return Out;
}

RasterizerState RS_Default
{
	FillMode = solid;
};



DepthStencilState DSS_Default
{
	DepthEnable = true;
	DepthWriteMask = all;
	DepthFunc = less_equal;
};

DepthStencilState DSS_Not_ZTest_Not_ZWrite
{
	DepthEnable = false;
	DepthWriteMask = zero;
};

BlendState BS_Default
{
	BlendEnable[0] = false;
};



technique11		DefaultTechnique
{
	pass Debug
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

	pass LightAcc_Directional
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Not_ZTest_Not_ZWrite, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_LIGHT_DIRECTIONAL();
	}

	pass LightAcc_Point
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Not_ZTest_Not_ZWrite, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_LIGHT_DIRECTIONAL();
	}

	pass Deferred_Blend
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Not_ZTest_Not_ZWrite, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DEFERRED_BLEND();
	}
}