matrix			g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
matrix			g_ViewMatrixInv, g_ProjMatrixInv;

vector			g_vCamPosition;

float			g_CameraFar;

vector			g_vLightDir;
vector			g_vLightPos;
float			g_fRange;
vector			g_vLightDiffuse;
vector			g_vLightAmbient;
vector			g_vLightSpecular;


vector			g_vMtrlAmbient = vector(1.f, 1.f, 1.f, 1.f);
vector			g_vMtrlSpecular = vector(1.f, 1.f, 1.f, 1.f);

texture2D		g_Texture;
texture2D		g_NormalTexture;
texture2D		g_DiffuseTexture;
texture2D		g_NonLightDiffuseTexture;
texture2D		g_BrightTexture;
texture2D		g_ShadeTexture;
texture2D		g_DepthTexture;
texture2D		g_SpecularTexture;

texture2D		g_BlurXTexture;
texture2D		g_BlurYTexture;

texture2D		g_BloomTexture;
texture2D		g_BloomOriginTexture;

float2			g_TexSize;


sampler LinearSampler = sampler_state
{
	filter = min_mag_mip_linear;
};

sampler BlurSampler = sampler_state
{
	filter = min_mag_mip_linear;
	AddressU = clamp;
	AddressV = clamp;
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

/* 픽셀의 색을 결정한다. */
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

	// uv좌표와 투영스페이스의 관계성으로 uv를 이용해서 투영의 xy 좌표를 만들고 z정보는 랜더타겟에서 가져옴
	
	///* 월드위치 * 뷰행렬 * 투영행렬 * 1/z */
	//vWorldPos.x = In.vTexUV.x * 2.f - 1.f;
	//vWorldPos.y = In.vTexUV.y * -2.f + 1.f;
	//vWorldPos.z = vDepthDesc.x;
	//vWorldPos.w = 1.f;

	///* 월드위치 * 뷰행렬 * 투영행렬 */
	//vWorldPos = vWorldPos * fViewZ;

	///* 월드위치 * 뷰행렬 */
	//vWorldPos = mul(vWorldPos, g_ProjMatrixInv);

	///* 월드위치 */
	//vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

	//vector	vReflect = reflect(normalize(g_vLightDir), vNormal);
	//vector	vLook = vWorldPos - g_vCamPosition;
	//Out.vSpecular.xyz = (g_vLightSpecular * g_vMtrlSpecular) * pow(saturate(dot(normalize(vReflect) * -1.f, normalize(vLook))), 30);

	return Out;
}

PS_OUT_LIGHT PS_MAIN_LIGHT_POINT(PS_IN In)
{
	PS_OUT_LIGHT			Out = (PS_OUT_LIGHT)0;

	vector	vNormalDesc = g_NormalTexture.Sample(LinearSampler, In.vTexUV);
	vector	vDepthDesc = g_DepthTexture.Sample(LinearSampler, In.vTexUV);
	float	fViewZ = vDepthDesc.y * 300.f;

	vector	vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);

	Out.vShade = g_vLightDiffuse * (saturate(dot(normalize(g_vLightDir) * -1.f, vNormal)) +
		(g_vLightAmbient * g_vMtrlAmbient));

	Out.vShade.a = 1.f;


	vector	vWorldPos;

	// uv좌표와 투영스페이스의 관계성으로 uv를 이용해서 투영의 xy 좌표를 만들고 z정보는 랜더타겟에서 가져옴

	///* 월드위치 * 뷰행렬 * 투영행렬 * 1/z */
	vWorldPos.x = In.vTexUV.x * 2.f - 1.f;
	vWorldPos.y = In.vTexUV.y * -2.f + 1.f;
	vWorldPos.z = vDepthDesc.x;
	vWorldPos.w = 1.f;

	///* 월드위치 * 뷰행렬 * 투영행렬 */
	vWorldPos = vWorldPos * fViewZ;

	///* 월드위치 * 뷰행렬 */
	vWorldPos = mul(vWorldPos, g_ProjMatrixInv);

	///* 월드위치 */
	vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

	vector	vLightDir = vWorldPos - g_vLightPos;

	/* 내 픽셀과 광원의 거리를 구하자. */
	float	fDistance = length(vLightDir);

	float	fAtt = saturate((g_fRange - fDistance) / g_fRange);

	Out.vShade = Out.vShade * fAtt;

	Out.vShade.a = 1.f;

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


/* 스카이박스 이후에 디퍼드를 그리면
알파가 0이되면 0이면 discard해서 미리 그렸던 스카이박스에 대한 연산을 하지 않으려 했던 작업이
스펙큘러에 의해 0.f이상이 되면 쓰레기값이 들어가면서 
태양같이 빛이 모이는 생기는 현상이 생김 이것을 막으려고 
1. Renderer에서 스펙큘러의 알파를 0으로 만들어주기
2. Out.vSpecular.xyz <- a의 값을 아예 배제해서 Out을 리턴해줌*/
//Out.vColor = vDiffuse * vShade + vSpecular;

	Out.vColor = vDiffuse * vShade;

	if (Out.vColor.a == 0.f)
		discard;

	return Out;
}

PS_OUT PS_MAIN_DEFERRED_BRIGHT(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	float4		vNonLightColor = g_NonLightDiffuseTexture.Sample(LinearSampler, In.vTexUV);
	//float4		vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	//vector		vShade = g_ShadeTexture.Sample(LinearSampler, In.vTexUV);

	float4 BrightColor = float4(0.f, 0.f, 0.f, 0.f);
	float brightness = dot(vNonLightColor.rgb, float3(0.2126f, 0.7152f, 0.0722f));
	if (brightness >= 0.95)
		BrightColor = float4(vNonLightColor.rgb, 1.0);

	Out.vColor = BrightColor;

	return Out;
}


static const float BlurWeights[7][7] =
{
	{0.0000, 0.0002, 0.0011, 0.0018, 0.0011, 0.0002, 0.0000},
	{0.0002, 0.0029, 0.0131, 0.0215, 0.0131, 0.0029, 0.0002},
	{0.0011, 0.0131, 0.0585, 0.0965, 0.0585, 0.0131, 0.0011},
	{0.0018, 0.0215, 0.0965, 0.1592, 0.0965, 0.0215, 0.0018},
	{0.0011, 0.0131, 0.0585, 0.0965, 0.0585, 0.0131, 0.0011},
	{0.0002, 0.0029, 0.0131, 0.0215, 0.0131, 0.0029, 0.0002},
	{0.0000, 0.0002, 0.0011, 0.0018, 0.0011, 0.0002, 0.0000}

};

static const float vTotal = 0.9976;


static const float Weights[13] =
{
	0.0561, 0.1353, 0.278, 0.4868, 0.7261, 0.9231,
	1, 0.9231, 0.7261, 0.4868, 0.278, 0.1353, 0.0561
};

static const float Total = 6.2108;

struct PS_OUT_BLOOM
{
	float4		vBloomColor : SV_TARGET0;
};

PS_OUT_BLOOM PS_MAIN_DEFERRED_BLURX(PS_IN In)
{
	PS_OUT_BLOOM			Out = (PS_OUT_BLOOM)0;

	//float4	vBrightDesc = g_BrightTexture.Sample(BlurSampler, In.vTexUV);

	//if (vBrightDesc.a == 0.f)
	//{
	//	discard;
	//}

	//float4 color = float4(0.f, 0.f, 0.f, 0.f);
	//float2 texelSize = 1.f / g_TexSize;

	//for (int x = -3; x <= 3; ++x)
	//{
	//	for (int y = -3; y <= 3; ++y)
	//	{
	//		color += BlurWeights[x + 3][y + 3] * 256 * g_BrightTexture.Sample(BlurSampler, In.vTexUV + float2(x * texelSize.x, y * texelSize.y));
	//	}
	//}

	//color = color / (vTotal * 64);

	//Out.vBloomColor = color;
	

	float4 vColor = float4(0.f, 0.f, 0.f, 0.f);
	float2 t = In.vTexUV;
	float2 uv = float2(0.f, 0.f);

	float tu = 1.f / g_TexSize.x;

	for (int i = -6; i < 6; ++i)
	{
		uv = t + float2(tu * i, 0.f);
		vColor += Weights[6+i] * g_BrightTexture.Sample(BlurSampler, uv);
	}

	vColor /= Total;

	Out.vBloomColor = vColor;

	return Out;
}

PS_OUT_BLOOM PS_MAIN_DEFERRED_BLURY(PS_IN In)
{
	PS_OUT_BLOOM			Out = (PS_OUT_BLOOM)0;

	float4 vColor = float4(0.f, 0.f, 0.f, 0.f);
	float2 t = In.vTexUV;
	float2 uv = float2(0.f, 0.f);

	float tv = 1.f / (g_TexSize.y * 0.5f);

	for (int i = -6; i < 6; ++i)
	{
		uv = t + float2(0.f, tv * i);
		vColor += Weights[6 + i] * g_BlurXTexture.Sample(BlurSampler, uv);
	}

	vColor /= Total;

	Out.vBloomColor = vColor;

	return Out;
}

PS_OUT PS_MAIN_DEFERRED_BLOOM_BLEND(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vNonLightDiffuse = g_NonLightDiffuseTexture.Sample(LinearSampler, In.vTexUV);
	vector		vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	vector		vShade = g_ShadeTexture.Sample(LinearSampler, In.vTexUV);
	vector		vBloomOriginColor = g_BloomOriginTexture.Sample(LinearSampler, In.vTexUV);
	vector		vBloomColor = g_BloomTexture.Sample(LinearSampler, In.vTexUV);

	Out.vColor = vDiffuse * vShade;

	if (Out.vColor.a == 0.f)
		discard;

	if (vNonLightDiffuse.a != 0.f)
	{
		Out.vColor = vNonLightDiffuse;
		float4		vBloom = pow(pow(abs(vBloomColor), 2.2f) + pow(abs(vBloomOriginColor), 2.2f), 1.f / 2.2f);

		Out.vColor = pow(abs(Out.vColor), 2.2f);;
		vBloom = pow(abs(vBloom), 2.2f);

		Out.vColor += vBloom;
		Out.vColor = pow(abs(Out.vColor), 1 / 2.2f);
	}

	

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

BlendState BS_Add_One
{
	BlendEnable[0] = true;
	BlendEnable[1] = true;

	SrcBlend = one;
	DestBlend = one;
	BlendOp = add;
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
		SetBlendState(BS_Add_One, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_LIGHT_POINT();
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

	pass Deferred_Bright
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Not_ZTest_Not_ZWrite, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DEFERRED_BRIGHT();
	}

	pass Deferred_BlurX
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Not_ZTest_Not_ZWrite, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DEFERRED_BLURX();
	}

	pass Deferred_BlurY
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Not_ZTest_Not_ZWrite, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DEFERRED_BLURY();
	}

	pass Deferred_Bloom_Blend
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Not_ZTest_Not_ZWrite, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DEFERRED_BLOOM_BLEND();
	}
}