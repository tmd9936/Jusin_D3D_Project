matrix			g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
matrix			g_ViewMatrixInv, g_ProjMatrixInv;

matrix			g_matLightView;
matrix			g_matLightProj;

vector			g_vCamPosition;

float			g_CameraFar;

vector			g_vLightDir;
vector			g_vLightPos;
float			g_fRange;
vector			g_vLightDiffuse;
vector			g_vLightAmbient;
vector			g_vLightSpecular;

float			g_LightFar = 500.f;

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

texture2D		g_ShadowDepthTexture;
float			g_fSunWidth = 2.f;

float2			g_TexSize;

texture2D		g_GrayTexture;
texture2D		g_LaplacianTexture;

float4			g_bLaplacian;

float			g_laplacianThesholdLow;
float			g_laplacianThesholdHigh;

sampler ShadowDepthSampler = sampler_state
{
	filter = min_mag_linear_mip_point;

	AddressU = clamp;
	AddressV = clamp;
};

sampler PointSampler = sampler_state
{
	filter = min_mag_mip_point;

	AddressU = border;
	AddressV = border;
	AddressW = border;
	MipLODBias = 0.f;
	MaxAnisotropy = 1;
	ComparisonFunc = always;
	BorderColor = float4(1.f, 1.f, 1.f, 1.f);
	MinLOD = 0;
	MaxLOD = 3.4e+38;
};

SamplerComparisonState  PointSamplerCmp
{
	filter = COMPARISON_MIN_MAG_LINEAR_MIP_POINT;
	AddressU = border;
	AddressV = border;
	AddressW = border;
	MipLODBias = 0.f;
	MaxAnisotropy = 1;
	ComparisonFunc = less_equal;
	BorderColor = float4(1.f, 1.f, 1.f, 1.f);
	MinLOD = 0;
	MaxLOD = 3.4e+38;
};

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


#define FILTER_SIZE    11
#define FS  FILTER_SIZE
#define FS2 ( FILTER_SIZE / 2 )

// 4 control matrices for a dynamic cubic bezier filter weights matrix

static const float C3[11][11] =
{ { 1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0 },
  { 1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0 },
  { 1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0 },
  { 1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0 },
  { 1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0 },
  { 1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0 },
  { 1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0 },
  { 1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0 },
  { 1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0 },
  { 1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0 },
  { 1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0 },
};

static const float C2[11][11] =
{ { 0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0 },
  { 0.0,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.0 },
  { 0.0,0.2,1.0,1.0,1.0,1.0,1.0,1.0,1.0,0.2,0.0 },
  { 0.0,0.2,1.0,1.0,1.0,1.0,1.0,1.0,1.0,0.2,0.0 },
  { 0.0,0.2,1.0,1.0,1.0,1.0,1.0,1.0,1.0,0.2,0.0 },
  { 0.0,0.2,1.0,1.0,1.0,1.0,1.0,1.0,1.0,0.2,0.0 },
  { 0.0,0.2,1.0,1.0,1.0,1.0,1.0,1.0,1.0,0.2,0.0 },
  { 0.0,0.2,1.0,1.0,1.0,1.0,1.0,1.0,1.0,0.2,0.0 },
  { 0.0,0.2,1.0,1.0,1.0,1.0,1.0,1.0,1.0,0.2,0.0 },
  { 0.0,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.0 },
  { 0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0 },
};

static const float C1[11][11] =
{ { 0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0 },
  { 0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0 },
  { 0.0,0.0,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.0,0.0 },
  { 0.0,0.0,0.2,1.0,1.0,1.0,1.0,1.0,0.2,0.0,0.0 },
  { 0.0,0.0,0.2,1.0,1.0,1.0,1.0,1.0,0.2,0.0,0.0 },
  { 0.0,0.0,0.2,1.0,1.0,1.0,1.0,1.0,0.2,0.0,0.0 },
  { 0.0,0.0,0.2,1.0,1.0,1.0,1.0,1.0,0.2,0.0,0.0 },
  { 0.0,0.0,0.2,1.0,1.0,1.0,1.0,1.0,0.2,0.0,0.0 },
  { 0.0,0.0,0.2,0.2,0.2,0.2,0.2,0.2,0.2,0.0,0.0 },
  { 0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0 },
  { 0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0 },
};

static const float C0[11][11] =
{ { 0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0 },
  { 0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0 },
  { 0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0 },
  { 0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0 },
  { 0.0,0.0,0.0,0.0,0.8,0.8,0.8,0.0,0.0,0.0,0.0 },
  { 0.0,0.0,0.0,0.0,0.8,1.0,0.8,0.0,0.0,0.0,0.0 },
  { 0.0,0.0,0.0,0.0,0.8,0.8,0.8,0.0,0.0,0.0,0.0 },
  { 0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0 },
  { 0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0 },
  { 0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0 },
  { 0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0 },
};

// compute dynamic weight at a certain row, column of the matrix
float Fw(int r, int c, float fL)
{
	return (1.0 - fL) * (1.0 - fL) * (1.0 - fL) * C0[r][c] +
		fL * fL * fL * C3[r][c] +
		3.0f * (1.0 - fL) * (1.0 - fL) * fL * C1[r][c] +
		3.0f * fL * fL * (1.0 - fL) * C2[r][c];
}

#define BLOCKER_FILTER_SIZE    11
#define BFS  BLOCKER_FILTER_SIZE
#define BFS2 ( BLOCKER_FILTER_SIZE / 2 )

#define SUN_WIDTH g_fSunWidth

//======================================================================================
// This shader computes the contact hardening shadow filter
//======================================================================================
float shadow(float3 tc)
{
	float2 texelSize = float2(1.f / g_TexSize.x, 1.f / g_TexSize.y);
	float  s = 0.0f;
	float2 stc = (g_TexSize.xy * tc.xy) + float2(0.5, 0.5);
	float2 tcs = floor(stc);
	float2 fc;
	int    row;
	int    col;
	float  w = 0.0;
	float  avgBlockerDepth = 0;
	float  blockerCount = 0;
	float  fRatio;
	float4 v1[FS2 + 1];
	float2 v0[FS2 + 1];
	float2 off;

	fc = stc - tcs;
	tc.xy = tc - (fc * texelSize);

	// find number of blockers and sum up blocker depth
	for (row = -BFS2; row <= BFS2; row += 2)
	{
		for (col = -BFS2; col <= BFS2; col += 2)
		{
			float4 d4 = g_ShadowDepthTexture.GatherRed(PointSampler, tc.xy, int2(col, row));
			float4 b4 = (tc.zzzz <= d4) ? (0.0).xxxx : (1.0).xxxx;

			blockerCount += dot(b4, (1.0).xxxx);
			avgBlockerDepth += dot(d4, b4);
		}
	}

	// compute ratio using formulas from PCSS
	if (blockerCount > 0.0)
	{
		avgBlockerDepth /= blockerCount;
		fRatio = saturate(((tc.z - avgBlockerDepth) * SUN_WIDTH) / avgBlockerDepth);
		fRatio *= fRatio;
	}
	else
	{
		fRatio = 0.0;
	}

	// sum up weights of dynamic filter matrix
	for (row = 0; row < FS; ++row)
	{
		for (col = 0; col < FS; ++col)
		{
			w += Fw(row, col, fRatio);
		}
	}

	// filter shadow map samples using the dynamic weights
	[unroll(FILTER_SIZE)] for (row = -FS2; row <= FS2; row += 2)
	{
		for (col = -FS2; col <= FS2; col += 2)
		{
			v1[(col + FS2) / 2] = g_ShadowDepthTexture.GatherCmpRed(PointSamplerCmp, tc.xy, tc.z,
				int2(col, row));

			if (col == -FS2)
			{
				s += (1 - fc.y) * (v1[0].w * (Fw(row + FS2, 0, fRatio) -
					Fw(row + FS2, 0, fRatio) * fc.x) + v1[0].z *
					(fc.x * (Fw(row + FS2, 0, fRatio) -
						Fw(row + FS2, 1, fRatio)) +
						Fw(row + FS2, 1, fRatio)));
				s += (fc.y) * (v1[0].x * (Fw(row + FS2, 0, fRatio) -
					Fw(row + FS2, 0, fRatio) * fc.x) +
					v1[0].y * (fc.x * (Fw(row + FS2, 0, fRatio) -
						Fw(row + FS2, 1, fRatio)) +
						Fw(row + FS2, 1, fRatio)));
				if (row > -FS2)
				{
					s += (1 - fc.y) * (v0[0].x * (Fw(row + FS2 - 1, 0, fRatio) -
						Fw(row + FS2 - 1, 0, fRatio) * fc.x) + v0[0].y *
						(fc.x * (Fw(row + FS2 - 1, 0, fRatio) -
							Fw(row + FS2 - 1, 1, fRatio)) +
							Fw(row + FS2 - 1, 1, fRatio)));
					s += (fc.y) * (v1[0].w * (Fw(row + FS2 - 1, 0, fRatio) -
						Fw(row + FS2 - 1, 0, fRatio) * fc.x) + v1[0].z *
						(fc.x * (Fw(row + FS2 - 1, 0, fRatio) -
							Fw(row + FS2 - 1, 1, fRatio)) +
							Fw(row + FS2 - 1, 1, fRatio)));
				}
			}
			else if (col == FS2)
			{
				s += (1 - fc.y) * (v1[FS2].w * (fc.x * (Fw(row + FS2, FS - 2, fRatio) -
					Fw(row + FS2, FS - 1, fRatio)) +
					Fw(row + FS2, FS - 1, fRatio)) + v1[FS2].z * fc.x *
					Fw(row + FS2, FS - 1, fRatio));
				s += (fc.y) * (v1[FS2].x * (fc.x * (Fw(row + FS2, FS - 2, fRatio) -
					Fw(row + FS2, FS - 1, fRatio)) +
					Fw(row + FS2, FS - 1, fRatio)) + v1[FS2].y * fc.x *
					Fw(row + FS2, FS - 1, fRatio));
				if (row > -FS2)
				{
					s += (1 - fc.y) * (v0[FS2].x * (fc.x *
						(Fw(row + FS2 - 1, FS - 2, fRatio) -
							Fw(row + FS2 - 1, FS - 1, fRatio)) +
						Fw(row + FS2 - 1, FS - 1, fRatio)) +
						v0[FS2].y * fc.x * Fw(row + FS2 - 1, FS - 1, fRatio));
					s += (fc.y) * (v1[FS2].w * (fc.x *
						(Fw(row + FS2 - 1, FS - 2, fRatio) -
							Fw(row + FS2 - 1, FS - 1, fRatio)) +
						Fw(row + FS2 - 1, FS - 1, fRatio)) +
						v1[FS2].z * fc.x * Fw(row + FS2 - 1, FS - 1, fRatio));
				}
			}
			else
			{
				s += (1 - fc.y) * (v1[(col + FS2) / 2].w * (fc.x *
					(Fw(row + FS2, col + FS2 - 1, fRatio) -
						Fw(row + FS2, col + FS2 + 0, fRatio)) +
					Fw(row + FS2, col + FS2 + 0, fRatio)) +
					v1[(col + FS2) / 2].z * (fc.x *
						(Fw(row + FS2, col + FS2 - 0, fRatio) -
							Fw(row + FS2, col + FS2 + 1, fRatio)) +
						Fw(row + FS2, col + FS2 + 1, fRatio)));
				s += (fc.y) * (v1[(col + FS2) / 2].x * (fc.x *
					(Fw(row + FS2, col + FS2 - 1, fRatio) -
						Fw(row + FS2, col + FS2 + 0, fRatio)) +
					Fw(row + FS2, col + FS2 + 0, fRatio)) +
					v1[(col + FS2) / 2].y * (fc.x *
						(Fw(row + FS2, col + FS2 - 0, fRatio) -
							Fw(row + FS2, col + FS2 + 1, fRatio)) +
						Fw(row + FS2, col + FS2 + 1, fRatio)));
				if (row > -FS2)
				{
					s += (1 - fc.y) * (v0[(col + FS2) / 2].x * (fc.x *
						(Fw(row + FS2 - 1, col + FS2 - 1, fRatio) -
							Fw(row + FS2 - 1, col + FS2 + 0, fRatio)) +
						Fw(row + FS2 - 1, col + FS2 + 0, fRatio)) +
						v0[(col + FS2) / 2].y * (fc.x *
							(Fw(row + FS2 - 1, col + FS2 - 0, fRatio) -
								Fw(row + FS2 - 1, col + FS2 + 1, fRatio)) +
							Fw(row + FS2 - 1, col + FS2 + 1, fRatio)));
					s += (fc.y) * (v1[(col + FS2) / 2].w * (fc.x *
						(Fw(row + FS2 - 1, col + FS2 - 1, fRatio) -
							Fw(row + FS2 - 1, col + FS2 + 0, fRatio)) +
						Fw(row + FS2 - 1, col + FS2 + 0, fRatio)) +
						v1[(col + FS2) / 2].z * (fc.x *
							(Fw(row + FS2 - 1, col + FS2 - 0, fRatio) -
								Fw(row + FS2 - 1, col + FS2 + 1, fRatio)) +
							Fw(row + FS2 - 1, col + FS2 + 1, fRatio)));
				}
			}

			if (row != FS2)
			{
				v0[(col + FS2) / 2] = v1[(col + FS2) / 2].xy;
			}
		}
	}

	return s / w;
}

PS_OUT_LIGHT PS_MAIN_LIGHT_DIRECTIONAL(PS_IN In)
{
	PS_OUT_LIGHT			Out = (PS_OUT_LIGHT)0;

	vector	vNormalDesc = g_NormalTexture.Sample(LinearSampler, In.vTexUV);
	//vector	vDepthDesc = g_DepthTexture.Sample(LinearSampler, In.vTexUV);

	//float	fViewZ = vDepthDesc.y * g_CameraFar;
	//
	vector	vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);

	//vector	vWorldPos;

	//// uv좌표와 투영스페이스의 관계성으로 uv를 이용해서 투영의 xy 좌표를 만들고 z정보는 랜더타겟에서 가져옴
	//
	/////* 월드위치 * 뷰행렬 * 투영행렬 * 1/z */
	//vWorldPos.x = In.vTexUV.x * 2.f - 1.f;
	//vWorldPos.y = In.vTexUV.y * -2.f + 1.f;
	//vWorldPos.z = vDepthDesc.x;
	//vWorldPos.w = 1.f;

	/////* 월드위치 * 뷰행렬 * 투영행렬 */
	//vWorldPos = vWorldPos * fViewZ;

	/////* 월드위치 * 뷰행렬 */
	//vWorldPos = mul(vWorldPos, g_ProjMatrixInv);

	/////* 월드위치 */
	//vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

	//vector		vPosition;

	//vPosition = mul(vWorldPos, g_matLightView);

	//vector		vUVPos = mul(vPosition, g_matLightProj);
	//float2		vNewUV;

	//vNewUV.x = ((vUVPos.x / vUVPos.w) * 0.5f + 0.5f);
	//vNewUV.y = ((vUVPos.y / vUVPos.w) * -0.5f + 0.5f);

	//vector	vShadowDepthInfo = g_ShadowDepthTexture.Sample(ShadowDepthSampler, vNewUV);

	//float3 f3TC = float3(vNewUV, vPosition.z - 0.005f);

	//float fShadow = shadow(f3TC);

	Out.vShade = g_vLightDiffuse * (saturate(dot(normalize(g_vLightDir) * -1.f, vNormal)) +
		(g_vLightAmbient * g_vMtrlAmbient));

	Out.vShade.a = 1.f;

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
	float	fViewZ = vDepthDesc.y * g_CameraFar;

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
	if (brightness >= 0.8)
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


static const float Weights[15] =
{ 0.01f, 0.03f, 0.04f, 0.05f, 0.07f, 0.1f, 0.1f, 0.2f, 0.1f, 0.1f, 0.07f, 0.05f, 0.04f, 0.03f, 0.01f };

static const float Total = 1.f;

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

	for (int i = -7; i < 7; ++i)
	{
		uv = t + float2(tu * i, 0.f);
		vColor += Weights[7 + i] * g_BrightTexture.Sample(BlurSampler, uv);
	}

	vColor /= (Total);

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

	for (int i = -7; i < 7; ++i)
	{
		uv = t + float2(0.f, tv * i);
		vColor += Weights[7 + i] * g_BlurXTexture.Sample(BlurSampler, uv);
	}

	vColor /= (Total);

	Out.vBloomColor = vColor;

	return Out;
}

PS_OUT PS_MAIN_DEFERRED_BLOOM_BLEND(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	vector		vShade = g_ShadeTexture.Sample(LinearSampler, In.vTexUV);
	vector		vBloomOriginColor = g_BloomOriginTexture.Sample(LinearSampler, In.vTexUV);
	vector		vBloomColor = g_BloomTexture.Sample(LinearSampler, In.vTexUV);
	vector		vLaplacian = g_LaplacianTexture.Sample(LinearSampler, In.vTexUV);
	vector		vDepthDesc = g_DepthTexture.Sample(LinearSampler, In.vTexUV);

	Out.vColor = vDiffuse * vShade;

	if (Out.vColor.a == 0.f)
		discard;

	vector	vWorldPos;

	// uv좌표와 투영스페이스의 관계성으로 uv를 이용해서 투영의 xy 좌표를 만들고 z정보는 랜더타겟에서 가져옴
	float	fViewZ = vDepthDesc.y * g_CameraFar;

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

	vector		vPosition;

	vPosition = mul(vWorldPos, g_matLightView);

	vector		vUVPos = mul(vPosition, g_matLightProj);
	float2		vNewUV;

	vNewUV.x = ((vUVPos.x / vUVPos.w) * 0.5f + 0.5f);
	vNewUV.y = ((vUVPos.y / vUVPos.w) * -0.5f + 0.5f);

	//float3 f3TC = float3(vNewUV, (vUVPos.z / vUVPos.w) - 0.005f);

	//float fShadow = shadow(f3TC);
	//
	//Out.vColor = Out.vColor * fShadow;

	vector		vShadowDepthInfo = g_ShadowDepthTexture.Sample(ShadowDepthSampler, vNewUV);

	if (vPosition.z - 0.1f > vShadowDepthInfo.r * g_CameraFar)
		Out.vColor = float4(0.15f, 0.15f, 0.15f, 1.f);

	if (g_bLaplacian.a >= 0.5f)
	{
		if (vLaplacian.a >= 0.49f)
		{
			Out.vColor = vLaplacian;
		}
	}

	if (vBloomOriginColor.a != 0.f)
	{
		Out.vColor = vBloomOriginColor;
		float4		vBloom = pow(pow(abs(vBloomColor), 2.2f) + pow(abs(vBloomOriginColor), 2.2f), 1.f / 2.2f);

		Out.vColor = pow(abs(Out.vColor), 2.2f);;
		vBloom = pow(abs(vBloom), 2.2f);

		Out.vColor += vBloom;
		Out.vColor = pow(abs(Out.vColor), 1 / 2.2f);
	}

	return Out;
}


PS_OUT PS_MAIN_DEFERRED_GRAY(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;
	
	vector		vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	vector		vShade = g_ShadeTexture.Sample(LinearSampler, In.vTexUV);

	Out.vColor = vDiffuse * vShade;

	if (Out.vColor.a == 0.f)
	{
		discard;
	}

	float gray = dot(vDiffuse.rgb, normalize(float3(1, -1, 1)));

	Out.vColor = float4(gray, gray, gray, 0.f);

	return Out;
}

float3x3 Laplaciankernel1 = float3x3(
	0, -1, 0,
	-1, 4, -1,
	0, -1, 0);

float3x3 Laplaciankernel2 = float3x3(
	0, -1, 0,
	-1, 8, -1,
	0, -1, 0);

float3x3 Laplaciankernel3 = float3x3(
	-1, -1, -1,
	-1, 4, -1,
	-1, -1, -1);

float3 convolve(float3x3 kernel, texture2D tex, float2 uv)
{
	float3 result = 0;
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			result += tex.Sample(LinearSampler, uv + float2(i / (g_TexSize.x), j/ (g_TexSize.y))) * kernel[i + 1][j + 1];
		}
	}
	return result;
}

PS_OUT PS_MAIN_DEFERRED_LAPLACIAN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	//vector	vColor = g_BlurYTexture.Sample(LinearSampler, In.vTexUV);
	vector		vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
	vector		vShade = g_ShadeTexture.Sample(LinearSampler, In.vTexUV);

	Out.vColor = vDiffuse * vShade;

	float3 result = convolve(Laplaciankernel1, g_BlurYTexture, In.vTexUV);

	float edgeStrength = length(result);

	float4 outputColor;
	if (edgeStrength > g_laplacianThesholdLow)
	{
		outputColor = float4(0, 0, 0, 1); // set pixel to black (non-edge)
	}
	else if (edgeStrength > 0.10)
	{
		outputColor = float4(0.1, 0.1, 0.1, 1); // set pixel to white (strong edge)
	}
	else if (edgeStrength > g_laplacianThesholdHigh)
	{
		outputColor = float4(0, 0, 0, 0); // set pixel to gray (weak edge)
	}
	else
	{
		outputColor = float4(0, 0, 0, 0); // set pixel to black (non-edge)
	}


	Out.vColor = outputColor;

	return Out;
}

PS_OUT PS_MAIN_DEFERRED_SHADOWDEPTH(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	vector	vDepthDesc = g_DepthTexture.Sample(LinearSampler, In.vTexUV);

	if (vDepthDesc.z < 1.f)
	{
		Out.vColor = float4(1.f, 1.f, 1.f, 0.f);
	}
	else
		Out.vColor = float4(0.f, 0.f, 0.f, 1.f);

	//if (vPos.z - 0.1f > )

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
	pass Debug // 0
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

	pass LightAcc_Directional // 1
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

	pass LightAcc_Point  // 2
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

	pass Deferred_Blend  // 3
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

	pass Deferred_Bright  // 4
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

	pass Deferred_BlurX  // 5
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

	pass Deferred_BlurY  // 6
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

	pass Deferred_Bloom_Blend  // 7
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

	pass Deferred_Gray  // 8
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Not_ZTest_Not_ZWrite, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DEFERRED_GRAY();
	}

	pass Deferred_Laplacian  // 9
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Not_ZTest_Not_ZWrite, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DEFERRED_LAPLACIAN();
	}


	pass Deferred_ShadowDepth  // 10
	{
		SetRasterizerState(RS_Default);
		SetDepthStencilState(DSS_Not_ZTest_Not_ZWrite, 0);
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DEFERRED_SHADOWDEPTH();
	}
}