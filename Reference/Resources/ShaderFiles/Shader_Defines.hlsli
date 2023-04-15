#ifndef PI 
#define PI 3.141592f
#endif

#ifndef ONE_OVER_PI
#define ONE_OVER_PI 0.318309f
#endif

/* For.Sampler State  */
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

/* For.Rasterizer State  */
RasterizerState RS_Default
{
	FillMode = solid;
};

RasterizerState RS_Wireframe
{
	FillMode = wireframe;
};

RasterizerState RS_CullMode
{
	FillMode = wireframe;
	CullMode = none;
};

/* For.DepthStencil State  */
DepthStencilState DSS_Default
{
	DepthEnable = true;
	DepthWriteMask = all;
	DepthFunc = less_equal;
};

DepthStencilState DSS_Disable_ZTest_Disable_ZWrite
{
	DepthEnable = false;
	DepthWriteMask = zero;
};

DepthStencilState DSS_Enable_ZTest_Disable_ZWrite
{
	DepthEnable = true;
	DepthWriteMask = zero;
};

/* For.Blend State  */
BlendState BS_Default
{
	BlendEnable[0] = false;
};

BlendState BS_AlphaBlend
{
	BlendEnable[0] = true;
	BlendEnable[1] = true;

	SrcBlend = src_alpha;
	DestBlend = inv_src_alpha;
	BlendOp = add;
};


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

float RoundedRectSDF(float2 centerPosition, float2 size, float radius)
{
	return length(max(abs(centerPosition) - (size / 2) + radius, 0)) - radius;
}

float roundedBoxSDF(float2 CenterPosition, float2 Size, float Radius) {
	return length(max(abs(CenterPosition) - Size + Radius, 0.0)) - Radius;
}

float CalculateClockAngle_float(float2 uv)
{
	float2 a = float2(0.f, 1.f);
	float2 b = normalize(uv - float2(0.5f, 0.5f));

	float dot = (a.x * b.x) - (a.y * b.y);
	float det = (a.x * b.y) + (a.y * b.x);
	float angle = atan2(-det, -dot);

	return ((angle + PI) * 0.5f) * ONE_OVER_PI;
}