//////////////////////////////////////////
//
// NOTE: This is *not* a valid shader file
//
///////////////////////////////////////////
Shader "Hidden/FXAA II" {
Properties {
_MainTex ("Base (RGB)", 2D) = "white" { }
}
SubShader {
 Pass {
  ZTest Always
  ZWrite Off
  Cull Off
  GpuProgramID 795
Program "vp" {
SubProgram "gles hw_tier00 " {
"#ifdef VERTEX
#version 100

uniform 	vec4 hlslcc_mtx4x4unity_ObjectToWorld[4];
uniform 	vec4 hlslcc_mtx4x4unity_MatrixVP[4];
uniform 	vec4 _MainTex_TexelSize;
attribute highp vec4 in_POSITION0;
attribute mediump vec2 in_TEXCOORD0;
varying highp vec4 vs_TEXCOORD0;
vec4 u_xlat0;
vec4 u_xlat1;
mediump vec2 u_xlat16_2;
void main()
{
    u_xlat0 = in_POSITION0.yyyy * hlslcc_mtx4x4unity_ObjectToWorld[1];
    u_xlat0 = hlslcc_mtx4x4unity_ObjectToWorld[0] * in_POSITION0.xxxx + u_xlat0;
    u_xlat0 = hlslcc_mtx4x4unity_ObjectToWorld[2] * in_POSITION0.zzzz + u_xlat0;
    u_xlat0 = u_xlat0 + hlslcc_mtx4x4unity_ObjectToWorld[3];
    u_xlat1 = u_xlat0.yyyy * hlslcc_mtx4x4unity_MatrixVP[1];
    u_xlat1 = hlslcc_mtx4x4unity_MatrixVP[0] * u_xlat0.xxxx + u_xlat1;
    u_xlat1 = hlslcc_mtx4x4unity_MatrixVP[2] * u_xlat0.zzzz + u_xlat1;
    gl_Position = hlslcc_mtx4x4unity_MatrixVP[3] * u_xlat0.wwww + u_xlat1;
    u_xlat16_2.xy = in_TEXCOORD0.xy * vec2(2.0, 2.0) + vec2(-1.0, -1.0);
    u_xlat0.xy = u_xlat16_2.xy * vec2(0.5, 0.5) + vec2(0.5, 0.5);
    vs_TEXCOORD0.zw = (-_MainTex_TexelSize.xy) * vec2(0.75, 0.75) + u_xlat0.xy;
    vs_TEXCOORD0.xy = u_xlat0.xy;
    return;
}

#endif
#ifdef FRAGMENT
#version 100
#ifdef GL_EXT_shader_texture_lod
#extension GL_EXT_shader_texture_lod : enable
#endif
#if !defined(GL_EXT_shader_texture_lod)
#define texture1DLodEXT texture1D
#define texture2DLodEXT texture2D
#define texture2DProjLodEXT texture2DProj
#define texture3DLodEXT texture3D
#define textureCubeLodEXT textureCube
#endif

#ifdef GL_FRAGMENT_PRECISION_HIGH
    precision highp float;
#else
    precision mediump float;
#endif
precision highp int;
uniform 	mediump vec4 _MainTex_ST;
uniform 	vec4 _MainTex_TexelSize;
uniform lowp sampler2D _MainTex;
varying highp vec4 vs_TEXCOORD0;
#define SV_Target0 gl_FragData[0]
vec4 u_xlat0;
lowp vec3 u_xlat10_0;
bvec2 u_xlatb0;
vec4 u_xlat1;
lowp vec3 u_xlat10_1;
vec4 u_xlat2;
lowp vec3 u_xlat10_2;
vec4 u_xlat3;
lowp vec3 u_xlat10_3;
lowp vec3 u_xlat10_4;
float u_xlat5;
float u_xlat6;
vec2 u_xlat10;
float u_xlat11;
float u_xlat15;
float u_xlat16;
void main()
{
    u_xlat0 = _MainTex_TexelSize.xyxy * vec4(1.0, 0.0, 0.0, 1.0) + vs_TEXCOORD0.zwzw;
    u_xlat0 = u_xlat0 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_1.xyz = texture2DLodEXT(_MainTex, u_xlat0.xy, 0.0).xyz;
    u_xlat10_0.xyz = texture2DLodEXT(_MainTex, u_xlat0.zw, 0.0).xyz;
    u_xlat0.x = dot(u_xlat10_0.xyz, vec3(0.298999995, 0.587000012, 0.114));
    u_xlat5 = dot(u_xlat10_1.xyz, vec3(0.298999995, 0.587000012, 0.114));
    u_xlat10.xy = vs_TEXCOORD0.zw + _MainTex_TexelSize.xy;
    u_xlat10.xy = u_xlat10.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_1.xyz = texture2DLodEXT(_MainTex, u_xlat10.xy, 0.0).xyz;
    u_xlat10.x = dot(u_xlat10_1.xyz, vec3(0.298999995, 0.587000012, 0.114));
    u_xlat15 = u_xlat10.x + u_xlat5;
    u_xlat1 = vs_TEXCOORD0.zwxy * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_2.xyz = texture2DLodEXT(_MainTex, u_xlat1.xy, 0.0).xyz;
    u_xlat10_1.xyz = texture2DLodEXT(_MainTex, u_xlat1.zw, 0.0).xyz;
    u_xlat1.x = dot(u_xlat10_1.xyz, vec3(0.298999995, 0.587000012, 0.114));
    u_xlat6 = dot(u_xlat10_2.xyz, vec3(0.298999995, 0.587000012, 0.114));
    u_xlat11 = u_xlat0.x + u_xlat6;
    u_xlat2.yw = (-vec2(u_xlat15)) + vec2(u_xlat11);
    u_xlat15 = u_xlat5 + u_xlat6;
    u_xlat11 = u_xlat10.x + u_xlat0.x;
    u_xlat11 = u_xlat15 + (-u_xlat11);
    u_xlat15 = u_xlat0.x + u_xlat15;
    u_xlat15 = u_xlat10.x + u_xlat15;
    u_xlat15 = u_xlat15 * 0.03125;
    u_xlat15 = max(u_xlat15, 0.0078125);
    u_xlat16 = min(abs(u_xlat2.w), abs(u_xlat11));
    u_xlat2.xz = (-vec2(u_xlat11));
    u_xlat15 = u_xlat15 + u_xlat16;
    u_xlat15 = float(1.0) / u_xlat15;
    u_xlat2 = vec4(u_xlat15) * u_xlat2;
    u_xlat2 = max(u_xlat2, vec4(-8.0, -8.0, -8.0, -8.0));
    u_xlat2 = min(u_xlat2, vec4(8.0, 8.0, 8.0, 8.0));
    u_xlat2 = u_xlat2 * _MainTex_TexelSize.xyxy;
    u_xlat3 = u_xlat2 * vec4(-0.5, -0.5, 0.5, 0.5) + vs_TEXCOORD0.xyxy;
    u_xlat2 = u_xlat2.zwzw * vec4(-0.166666672, -0.166666672, 0.166666672, 0.166666672) + vs_TEXCOORD0.xyxy;
    u_xlat2 = u_xlat2 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat3 = u_xlat3 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_4.xyz = texture2DLodEXT(_MainTex, u_xlat3.xy, 0.0).xyz;
    u_xlat10_3.xyz = texture2DLodEXT(_MainTex, u_xlat3.zw, 0.0).xyz;
    u_xlat3.xyz = u_xlat10_3.xyz + u_xlat10_4.xyz;
    u_xlat3.xyz = u_xlat3.xyz * vec3(0.25, 0.25, 0.25);
    u_xlat10_4.xyz = texture2DLodEXT(_MainTex, u_xlat2.xy, 0.0).xyz;
    u_xlat10_2.xyz = texture2DLodEXT(_MainTex, u_xlat2.zw, 0.0).xyz;
    u_xlat2.xyz = u_xlat10_2.xyz + u_xlat10_4.xyz;
    u_xlat3.xyz = u_xlat2.xyz * vec3(0.25, 0.25, 0.25) + u_xlat3.xyz;
    u_xlat2.xyz = u_xlat2.xyz * vec3(0.5, 0.5, 0.5);
    u_xlat0.w = dot(u_xlat3.xyz, vec3(0.298999995, 0.587000012, 0.114));
    u_xlat11 = min(u_xlat10.x, u_xlat0.x);
    u_xlat0.x = max(u_xlat10.x, u_xlat0.x);
    u_xlat10.x = min(u_xlat5, u_xlat6);
    u_xlat5 = max(u_xlat5, u_xlat6);
    u_xlat0.x = max(u_xlat0.x, u_xlat5);
    u_xlat0.x = max(u_xlat0.x, u_xlat1.x);
    u_xlat5 = min(u_xlat11, u_xlat10.x);
    u_xlat0.y = min(u_xlat5, u_xlat1.x);
    u_xlatb0.xy = lessThan(u_xlat0.xwxx, u_xlat0.wyww).xy;
    u_xlatb0.x = u_xlatb0.x || u_xlatb0.y;
    SV_Target0.xyz = (u_xlatb0.x) ? u_xlat2.xyz : u_xlat3.xyz;
    SV_Target0.w = 0.0;
    return;
}

#endif
"
}
SubProgram "gles hw_tier01 " {
"#ifdef VERTEX
#version 100

uniform 	vec4 hlslcc_mtx4x4unity_ObjectToWorld[4];
uniform 	vec4 hlslcc_mtx4x4unity_MatrixVP[4];
uniform 	vec4 _MainTex_TexelSize;
attribute highp vec4 in_POSITION0;
attribute mediump vec2 in_TEXCOORD0;
varying highp vec4 vs_TEXCOORD0;
vec4 u_xlat0;
vec4 u_xlat1;
mediump vec2 u_xlat16_2;
void main()
{
    u_xlat0 = in_POSITION0.yyyy * hlslcc_mtx4x4unity_ObjectToWorld[1];
    u_xlat0 = hlslcc_mtx4x4unity_ObjectToWorld[0] * in_POSITION0.xxxx + u_xlat0;
    u_xlat0 = hlslcc_mtx4x4unity_ObjectToWorld[2] * in_POSITION0.zzzz + u_xlat0;
    u_xlat0 = u_xlat0 + hlslcc_mtx4x4unity_ObjectToWorld[3];
    u_xlat1 = u_xlat0.yyyy * hlslcc_mtx4x4unity_MatrixVP[1];
    u_xlat1 = hlslcc_mtx4x4unity_MatrixVP[0] * u_xlat0.xxxx + u_xlat1;
    u_xlat1 = hlslcc_mtx4x4unity_MatrixVP[2] * u_xlat0.zzzz + u_xlat1;
    gl_Position = hlslcc_mtx4x4unity_MatrixVP[3] * u_xlat0.wwww + u_xlat1;
    u_xlat16_2.xy = in_TEXCOORD0.xy * vec2(2.0, 2.0) + vec2(-1.0, -1.0);
    u_xlat0.xy = u_xlat16_2.xy * vec2(0.5, 0.5) + vec2(0.5, 0.5);
    vs_TEXCOORD0.zw = (-_MainTex_TexelSize.xy) * vec2(0.75, 0.75) + u_xlat0.xy;
    vs_TEXCOORD0.xy = u_xlat0.xy;
    return;
}

#endif
#ifdef FRAGMENT
#version 100
#ifdef GL_EXT_shader_texture_lod
#extension GL_EXT_shader_texture_lod : enable
#endif
#if !defined(GL_EXT_shader_texture_lod)
#define texture1DLodEXT texture1D
#define texture2DLodEXT texture2D
#define texture2DProjLodEXT texture2DProj
#define texture3DLodEXT texture3D
#define textureCubeLodEXT textureCube
#endif

#ifdef GL_FRAGMENT_PRECISION_HIGH
    precision highp float;
#else
    precision mediump float;
#endif
precision highp int;
uniform 	mediump vec4 _MainTex_ST;
uniform 	vec4 _MainTex_TexelSize;
uniform lowp sampler2D _MainTex;
varying highp vec4 vs_TEXCOORD0;
#define SV_Target0 gl_FragData[0]
vec4 u_xlat0;
lowp vec3 u_xlat10_0;
bvec2 u_xlatb0;
vec4 u_xlat1;
lowp vec3 u_xlat10_1;
vec4 u_xlat2;
lowp vec3 u_xlat10_2;
vec4 u_xlat3;
lowp vec3 u_xlat10_3;
lowp vec3 u_xlat10_4;
float u_xlat5;
float u_xlat6;
vec2 u_xlat10;
float u_xlat11;
float u_xlat15;
float u_xlat16;
void main()
{
    u_xlat0 = _MainTex_TexelSize.xyxy * vec4(1.0, 0.0, 0.0, 1.0) + vs_TEXCOORD0.zwzw;
    u_xlat0 = u_xlat0 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_1.xyz = texture2DLodEXT(_MainTex, u_xlat0.xy, 0.0).xyz;
    u_xlat10_0.xyz = texture2DLodEXT(_MainTex, u_xlat0.zw, 0.0).xyz;
    u_xlat0.x = dot(u_xlat10_0.xyz, vec3(0.298999995, 0.587000012, 0.114));
    u_xlat5 = dot(u_xlat10_1.xyz, vec3(0.298999995, 0.587000012, 0.114));
    u_xlat10.xy = vs_TEXCOORD0.zw + _MainTex_TexelSize.xy;
    u_xlat10.xy = u_xlat10.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_1.xyz = texture2DLodEXT(_MainTex, u_xlat10.xy, 0.0).xyz;
    u_xlat10.x = dot(u_xlat10_1.xyz, vec3(0.298999995, 0.587000012, 0.114));
    u_xlat15 = u_xlat10.x + u_xlat5;
    u_xlat1 = vs_TEXCOORD0.zwxy * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_2.xyz = texture2DLodEXT(_MainTex, u_xlat1.xy, 0.0).xyz;
    u_xlat10_1.xyz = texture2DLodEXT(_MainTex, u_xlat1.zw, 0.0).xyz;
    u_xlat1.x = dot(u_xlat10_1.xyz, vec3(0.298999995, 0.587000012, 0.114));
    u_xlat6 = dot(u_xlat10_2.xyz, vec3(0.298999995, 0.587000012, 0.114));
    u_xlat11 = u_xlat0.x + u_xlat6;
    u_xlat2.yw = (-vec2(u_xlat15)) + vec2(u_xlat11);
    u_xlat15 = u_xlat5 + u_xlat6;
    u_xlat11 = u_xlat10.x + u_xlat0.x;
    u_xlat11 = u_xlat15 + (-u_xlat11);
    u_xlat15 = u_xlat0.x + u_xlat15;
    u_xlat15 = u_xlat10.x + u_xlat15;
    u_xlat15 = u_xlat15 * 0.03125;
    u_xlat15 = max(u_xlat15, 0.0078125);
    u_xlat16 = min(abs(u_xlat2.w), abs(u_xlat11));
    u_xlat2.xz = (-vec2(u_xlat11));
    u_xlat15 = u_xlat15 + u_xlat16;
    u_xlat15 = float(1.0) / u_xlat15;
    u_xlat2 = vec4(u_xlat15) * u_xlat2;
    u_xlat2 = max(u_xlat2, vec4(-8.0, -8.0, -8.0, -8.0));
    u_xlat2 = min(u_xlat2, vec4(8.0, 8.0, 8.0, 8.0));
    u_xlat2 = u_xlat2 * _MainTex_TexelSize.xyxy;
    u_xlat3 = u_xlat2 * vec4(-0.5, -0.5, 0.5, 0.5) + vs_TEXCOORD0.xyxy;
    u_xlat2 = u_xlat2.zwzw * vec4(-0.166666672, -0.166666672, 0.166666672, 0.166666672) + vs_TEXCOORD0.xyxy;
    u_xlat2 = u_xlat2 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat3 = u_xlat3 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_4.xyz = texture2DLodEXT(_MainTex, u_xlat3.xy, 0.0).xyz;
    u_xlat10_3.xyz = texture2DLodEXT(_MainTex, u_xlat3.zw, 0.0).xyz;
    u_xlat3.xyz = u_xlat10_3.xyz + u_xlat10_4.xyz;
    u_xlat3.xyz = u_xlat3.xyz * vec3(0.25, 0.25, 0.25);
    u_xlat10_4.xyz = texture2DLodEXT(_MainTex, u_xlat2.xy, 0.0).xyz;
    u_xlat10_2.xyz = texture2DLodEXT(_MainTex, u_xlat2.zw, 0.0).xyz;
    u_xlat2.xyz = u_xlat10_2.xyz + u_xlat10_4.xyz;
    u_xlat3.xyz = u_xlat2.xyz * vec3(0.25, 0.25, 0.25) + u_xlat3.xyz;
    u_xlat2.xyz = u_xlat2.xyz * vec3(0.5, 0.5, 0.5);
    u_xlat0.w = dot(u_xlat3.xyz, vec3(0.298999995, 0.587000012, 0.114));
    u_xlat11 = min(u_xlat10.x, u_xlat0.x);
    u_xlat0.x = max(u_xlat10.x, u_xlat0.x);
    u_xlat10.x = min(u_xlat5, u_xlat6);
    u_xlat5 = max(u_xlat5, u_xlat6);
    u_xlat0.x = max(u_xlat0.x, u_xlat5);
    u_xlat0.x = max(u_xlat0.x, u_xlat1.x);
    u_xlat5 = min(u_xlat11, u_xlat10.x);
    u_xlat0.y = min(u_xlat5, u_xlat1.x);
    u_xlatb0.xy = lessThan(u_xlat0.xwxx, u_xlat0.wyww).xy;
    u_xlatb0.x = u_xlatb0.x || u_xlatb0.y;
    SV_Target0.xyz = (u_xlatb0.x) ? u_xlat2.xyz : u_xlat3.xyz;
    SV_Target0.w = 0.0;
    return;
}

#endif
"
}
SubProgram "gles hw_tier02 " {
"#ifdef VERTEX
#version 100

uniform 	vec4 hlslcc_mtx4x4unity_ObjectToWorld[4];
uniform 	vec4 hlslcc_mtx4x4unity_MatrixVP[4];
uniform 	vec4 _MainTex_TexelSize;
attribute highp vec4 in_POSITION0;
attribute mediump vec2 in_TEXCOORD0;
varying highp vec4 vs_TEXCOORD0;
vec4 u_xlat0;
vec4 u_xlat1;
mediump vec2 u_xlat16_2;
void main()
{
    u_xlat0 = in_POSITION0.yyyy * hlslcc_mtx4x4unity_ObjectToWorld[1];
    u_xlat0 = hlslcc_mtx4x4unity_ObjectToWorld[0] * in_POSITION0.xxxx + u_xlat0;
    u_xlat0 = hlslcc_mtx4x4unity_ObjectToWorld[2] * in_POSITION0.zzzz + u_xlat0;
    u_xlat0 = u_xlat0 + hlslcc_mtx4x4unity_ObjectToWorld[3];
    u_xlat1 = u_xlat0.yyyy * hlslcc_mtx4x4unity_MatrixVP[1];
    u_xlat1 = hlslcc_mtx4x4unity_MatrixVP[0] * u_xlat0.xxxx + u_xlat1;
    u_xlat1 = hlslcc_mtx4x4unity_MatrixVP[2] * u_xlat0.zzzz + u_xlat1;
    gl_Position = hlslcc_mtx4x4unity_MatrixVP[3] * u_xlat0.wwww + u_xlat1;
    u_xlat16_2.xy = in_TEXCOORD0.xy * vec2(2.0, 2.0) + vec2(-1.0, -1.0);
    u_xlat0.xy = u_xlat16_2.xy * vec2(0.5, 0.5) + vec2(0.5, 0.5);
    vs_TEXCOORD0.zw = (-_MainTex_TexelSize.xy) * vec2(0.75, 0.75) + u_xlat0.xy;
    vs_TEXCOORD0.xy = u_xlat0.xy;
    return;
}

#endif
#ifdef FRAGMENT
#version 100
#ifdef GL_EXT_shader_texture_lod
#extension GL_EXT_shader_texture_lod : enable
#endif
#if !defined(GL_EXT_shader_texture_lod)
#define texture1DLodEXT texture1D
#define texture2DLodEXT texture2D
#define texture2DProjLodEXT texture2DProj
#define texture3DLodEXT texture3D
#define textureCubeLodEXT textureCube
#endif

#ifdef GL_FRAGMENT_PRECISION_HIGH
    precision highp float;
#else
    precision mediump float;
#endif
precision highp int;
uniform 	mediump vec4 _MainTex_ST;
uniform 	vec4 _MainTex_TexelSize;
uniform lowp sampler2D _MainTex;
varying highp vec4 vs_TEXCOORD0;
#define SV_Target0 gl_FragData[0]
vec4 u_xlat0;
lowp vec3 u_xlat10_0;
bvec2 u_xlatb0;
vec4 u_xlat1;
lowp vec3 u_xlat10_1;
vec4 u_xlat2;
lowp vec3 u_xlat10_2;
vec4 u_xlat3;
lowp vec3 u_xlat10_3;
lowp vec3 u_xlat10_4;
float u_xlat5;
float u_xlat6;
vec2 u_xlat10;
float u_xlat11;
float u_xlat15;
float u_xlat16;
void main()
{
    u_xlat0 = _MainTex_TexelSize.xyxy * vec4(1.0, 0.0, 0.0, 1.0) + vs_TEXCOORD0.zwzw;
    u_xlat0 = u_xlat0 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_1.xyz = texture2DLodEXT(_MainTex, u_xlat0.xy, 0.0).xyz;
    u_xlat10_0.xyz = texture2DLodEXT(_MainTex, u_xlat0.zw, 0.0).xyz;
    u_xlat0.x = dot(u_xlat10_0.xyz, vec3(0.298999995, 0.587000012, 0.114));
    u_xlat5 = dot(u_xlat10_1.xyz, vec3(0.298999995, 0.587000012, 0.114));
    u_xlat10.xy = vs_TEXCOORD0.zw + _MainTex_TexelSize.xy;
    u_xlat10.xy = u_xlat10.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_1.xyz = texture2DLodEXT(_MainTex, u_xlat10.xy, 0.0).xyz;
    u_xlat10.x = dot(u_xlat10_1.xyz, vec3(0.298999995, 0.587000012, 0.114));
    u_xlat15 = u_xlat10.x + u_xlat5;
    u_xlat1 = vs_TEXCOORD0.zwxy * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_2.xyz = texture2DLodEXT(_MainTex, u_xlat1.xy, 0.0).xyz;
    u_xlat10_1.xyz = texture2DLodEXT(_MainTex, u_xlat1.zw, 0.0).xyz;
    u_xlat1.x = dot(u_xlat10_1.xyz, vec3(0.298999995, 0.587000012, 0.114));
    u_xlat6 = dot(u_xlat10_2.xyz, vec3(0.298999995, 0.587000012, 0.114));
    u_xlat11 = u_xlat0.x + u_xlat6;
    u_xlat2.yw = (-vec2(u_xlat15)) + vec2(u_xlat11);
    u_xlat15 = u_xlat5 + u_xlat6;
    u_xlat11 = u_xlat10.x + u_xlat0.x;
    u_xlat11 = u_xlat15 + (-u_xlat11);
    u_xlat15 = u_xlat0.x + u_xlat15;
    u_xlat15 = u_xlat10.x + u_xlat15;
    u_xlat15 = u_xlat15 * 0.03125;
    u_xlat15 = max(u_xlat15, 0.0078125);
    u_xlat16 = min(abs(u_xlat2.w), abs(u_xlat11));
    u_xlat2.xz = (-vec2(u_xlat11));
    u_xlat15 = u_xlat15 + u_xlat16;
    u_xlat15 = float(1.0) / u_xlat15;
    u_xlat2 = vec4(u_xlat15) * u_xlat2;
    u_xlat2 = max(u_xlat2, vec4(-8.0, -8.0, -8.0, -8.0));
    u_xlat2 = min(u_xlat2, vec4(8.0, 8.0, 8.0, 8.0));
    u_xlat2 = u_xlat2 * _MainTex_TexelSize.xyxy;
    u_xlat3 = u_xlat2 * vec4(-0.5, -0.5, 0.5, 0.5) + vs_TEXCOORD0.xyxy;
    u_xlat2 = u_xlat2.zwzw * vec4(-0.166666672, -0.166666672, 0.166666672, 0.166666672) + vs_TEXCOORD0.xyxy;
    u_xlat2 = u_xlat2 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat3 = u_xlat3 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_4.xyz = texture2DLodEXT(_MainTex, u_xlat3.xy, 0.0).xyz;
    u_xlat10_3.xyz = texture2DLodEXT(_MainTex, u_xlat3.zw, 0.0).xyz;
    u_xlat3.xyz = u_xlat10_3.xyz + u_xlat10_4.xyz;
    u_xlat3.xyz = u_xlat3.xyz * vec3(0.25, 0.25, 0.25);
    u_xlat10_4.xyz = texture2DLodEXT(_MainTex, u_xlat2.xy, 0.0).xyz;
    u_xlat10_2.xyz = texture2DLodEXT(_MainTex, u_xlat2.zw, 0.0).xyz;
    u_xlat2.xyz = u_xlat10_2.xyz + u_xlat10_4.xyz;
    u_xlat3.xyz = u_xlat2.xyz * vec3(0.25, 0.25, 0.25) + u_xlat3.xyz;
    u_xlat2.xyz = u_xlat2.xyz * vec3(0.5, 0.5, 0.5);
    u_xlat0.w = dot(u_xlat3.xyz, vec3(0.298999995, 0.587000012, 0.114));
    u_xlat11 = min(u_xlat10.x, u_xlat0.x);
    u_xlat0.x = max(u_xlat10.x, u_xlat0.x);
    u_xlat10.x = min(u_xlat5, u_xlat6);
    u_xlat5 = max(u_xlat5, u_xlat6);
    u_xlat0.x = max(u_xlat0.x, u_xlat5);
    u_xlat0.x = max(u_xlat0.x, u_xlat1.x);
    u_xlat5 = min(u_xlat11, u_xlat10.x);
    u_xlat0.y = min(u_xlat5, u_xlat1.x);
    u_xlatb0.xy = lessThan(u_xlat0.xwxx, u_xlat0.wyww).xy;
    u_xlatb0.x = u_xlatb0.x || u_xlatb0.y;
    SV_Target0.xyz = (u_xlatb0.x) ? u_xlat2.xyz : u_xlat3.xyz;
    SV_Target0.w = 0.0;
    return;
}

#endif
"
}
SubProgram "gles3 hw_tier00 " {
"#ifdef VERTEX
#version 300 es

uniform 	vec4 hlslcc_mtx4x4unity_ObjectToWorld[4];
uniform 	vec4 hlslcc_mtx4x4unity_MatrixVP[4];
uniform 	vec4 _MainTex_TexelSize;
in highp vec4 in_POSITION0;
in mediump vec2 in_TEXCOORD0;
out highp vec4 vs_TEXCOORD0;
vec4 u_xlat0;
vec4 u_xlat1;
mediump vec2 u_xlat16_2;
void main()
{
    u_xlat0 = in_POSITION0.yyyy * hlslcc_mtx4x4unity_ObjectToWorld[1];
    u_xlat0 = hlslcc_mtx4x4unity_ObjectToWorld[0] * in_POSITION0.xxxx + u_xlat0;
    u_xlat0 = hlslcc_mtx4x4unity_ObjectToWorld[2] * in_POSITION0.zzzz + u_xlat0;
    u_xlat0 = u_xlat0 + hlslcc_mtx4x4unity_ObjectToWorld[3];
    u_xlat1 = u_xlat0.yyyy * hlslcc_mtx4x4unity_MatrixVP[1];
    u_xlat1 = hlslcc_mtx4x4unity_MatrixVP[0] * u_xlat0.xxxx + u_xlat1;
    u_xlat1 = hlslcc_mtx4x4unity_MatrixVP[2] * u_xlat0.zzzz + u_xlat1;
    gl_Position = hlslcc_mtx4x4unity_MatrixVP[3] * u_xlat0.wwww + u_xlat1;
    u_xlat16_2.xy = in_TEXCOORD0.xy * vec2(2.0, 2.0) + vec2(-1.0, -1.0);
    u_xlat0.xy = u_xlat16_2.xy * vec2(0.5, 0.5) + vec2(0.5, 0.5);
    vs_TEXCOORD0.zw = (-_MainTex_TexelSize.xy) * vec2(0.75, 0.75) + u_xlat0.xy;
    vs_TEXCOORD0.xy = u_xlat0.xy;
    return;
}

#endif
#ifdef FRAGMENT
#version 300 es
#ifdef GL_EXT_shader_texture_lod
#extension GL_EXT_shader_texture_lod : enable
#endif

precision highp float;
precision highp int;
uniform 	mediump vec4 _MainTex_ST;
uniform 	vec4 _MainTex_TexelSize;
uniform mediump sampler2D _MainTex;
in highp vec4 vs_TEXCOORD0;
layout(location = 0) out highp vec4 SV_Target0;
vec4 u_xlat0;
mediump vec3 u_xlat16_0;
bvec2 u_xlatb0;
vec4 u_xlat1;
mediump vec3 u_xlat16_1;
vec4 u_xlat2;
mediump vec3 u_xlat16_2;
vec4 u_xlat3;
mediump vec3 u_xlat16_3;
mediump vec3 u_xlat16_4;
float u_xlat5;
float u_xlat6;
vec2 u_xlat10;
float u_xlat11;
float u_xlat15;
float u_xlat16;
void main()
{
    u_xlat0 = _MainTex_TexelSize.xyxy * vec4(1.0, 0.0, 0.0, 1.0) + vs_TEXCOORD0.zwzw;
    u_xlat0 = u_xlat0 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_1.xyz = textureLod(_MainTex, u_xlat0.xy, 0.0).xyz;
    u_xlat16_0.xyz = textureLod(_MainTex, u_xlat0.zw, 0.0).xyz;
    u_xlat0.x = dot(u_xlat16_0.xyz, vec3(0.298999995, 0.587000012, 0.114));
    u_xlat5 = dot(u_xlat16_1.xyz, vec3(0.298999995, 0.587000012, 0.114));
    u_xlat10.xy = vs_TEXCOORD0.zw + _MainTex_TexelSize.xy;
    u_xlat10.xy = u_xlat10.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_1.xyz = textureLod(_MainTex, u_xlat10.xy, 0.0).xyz;
    u_xlat10.x = dot(u_xlat16_1.xyz, vec3(0.298999995, 0.587000012, 0.114));
    u_xlat15 = u_xlat10.x + u_xlat5;
    u_xlat1 = vs_TEXCOORD0.zwxy * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_2.xyz = textureLod(_MainTex, u_xlat1.xy, 0.0).xyz;
    u_xlat16_1.xyz = textureLod(_MainTex, u_xlat1.zw, 0.0).xyz;
    u_xlat1.x = dot(u_xlat16_1.xyz, vec3(0.298999995, 0.587000012, 0.114));
    u_xlat6 = dot(u_xlat16_2.xyz, vec3(0.298999995, 0.587000012, 0.114));
    u_xlat11 = u_xlat0.x + u_xlat6;
    u_xlat2.yw = (-vec2(u_xlat15)) + vec2(u_xlat11);
    u_xlat15 = u_xlat5 + u_xlat6;
    u_xlat11 = u_xlat10.x + u_xlat0.x;
    u_xlat11 = u_xlat15 + (-u_xlat11);
    u_xlat15 = u_xlat0.x + u_xlat15;
    u_xlat15 = u_xlat10.x + u_xlat15;
    u_xlat15 = u_xlat15 * 0.03125;
    u_xlat15 = max(u_xlat15, 0.0078125);
    u_xlat16 = min(abs(u_xlat2.w), abs(u_xlat11));
    u_xlat2.xz = (-vec2(u_xlat11));
    u_xlat15 = u_xlat15 + u_xlat16;
    u_xlat15 = float(1.0) / u_xlat15;
    u_xlat2 = vec4(u_xlat15) * u_xlat2;
    u_xlat2 = max(u_xlat2, vec4(-8.0, -8.0, -8.0, -8.0));
    u_xlat2 = min(u_xlat2, vec4(8.0, 8.0, 8.0, 8.0));
    u_xlat2 = u_xlat2 * _MainTex_TexelSize.xyxy;
    u_xlat3 = u_xlat2 * vec4(-0.5, -0.5, 0.5, 0.5) + vs_TEXCOORD0.xyxy;
    u_xlat2 = u_xlat2.zwzw * vec4(-0.166666672, -0.166666672, 0.166666672, 0.166666672) + vs_TEXCOORD0.xyxy;
    u_xlat2 = u_xlat2 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat3 = u_xlat3 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_4.xyz = textureLod(_MainTex, u_xlat3.xy, 0.0).xyz;
    u_xlat16_3.xyz = textureLod(_MainTex, u_xlat3.zw, 0.0).xyz;
    u_xlat3.xyz = u_xlat16_3.xyz + u_xlat16_4.xyz;
    u_xlat3.xyz = u_xlat3.xyz * vec3(0.25, 0.25, 0.25);
    u_xlat16_4.xyz = textureLod(_MainTex, u_xlat2.xy, 0.0).xyz;
    u_xlat16_2.xyz = textureLod(_MainTex, u_xlat2.zw, 0.0).xyz;
    u_xlat2.xyz = u_xlat16_2.xyz + u_xlat16_4.xyz;
    u_xlat3.xyz = u_xlat2.xyz * vec3(0.25, 0.25, 0.25) + u_xlat3.xyz;
    u_xlat2.xyz = u_xlat2.xyz * vec3(0.5, 0.5, 0.5);
    u_xlat0.w = dot(u_xlat3.xyz, vec3(0.298999995, 0.587000012, 0.114));
    u_xlat11 = min(u_xlat10.x, u_xlat0.x);
    u_xlat0.x = max(u_xlat10.x, u_xlat0.x);
    u_xlat10.x = min(u_xlat5, u_xlat6);
    u_xlat5 = max(u_xlat5, u_xlat6);
    u_xlat0.x = max(u_xlat0.x, u_xlat5);
    u_xlat0.x = max(u_xlat0.x, u_xlat1.x);
    u_xlat5 = min(u_xlat11, u_xlat10.x);
    u_xlat0.y = min(u_xlat5, u_xlat1.x);
    u_xlatb0.xy = lessThan(u_xlat0.xwxx, u_xlat0.wyww).xy;
    u_xlatb0.x = u_xlatb0.x || u_xlatb0.y;
    SV_Target0.xyz = (u_xlatb0.x) ? u_xlat2.xyz : u_xlat3.xyz;
    SV_Target0.w = 0.0;
    return;
}

#endif
"
}
SubProgram "gles3 hw_tier01 " {
"#ifdef VERTEX
#version 300 es

uniform 	vec4 hlslcc_mtx4x4unity_ObjectToWorld[4];
uniform 	vec4 hlslcc_mtx4x4unity_MatrixVP[4];
uniform 	vec4 _MainTex_TexelSize;
in highp vec4 in_POSITION0;
in mediump vec2 in_TEXCOORD0;
out highp vec4 vs_TEXCOORD0;
vec4 u_xlat0;
vec4 u_xlat1;
mediump vec2 u_xlat16_2;
void main()
{
    u_xlat0 = in_POSITION0.yyyy * hlslcc_mtx4x4unity_ObjectToWorld[1];
    u_xlat0 = hlslcc_mtx4x4unity_ObjectToWorld[0] * in_POSITION0.xxxx + u_xlat0;
    u_xlat0 = hlslcc_mtx4x4unity_ObjectToWorld[2] * in_POSITION0.zzzz + u_xlat0;
    u_xlat0 = u_xlat0 + hlslcc_mtx4x4unity_ObjectToWorld[3];
    u_xlat1 = u_xlat0.yyyy * hlslcc_mtx4x4unity_MatrixVP[1];
    u_xlat1 = hlslcc_mtx4x4unity_MatrixVP[0] * u_xlat0.xxxx + u_xlat1;
    u_xlat1 = hlslcc_mtx4x4unity_MatrixVP[2] * u_xlat0.zzzz + u_xlat1;
    gl_Position = hlslcc_mtx4x4unity_MatrixVP[3] * u_xlat0.wwww + u_xlat1;
    u_xlat16_2.xy = in_TEXCOORD0.xy * vec2(2.0, 2.0) + vec2(-1.0, -1.0);
    u_xlat0.xy = u_xlat16_2.xy * vec2(0.5, 0.5) + vec2(0.5, 0.5);
    vs_TEXCOORD0.zw = (-_MainTex_TexelSize.xy) * vec2(0.75, 0.75) + u_xlat0.xy;
    vs_TEXCOORD0.xy = u_xlat0.xy;
    return;
}

#endif
#ifdef FRAGMENT
#version 300 es
#ifdef GL_EXT_shader_texture_lod
#extension GL_EXT_shader_texture_lod : enable
#endif

precision highp float;
precision highp int;
uniform 	mediump vec4 _MainTex_ST;
uniform 	vec4 _MainTex_TexelSize;
uniform mediump sampler2D _MainTex;
in highp vec4 vs_TEXCOORD0;
layout(location = 0) out highp vec4 SV_Target0;
vec4 u_xlat0;
mediump vec3 u_xlat16_0;
bvec2 u_xlatb0;
vec4 u_xlat1;
mediump vec3 u_xlat16_1;
vec4 u_xlat2;
mediump vec3 u_xlat16_2;
vec4 u_xlat3;
mediump vec3 u_xlat16_3;
mediump vec3 u_xlat16_4;
float u_xlat5;
float u_xlat6;
vec2 u_xlat10;
float u_xlat11;
float u_xlat15;
float u_xlat16;
void main()
{
    u_xlat0 = _MainTex_TexelSize.xyxy * vec4(1.0, 0.0, 0.0, 1.0) + vs_TEXCOORD0.zwzw;
    u_xlat0 = u_xlat0 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_1.xyz = textureLod(_MainTex, u_xlat0.xy, 0.0).xyz;
    u_xlat16_0.xyz = textureLod(_MainTex, u_xlat0.zw, 0.0).xyz;
    u_xlat0.x = dot(u_xlat16_0.xyz, vec3(0.298999995, 0.587000012, 0.114));
    u_xlat5 = dot(u_xlat16_1.xyz, vec3(0.298999995, 0.587000012, 0.114));
    u_xlat10.xy = vs_TEXCOORD0.zw + _MainTex_TexelSize.xy;
    u_xlat10.xy = u_xlat10.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_1.xyz = textureLod(_MainTex, u_xlat10.xy, 0.0).xyz;
    u_xlat10.x = dot(u_xlat16_1.xyz, vec3(0.298999995, 0.587000012, 0.114));
    u_xlat15 = u_xlat10.x + u_xlat5;
    u_xlat1 = vs_TEXCOORD0.zwxy * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_2.xyz = textureLod(_MainTex, u_xlat1.xy, 0.0).xyz;
    u_xlat16_1.xyz = textureLod(_MainTex, u_xlat1.zw, 0.0).xyz;
    u_xlat1.x = dot(u_xlat16_1.xyz, vec3(0.298999995, 0.587000012, 0.114));
    u_xlat6 = dot(u_xlat16_2.xyz, vec3(0.298999995, 0.587000012, 0.114));
    u_xlat11 = u_xlat0.x + u_xlat6;
    u_xlat2.yw = (-vec2(u_xlat15)) + vec2(u_xlat11);
    u_xlat15 = u_xlat5 + u_xlat6;
    u_xlat11 = u_xlat10.x + u_xlat0.x;
    u_xlat11 = u_xlat15 + (-u_xlat11);
    u_xlat15 = u_xlat0.x + u_xlat15;
    u_xlat15 = u_xlat10.x + u_xlat15;
    u_xlat15 = u_xlat15 * 0.03125;
    u_xlat15 = max(u_xlat15, 0.0078125);
    u_xlat16 = min(abs(u_xlat2.w), abs(u_xlat11));
    u_xlat2.xz = (-vec2(u_xlat11));
    u_xlat15 = u_xlat15 + u_xlat16;
    u_xlat15 = float(1.0) / u_xlat15;
    u_xlat2 = vec4(u_xlat15) * u_xlat2;
    u_xlat2 = max(u_xlat2, vec4(-8.0, -8.0, -8.0, -8.0));
    u_xlat2 = min(u_xlat2, vec4(8.0, 8.0, 8.0, 8.0));
    u_xlat2 = u_xlat2 * _MainTex_TexelSize.xyxy;
    u_xlat3 = u_xlat2 * vec4(-0.5, -0.5, 0.5, 0.5) + vs_TEXCOORD0.xyxy;
    u_xlat2 = u_xlat2.zwzw * vec4(-0.166666672, -0.166666672, 0.166666672, 0.166666672) + vs_TEXCOORD0.xyxy;
    u_xlat2 = u_xlat2 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat3 = u_xlat3 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_4.xyz = textureLod(_MainTex, u_xlat3.xy, 0.0).xyz;
    u_xlat16_3.xyz = textureLod(_MainTex, u_xlat3.zw, 0.0).xyz;
    u_xlat3.xyz = u_xlat16_3.xyz + u_xlat16_4.xyz;
    u_xlat3.xyz = u_xlat3.xyz * vec3(0.25, 0.25, 0.25);
    u_xlat16_4.xyz = textureLod(_MainTex, u_xlat2.xy, 0.0).xyz;
    u_xlat16_2.xyz = textureLod(_MainTex, u_xlat2.zw, 0.0).xyz;
    u_xlat2.xyz = u_xlat16_2.xyz + u_xlat16_4.xyz;
    u_xlat3.xyz = u_xlat2.xyz * vec3(0.25, 0.25, 0.25) + u_xlat3.xyz;
    u_xlat2.xyz = u_xlat2.xyz * vec3(0.5, 0.5, 0.5);
    u_xlat0.w = dot(u_xlat3.xyz, vec3(0.298999995, 0.587000012, 0.114));
    u_xlat11 = min(u_xlat10.x, u_xlat0.x);
    u_xlat0.x = max(u_xlat10.x, u_xlat0.x);
    u_xlat10.x = min(u_xlat5, u_xlat6);
    u_xlat5 = max(u_xlat5, u_xlat6);
    u_xlat0.x = max(u_xlat0.x, u_xlat5);
    u_xlat0.x = max(u_xlat0.x, u_xlat1.x);
    u_xlat5 = min(u_xlat11, u_xlat10.x);
    u_xlat0.y = min(u_xlat5, u_xlat1.x);
    u_xlatb0.xy = lessThan(u_xlat0.xwxx, u_xlat0.wyww).xy;
    u_xlatb0.x = u_xlatb0.x || u_xlatb0.y;
    SV_Target0.xyz = (u_xlatb0.x) ? u_xlat2.xyz : u_xlat3.xyz;
    SV_Target0.w = 0.0;
    return;
}

#endif
"
}
SubProgram "gles3 hw_tier02 " {
"#ifdef VERTEX
#version 300 es

uniform 	vec4 hlslcc_mtx4x4unity_ObjectToWorld[4];
uniform 	vec4 hlslcc_mtx4x4unity_MatrixVP[4];
uniform 	vec4 _MainTex_TexelSize;
in highp vec4 in_POSITION0;
in mediump vec2 in_TEXCOORD0;
out highp vec4 vs_TEXCOORD0;
vec4 u_xlat0;
vec4 u_xlat1;
mediump vec2 u_xlat16_2;
void main()
{
    u_xlat0 = in_POSITION0.yyyy * hlslcc_mtx4x4unity_ObjectToWorld[1];
    u_xlat0 = hlslcc_mtx4x4unity_ObjectToWorld[0] * in_POSITION0.xxxx + u_xlat0;
    u_xlat0 = hlslcc_mtx4x4unity_ObjectToWorld[2] * in_POSITION0.zzzz + u_xlat0;
    u_xlat0 = u_xlat0 + hlslcc_mtx4x4unity_ObjectToWorld[3];
    u_xlat1 = u_xlat0.yyyy * hlslcc_mtx4x4unity_MatrixVP[1];
    u_xlat1 = hlslcc_mtx4x4unity_MatrixVP[0] * u_xlat0.xxxx + u_xlat1;
    u_xlat1 = hlslcc_mtx4x4unity_MatrixVP[2] * u_xlat0.zzzz + u_xlat1;
    gl_Position = hlslcc_mtx4x4unity_MatrixVP[3] * u_xlat0.wwww + u_xlat1;
    u_xlat16_2.xy = in_TEXCOORD0.xy * vec2(2.0, 2.0) + vec2(-1.0, -1.0);
    u_xlat0.xy = u_xlat16_2.xy * vec2(0.5, 0.5) + vec2(0.5, 0.5);
    vs_TEXCOORD0.zw = (-_MainTex_TexelSize.xy) * vec2(0.75, 0.75) + u_xlat0.xy;
    vs_TEXCOORD0.xy = u_xlat0.xy;
    return;
}

#endif
#ifdef FRAGMENT
#version 300 es
#ifdef GL_EXT_shader_texture_lod
#extension GL_EXT_shader_texture_lod : enable
#endif

precision highp float;
precision highp int;
uniform 	mediump vec4 _MainTex_ST;
uniform 	vec4 _MainTex_TexelSize;
uniform mediump sampler2D _MainTex;
in highp vec4 vs_TEXCOORD0;
layout(location = 0) out highp vec4 SV_Target0;
vec4 u_xlat0;
mediump vec3 u_xlat16_0;
bvec2 u_xlatb0;
vec4 u_xlat1;
mediump vec3 u_xlat16_1;
vec4 u_xlat2;
mediump vec3 u_xlat16_2;
vec4 u_xlat3;
mediump vec3 u_xlat16_3;
mediump vec3 u_xlat16_4;
float u_xlat5;
float u_xlat6;
vec2 u_xlat10;
float u_xlat11;
float u_xlat15;
float u_xlat16;
void main()
{
    u_xlat0 = _MainTex_TexelSize.xyxy * vec4(1.0, 0.0, 0.0, 1.0) + vs_TEXCOORD0.zwzw;
    u_xlat0 = u_xlat0 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_1.xyz = textureLod(_MainTex, u_xlat0.xy, 0.0).xyz;
    u_xlat16_0.xyz = textureLod(_MainTex, u_xlat0.zw, 0.0).xyz;
    u_xlat0.x = dot(u_xlat16_0.xyz, vec3(0.298999995, 0.587000012, 0.114));
    u_xlat5 = dot(u_xlat16_1.xyz, vec3(0.298999995, 0.587000012, 0.114));
    u_xlat10.xy = vs_TEXCOORD0.zw + _MainTex_TexelSize.xy;
    u_xlat10.xy = u_xlat10.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_1.xyz = textureLod(_MainTex, u_xlat10.xy, 0.0).xyz;
    u_xlat10.x = dot(u_xlat16_1.xyz, vec3(0.298999995, 0.587000012, 0.114));
    u_xlat15 = u_xlat10.x + u_xlat5;
    u_xlat1 = vs_TEXCOORD0.zwxy * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_2.xyz = textureLod(_MainTex, u_xlat1.xy, 0.0).xyz;
    u_xlat16_1.xyz = textureLod(_MainTex, u_xlat1.zw, 0.0).xyz;
    u_xlat1.x = dot(u_xlat16_1.xyz, vec3(0.298999995, 0.587000012, 0.114));
    u_xlat6 = dot(u_xlat16_2.xyz, vec3(0.298999995, 0.587000012, 0.114));
    u_xlat11 = u_xlat0.x + u_xlat6;
    u_xlat2.yw = (-vec2(u_xlat15)) + vec2(u_xlat11);
    u_xlat15 = u_xlat5 + u_xlat6;
    u_xlat11 = u_xlat10.x + u_xlat0.x;
    u_xlat11 = u_xlat15 + (-u_xlat11);
    u_xlat15 = u_xlat0.x + u_xlat15;
    u_xlat15 = u_xlat10.x + u_xlat15;
    u_xlat15 = u_xlat15 * 0.03125;
    u_xlat15 = max(u_xlat15, 0.0078125);
    u_xlat16 = min(abs(u_xlat2.w), abs(u_xlat11));
    u_xlat2.xz = (-vec2(u_xlat11));
    u_xlat15 = u_xlat15 + u_xlat16;
    u_xlat15 = float(1.0) / u_xlat15;
    u_xlat2 = vec4(u_xlat15) * u_xlat2;
    u_xlat2 = max(u_xlat2, vec4(-8.0, -8.0, -8.0, -8.0));
    u_xlat2 = min(u_xlat2, vec4(8.0, 8.0, 8.0, 8.0));
    u_xlat2 = u_xlat2 * _MainTex_TexelSize.xyxy;
    u_xlat3 = u_xlat2 * vec4(-0.5, -0.5, 0.5, 0.5) + vs_TEXCOORD0.xyxy;
    u_xlat2 = u_xlat2.zwzw * vec4(-0.166666672, -0.166666672, 0.166666672, 0.166666672) + vs_TEXCOORD0.xyxy;
    u_xlat2 = u_xlat2 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat3 = u_xlat3 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_4.xyz = textureLod(_MainTex, u_xlat3.xy, 0.0).xyz;
    u_xlat16_3.xyz = textureLod(_MainTex, u_xlat3.zw, 0.0).xyz;
    u_xlat3.xyz = u_xlat16_3.xyz + u_xlat16_4.xyz;
    u_xlat3.xyz = u_xlat3.xyz * vec3(0.25, 0.25, 0.25);
    u_xlat16_4.xyz = textureLod(_MainTex, u_xlat2.xy, 0.0).xyz;
    u_xlat16_2.xyz = textureLod(_MainTex, u_xlat2.zw, 0.0).xyz;
    u_xlat2.xyz = u_xlat16_2.xyz + u_xlat16_4.xyz;
    u_xlat3.xyz = u_xlat2.xyz * vec3(0.25, 0.25, 0.25) + u_xlat3.xyz;
    u_xlat2.xyz = u_xlat2.xyz * vec3(0.5, 0.5, 0.5);
    u_xlat0.w = dot(u_xlat3.xyz, vec3(0.298999995, 0.587000012, 0.114));
    u_xlat11 = min(u_xlat10.x, u_xlat0.x);
    u_xlat0.x = max(u_xlat10.x, u_xlat0.x);
    u_xlat10.x = min(u_xlat5, u_xlat6);
    u_xlat5 = max(u_xlat5, u_xlat6);
    u_xlat0.x = max(u_xlat0.x, u_xlat5);
    u_xlat0.x = max(u_xlat0.x, u_xlat1.x);
    u_xlat5 = min(u_xlat11, u_xlat10.x);
    u_xlat0.y = min(u_xlat5, u_xlat1.x);
    u_xlatb0.xy = lessThan(u_xlat0.xwxx, u_xlat0.wyww).xy;
    u_xlatb0.x = u_xlatb0.x || u_xlatb0.y;
    SV_Target0.xyz = (u_xlatb0.x) ? u_xlat2.xyz : u_xlat3.xyz;
    SV_Target0.w = 0.0;
    return;
}

#endif
"
}
}
Program "fp" {
SubProgram "gles hw_tier00 " {
""
}
SubProgram "gles hw_tier01 " {
""
}
SubProgram "gles hw_tier02 " {
""
}
SubProgram "gles3 hw_tier00 " {
""
}
SubProgram "gles3 hw_tier01 " {
""
}
SubProgram "gles3 hw_tier02 " {
""
}
}
}
}
}