//////////////////////////////////////////
//
// NOTE: This is *not* a valid shader file
//
///////////////////////////////////////////
Shader "Hidden/FXAA III (Console)" {
Properties {
_MainTex ("-", 2D) = "white" { }
_EdgeThresholdMin ("Edge threshold min", Float) = 0.125
_EdgeThreshold ("Edge Threshold", Float) = 0.25
_EdgeSharpness ("Edge sharpness", Float) = 4
}
SubShader {
 Pass {
  ZTest Always
  ZWrite Off
  Cull Off
  GpuProgramID 48322
Program "vp" {
SubProgram "gles hw_tier00 " {
"#ifdef VERTEX
#version 100

uniform 	vec4 hlslcc_mtx4x4unity_ObjectToWorld[4];
uniform 	vec4 hlslcc_mtx4x4unity_MatrixVP[4];
uniform 	vec4 _MainTex_TexelSize;
attribute highp vec4 in_POSITION0;
attribute mediump vec2 in_TEXCOORD0;
varying highp vec2 vs_TEXCOORD0;
varying highp vec4 vs_TEXCOORD1;
varying highp vec4 vs_TEXCOORD2;
varying highp vec4 vs_TEXCOORD3;
vec4 u_xlat0;
vec4 u_xlat1;
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
    vs_TEXCOORD0.xy = in_TEXCOORD0.xy;
    vs_TEXCOORD1.xy = (-_MainTex_TexelSize.xy) * vec2(0.5, 0.5) + in_TEXCOORD0.xy;
    vs_TEXCOORD1.zw = _MainTex_TexelSize.xy * vec2(0.5, 0.5) + in_TEXCOORD0.xy;
    u_xlat0 = _MainTex_TexelSize.xyxy * vec4(-0.5, -0.5, 0.5, 0.5);
    vs_TEXCOORD2 = u_xlat0;
    vs_TEXCOORD3.xy = _MainTex_TexelSize.xy * vec2(-2.0, -2.0);
    vs_TEXCOORD3.zw = _MainTex_TexelSize.xy + _MainTex_TexelSize.xy;
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
uniform 	mediump float _EdgeThresholdMin;
uniform 	mediump float _EdgeThreshold;
uniform 	mediump float _EdgeSharpness;
uniform 	mediump vec4 _MainTex_ST;
uniform lowp sampler2D _MainTex;
varying highp vec2 vs_TEXCOORD0;
varying highp vec4 vs_TEXCOORD1;
varying highp vec4 vs_TEXCOORD2;
varying highp vec4 vs_TEXCOORD3;
#define SV_Target0 gl_FragData[0]
vec4 u_xlat0;
lowp vec3 u_xlat10_0;
vec2 u_xlat1;
lowp vec3 u_xlat10_1;
bool u_xlatb1;
mediump vec3 u_xlat16_2;
mediump float u_xlat16_3;
mediump vec3 u_xlat16_4;
vec2 u_xlat5;
lowp vec3 u_xlat10_5;
vec2 u_xlat6;
lowp vec3 u_xlat10_6;
vec2 u_xlat7;
lowp vec3 u_xlat10_7;
mediump vec3 u_xlat16_8;
mediump vec3 u_xlat16_11;
mediump vec3 u_xlat16_12;
mediump float u_xlat16_20;
mediump float u_xlat16_21;
bool u_xlatb27;
mediump float u_xlat16_29;
mediump float u_xlat16_30;
void main()
{
    u_xlat0 = vs_TEXCOORD1.xyxw * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_1.xyz = texture2DLodEXT(_MainTex, u_xlat0.xy, 0.0).xyz;
    u_xlat16_2.x = dot(u_xlat10_1.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat10_0.xyz = texture2DLodEXT(_MainTex, u_xlat0.zw, 0.0).xyz;
    u_xlat16_11.x = dot(u_xlat10_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat0 = vs_TEXCOORD1.zyzw * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_1.xyz = texture2DLodEXT(_MainTex, u_xlat0.xy, 0.0).xyz;
    u_xlat16_20 = dot(u_xlat10_1.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat10_0.xyz = texture2DLodEXT(_MainTex, u_xlat0.zw, 0.0).xyz;
    u_xlat16_11.z = dot(u_xlat10_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat0.xy = vs_TEXCOORD0.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_0.xyz = texture2DLodEXT(_MainTex, u_xlat0.xy, 0.0).xyz;
    u_xlat16_3 = dot(u_xlat10_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat16_2.z = u_xlat16_20 + 0.00260416674;
    u_xlat16_21 = min(u_xlat16_11.x, u_xlat16_2.x);
    u_xlat16_12.xz = max(u_xlat16_11.xz, u_xlat16_2.xz);
    u_xlat16_4.x = min(u_xlat16_11.z, u_xlat16_2.z);
    u_xlat16_12.x = max(u_xlat16_12.x, u_xlat16_12.z);
    u_xlat16_21 = min(u_xlat16_21, u_xlat16_4.x);
    u_xlat16_30 = u_xlat16_12.x * _EdgeThreshold;
    u_xlat16_4.x = min(u_xlat16_3, u_xlat16_21);
    u_xlat16_30 = max(u_xlat16_30, _EdgeThresholdMin);
    u_xlat16_3 = max(u_xlat16_3, u_xlat16_12.x);
    u_xlat16_3 = (-u_xlat16_4.x) + u_xlat16_3;
    u_xlatb27 = u_xlat16_3>=u_xlat16_30;
    if(u_xlatb27){
        u_xlat16_2.xy = (-u_xlat16_2.xz) + u_xlat16_11.zx;
        u_xlat16_4.x = u_xlat16_2.x + u_xlat16_2.y;
        u_xlat16_4.y = (-u_xlat16_2.x) + u_xlat16_2.y;
        u_xlat16_2.x = dot(u_xlat16_4.xy, u_xlat16_4.xy);
        u_xlat16_2.x = inversesqrt(u_xlat16_2.x);
        u_xlat16_2.xy = u_xlat16_2.xx * u_xlat16_4.xy;
        u_xlat1.xy = (-u_xlat16_2.xy) * vs_TEXCOORD2.zw + vs_TEXCOORD0.xy;
        u_xlat1.xy = u_xlat1.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat10_1.xyz = texture2DLodEXT(_MainTex, u_xlat1.xy, 0.0).xyz;
        u_xlat5.xy = u_xlat16_2.xy * vs_TEXCOORD2.zw + vs_TEXCOORD0.xy;
        u_xlat5.xy = u_xlat5.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat10_5.xyz = texture2DLodEXT(_MainTex, u_xlat5.xy, 0.0).xyz;
        u_xlat16_20 = min(abs(u_xlat16_2.y), abs(u_xlat16_2.x));
        u_xlat16_20 = u_xlat16_20 * _EdgeSharpness;
        u_xlat16_2.xy = u_xlat16_2.xy / vec2(u_xlat16_20);
        u_xlat16_2.xy = max(u_xlat16_2.xy, vec2(-2.0, -2.0));
        u_xlat16_2.xy = min(u_xlat16_2.xy, vec2(2.0, 2.0));
        u_xlat6.xy = (-u_xlat16_2.xy) * vs_TEXCOORD3.zw + vs_TEXCOORD0.xy;
        u_xlat6.xy = u_xlat6.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat10_6.xyz = texture2DLodEXT(_MainTex, u_xlat6.xy, 0.0).xyz;
        u_xlat7.xy = u_xlat16_2.xy * vs_TEXCOORD3.zw + vs_TEXCOORD0.xy;
        u_xlat7.xy = u_xlat7.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat10_7.xyz = texture2DLodEXT(_MainTex, u_xlat7.xy, 0.0).xyz;
        u_xlat16_2.xyz = u_xlat10_1.xyz + u_xlat10_5.xyz;
        u_xlat16_4.xyz = u_xlat10_6.xyz + u_xlat10_7.xyz;
        u_xlat16_8.xyz = u_xlat16_2.xyz * vec3(0.25, 0.25, 0.25);
        u_xlat16_4.xyz = u_xlat16_4.xyz * vec3(0.25, 0.25, 0.25) + u_xlat16_8.xyz;
        u_xlat16_29 = dot(u_xlat16_2.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
        u_xlatb27 = u_xlat16_29<u_xlat16_21;
        u_xlat16_29 = dot(u_xlat16_4.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
        u_xlatb1 = u_xlat16_12.x<u_xlat16_29;
        u_xlatb27 = u_xlatb27 || u_xlatb1;
        u_xlat16_2.xyz = u_xlat16_2.xyz * vec3(0.5, 0.5, 0.5);
        u_xlat16_2.xyz = (bool(u_xlatb27)) ? u_xlat16_2.xyz : u_xlat16_4.xyz;
    } else {
        u_xlat16_2.xyz = u_xlat10_0.xyz;
    //ENDIF
    }
    SV_Target0.xyz = u_xlat16_2.xyz;
    SV_Target0.w = 1.0;
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
varying highp vec2 vs_TEXCOORD0;
varying highp vec4 vs_TEXCOORD1;
varying highp vec4 vs_TEXCOORD2;
varying highp vec4 vs_TEXCOORD3;
vec4 u_xlat0;
vec4 u_xlat1;
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
    vs_TEXCOORD0.xy = in_TEXCOORD0.xy;
    vs_TEXCOORD1.xy = (-_MainTex_TexelSize.xy) * vec2(0.5, 0.5) + in_TEXCOORD0.xy;
    vs_TEXCOORD1.zw = _MainTex_TexelSize.xy * vec2(0.5, 0.5) + in_TEXCOORD0.xy;
    u_xlat0 = _MainTex_TexelSize.xyxy * vec4(-0.5, -0.5, 0.5, 0.5);
    vs_TEXCOORD2 = u_xlat0;
    vs_TEXCOORD3.xy = _MainTex_TexelSize.xy * vec2(-2.0, -2.0);
    vs_TEXCOORD3.zw = _MainTex_TexelSize.xy + _MainTex_TexelSize.xy;
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
uniform 	mediump float _EdgeThresholdMin;
uniform 	mediump float _EdgeThreshold;
uniform 	mediump float _EdgeSharpness;
uniform 	mediump vec4 _MainTex_ST;
uniform lowp sampler2D _MainTex;
varying highp vec2 vs_TEXCOORD0;
varying highp vec4 vs_TEXCOORD1;
varying highp vec4 vs_TEXCOORD2;
varying highp vec4 vs_TEXCOORD3;
#define SV_Target0 gl_FragData[0]
vec4 u_xlat0;
lowp vec3 u_xlat10_0;
vec2 u_xlat1;
lowp vec3 u_xlat10_1;
bool u_xlatb1;
mediump vec3 u_xlat16_2;
mediump float u_xlat16_3;
mediump vec3 u_xlat16_4;
vec2 u_xlat5;
lowp vec3 u_xlat10_5;
vec2 u_xlat6;
lowp vec3 u_xlat10_6;
vec2 u_xlat7;
lowp vec3 u_xlat10_7;
mediump vec3 u_xlat16_8;
mediump vec3 u_xlat16_11;
mediump vec3 u_xlat16_12;
mediump float u_xlat16_20;
mediump float u_xlat16_21;
bool u_xlatb27;
mediump float u_xlat16_29;
mediump float u_xlat16_30;
void main()
{
    u_xlat0 = vs_TEXCOORD1.xyxw * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_1.xyz = texture2DLodEXT(_MainTex, u_xlat0.xy, 0.0).xyz;
    u_xlat16_2.x = dot(u_xlat10_1.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat10_0.xyz = texture2DLodEXT(_MainTex, u_xlat0.zw, 0.0).xyz;
    u_xlat16_11.x = dot(u_xlat10_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat0 = vs_TEXCOORD1.zyzw * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_1.xyz = texture2DLodEXT(_MainTex, u_xlat0.xy, 0.0).xyz;
    u_xlat16_20 = dot(u_xlat10_1.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat10_0.xyz = texture2DLodEXT(_MainTex, u_xlat0.zw, 0.0).xyz;
    u_xlat16_11.z = dot(u_xlat10_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat0.xy = vs_TEXCOORD0.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_0.xyz = texture2DLodEXT(_MainTex, u_xlat0.xy, 0.0).xyz;
    u_xlat16_3 = dot(u_xlat10_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat16_2.z = u_xlat16_20 + 0.00260416674;
    u_xlat16_21 = min(u_xlat16_11.x, u_xlat16_2.x);
    u_xlat16_12.xz = max(u_xlat16_11.xz, u_xlat16_2.xz);
    u_xlat16_4.x = min(u_xlat16_11.z, u_xlat16_2.z);
    u_xlat16_12.x = max(u_xlat16_12.x, u_xlat16_12.z);
    u_xlat16_21 = min(u_xlat16_21, u_xlat16_4.x);
    u_xlat16_30 = u_xlat16_12.x * _EdgeThreshold;
    u_xlat16_4.x = min(u_xlat16_3, u_xlat16_21);
    u_xlat16_30 = max(u_xlat16_30, _EdgeThresholdMin);
    u_xlat16_3 = max(u_xlat16_3, u_xlat16_12.x);
    u_xlat16_3 = (-u_xlat16_4.x) + u_xlat16_3;
    u_xlatb27 = u_xlat16_3>=u_xlat16_30;
    if(u_xlatb27){
        u_xlat16_2.xy = (-u_xlat16_2.xz) + u_xlat16_11.zx;
        u_xlat16_4.x = u_xlat16_2.x + u_xlat16_2.y;
        u_xlat16_4.y = (-u_xlat16_2.x) + u_xlat16_2.y;
        u_xlat16_2.x = dot(u_xlat16_4.xy, u_xlat16_4.xy);
        u_xlat16_2.x = inversesqrt(u_xlat16_2.x);
        u_xlat16_2.xy = u_xlat16_2.xx * u_xlat16_4.xy;
        u_xlat1.xy = (-u_xlat16_2.xy) * vs_TEXCOORD2.zw + vs_TEXCOORD0.xy;
        u_xlat1.xy = u_xlat1.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat10_1.xyz = texture2DLodEXT(_MainTex, u_xlat1.xy, 0.0).xyz;
        u_xlat5.xy = u_xlat16_2.xy * vs_TEXCOORD2.zw + vs_TEXCOORD0.xy;
        u_xlat5.xy = u_xlat5.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat10_5.xyz = texture2DLodEXT(_MainTex, u_xlat5.xy, 0.0).xyz;
        u_xlat16_20 = min(abs(u_xlat16_2.y), abs(u_xlat16_2.x));
        u_xlat16_20 = u_xlat16_20 * _EdgeSharpness;
        u_xlat16_2.xy = u_xlat16_2.xy / vec2(u_xlat16_20);
        u_xlat16_2.xy = max(u_xlat16_2.xy, vec2(-2.0, -2.0));
        u_xlat16_2.xy = min(u_xlat16_2.xy, vec2(2.0, 2.0));
        u_xlat6.xy = (-u_xlat16_2.xy) * vs_TEXCOORD3.zw + vs_TEXCOORD0.xy;
        u_xlat6.xy = u_xlat6.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat10_6.xyz = texture2DLodEXT(_MainTex, u_xlat6.xy, 0.0).xyz;
        u_xlat7.xy = u_xlat16_2.xy * vs_TEXCOORD3.zw + vs_TEXCOORD0.xy;
        u_xlat7.xy = u_xlat7.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat10_7.xyz = texture2DLodEXT(_MainTex, u_xlat7.xy, 0.0).xyz;
        u_xlat16_2.xyz = u_xlat10_1.xyz + u_xlat10_5.xyz;
        u_xlat16_4.xyz = u_xlat10_6.xyz + u_xlat10_7.xyz;
        u_xlat16_8.xyz = u_xlat16_2.xyz * vec3(0.25, 0.25, 0.25);
        u_xlat16_4.xyz = u_xlat16_4.xyz * vec3(0.25, 0.25, 0.25) + u_xlat16_8.xyz;
        u_xlat16_29 = dot(u_xlat16_2.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
        u_xlatb27 = u_xlat16_29<u_xlat16_21;
        u_xlat16_29 = dot(u_xlat16_4.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
        u_xlatb1 = u_xlat16_12.x<u_xlat16_29;
        u_xlatb27 = u_xlatb27 || u_xlatb1;
        u_xlat16_2.xyz = u_xlat16_2.xyz * vec3(0.5, 0.5, 0.5);
        u_xlat16_2.xyz = (bool(u_xlatb27)) ? u_xlat16_2.xyz : u_xlat16_4.xyz;
    } else {
        u_xlat16_2.xyz = u_xlat10_0.xyz;
    //ENDIF
    }
    SV_Target0.xyz = u_xlat16_2.xyz;
    SV_Target0.w = 1.0;
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
varying highp vec2 vs_TEXCOORD0;
varying highp vec4 vs_TEXCOORD1;
varying highp vec4 vs_TEXCOORD2;
varying highp vec4 vs_TEXCOORD3;
vec4 u_xlat0;
vec4 u_xlat1;
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
    vs_TEXCOORD0.xy = in_TEXCOORD0.xy;
    vs_TEXCOORD1.xy = (-_MainTex_TexelSize.xy) * vec2(0.5, 0.5) + in_TEXCOORD0.xy;
    vs_TEXCOORD1.zw = _MainTex_TexelSize.xy * vec2(0.5, 0.5) + in_TEXCOORD0.xy;
    u_xlat0 = _MainTex_TexelSize.xyxy * vec4(-0.5, -0.5, 0.5, 0.5);
    vs_TEXCOORD2 = u_xlat0;
    vs_TEXCOORD3.xy = _MainTex_TexelSize.xy * vec2(-2.0, -2.0);
    vs_TEXCOORD3.zw = _MainTex_TexelSize.xy + _MainTex_TexelSize.xy;
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
uniform 	mediump float _EdgeThresholdMin;
uniform 	mediump float _EdgeThreshold;
uniform 	mediump float _EdgeSharpness;
uniform 	mediump vec4 _MainTex_ST;
uniform lowp sampler2D _MainTex;
varying highp vec2 vs_TEXCOORD0;
varying highp vec4 vs_TEXCOORD1;
varying highp vec4 vs_TEXCOORD2;
varying highp vec4 vs_TEXCOORD3;
#define SV_Target0 gl_FragData[0]
vec4 u_xlat0;
lowp vec3 u_xlat10_0;
vec2 u_xlat1;
lowp vec3 u_xlat10_1;
bool u_xlatb1;
mediump vec3 u_xlat16_2;
mediump float u_xlat16_3;
mediump vec3 u_xlat16_4;
vec2 u_xlat5;
lowp vec3 u_xlat10_5;
vec2 u_xlat6;
lowp vec3 u_xlat10_6;
vec2 u_xlat7;
lowp vec3 u_xlat10_7;
mediump vec3 u_xlat16_8;
mediump vec3 u_xlat16_11;
mediump vec3 u_xlat16_12;
mediump float u_xlat16_20;
mediump float u_xlat16_21;
bool u_xlatb27;
mediump float u_xlat16_29;
mediump float u_xlat16_30;
void main()
{
    u_xlat0 = vs_TEXCOORD1.xyxw * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_1.xyz = texture2DLodEXT(_MainTex, u_xlat0.xy, 0.0).xyz;
    u_xlat16_2.x = dot(u_xlat10_1.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat10_0.xyz = texture2DLodEXT(_MainTex, u_xlat0.zw, 0.0).xyz;
    u_xlat16_11.x = dot(u_xlat10_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat0 = vs_TEXCOORD1.zyzw * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_1.xyz = texture2DLodEXT(_MainTex, u_xlat0.xy, 0.0).xyz;
    u_xlat16_20 = dot(u_xlat10_1.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat10_0.xyz = texture2DLodEXT(_MainTex, u_xlat0.zw, 0.0).xyz;
    u_xlat16_11.z = dot(u_xlat10_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat0.xy = vs_TEXCOORD0.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_0.xyz = texture2DLodEXT(_MainTex, u_xlat0.xy, 0.0).xyz;
    u_xlat16_3 = dot(u_xlat10_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat16_2.z = u_xlat16_20 + 0.00260416674;
    u_xlat16_21 = min(u_xlat16_11.x, u_xlat16_2.x);
    u_xlat16_12.xz = max(u_xlat16_11.xz, u_xlat16_2.xz);
    u_xlat16_4.x = min(u_xlat16_11.z, u_xlat16_2.z);
    u_xlat16_12.x = max(u_xlat16_12.x, u_xlat16_12.z);
    u_xlat16_21 = min(u_xlat16_21, u_xlat16_4.x);
    u_xlat16_30 = u_xlat16_12.x * _EdgeThreshold;
    u_xlat16_4.x = min(u_xlat16_3, u_xlat16_21);
    u_xlat16_30 = max(u_xlat16_30, _EdgeThresholdMin);
    u_xlat16_3 = max(u_xlat16_3, u_xlat16_12.x);
    u_xlat16_3 = (-u_xlat16_4.x) + u_xlat16_3;
    u_xlatb27 = u_xlat16_3>=u_xlat16_30;
    if(u_xlatb27){
        u_xlat16_2.xy = (-u_xlat16_2.xz) + u_xlat16_11.zx;
        u_xlat16_4.x = u_xlat16_2.x + u_xlat16_2.y;
        u_xlat16_4.y = (-u_xlat16_2.x) + u_xlat16_2.y;
        u_xlat16_2.x = dot(u_xlat16_4.xy, u_xlat16_4.xy);
        u_xlat16_2.x = inversesqrt(u_xlat16_2.x);
        u_xlat16_2.xy = u_xlat16_2.xx * u_xlat16_4.xy;
        u_xlat1.xy = (-u_xlat16_2.xy) * vs_TEXCOORD2.zw + vs_TEXCOORD0.xy;
        u_xlat1.xy = u_xlat1.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat10_1.xyz = texture2DLodEXT(_MainTex, u_xlat1.xy, 0.0).xyz;
        u_xlat5.xy = u_xlat16_2.xy * vs_TEXCOORD2.zw + vs_TEXCOORD0.xy;
        u_xlat5.xy = u_xlat5.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat10_5.xyz = texture2DLodEXT(_MainTex, u_xlat5.xy, 0.0).xyz;
        u_xlat16_20 = min(abs(u_xlat16_2.y), abs(u_xlat16_2.x));
        u_xlat16_20 = u_xlat16_20 * _EdgeSharpness;
        u_xlat16_2.xy = u_xlat16_2.xy / vec2(u_xlat16_20);
        u_xlat16_2.xy = max(u_xlat16_2.xy, vec2(-2.0, -2.0));
        u_xlat16_2.xy = min(u_xlat16_2.xy, vec2(2.0, 2.0));
        u_xlat6.xy = (-u_xlat16_2.xy) * vs_TEXCOORD3.zw + vs_TEXCOORD0.xy;
        u_xlat6.xy = u_xlat6.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat10_6.xyz = texture2DLodEXT(_MainTex, u_xlat6.xy, 0.0).xyz;
        u_xlat7.xy = u_xlat16_2.xy * vs_TEXCOORD3.zw + vs_TEXCOORD0.xy;
        u_xlat7.xy = u_xlat7.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat10_7.xyz = texture2DLodEXT(_MainTex, u_xlat7.xy, 0.0).xyz;
        u_xlat16_2.xyz = u_xlat10_1.xyz + u_xlat10_5.xyz;
        u_xlat16_4.xyz = u_xlat10_6.xyz + u_xlat10_7.xyz;
        u_xlat16_8.xyz = u_xlat16_2.xyz * vec3(0.25, 0.25, 0.25);
        u_xlat16_4.xyz = u_xlat16_4.xyz * vec3(0.25, 0.25, 0.25) + u_xlat16_8.xyz;
        u_xlat16_29 = dot(u_xlat16_2.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
        u_xlatb27 = u_xlat16_29<u_xlat16_21;
        u_xlat16_29 = dot(u_xlat16_4.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
        u_xlatb1 = u_xlat16_12.x<u_xlat16_29;
        u_xlatb27 = u_xlatb27 || u_xlatb1;
        u_xlat16_2.xyz = u_xlat16_2.xyz * vec3(0.5, 0.5, 0.5);
        u_xlat16_2.xyz = (bool(u_xlatb27)) ? u_xlat16_2.xyz : u_xlat16_4.xyz;
    } else {
        u_xlat16_2.xyz = u_xlat10_0.xyz;
    //ENDIF
    }
    SV_Target0.xyz = u_xlat16_2.xyz;
    SV_Target0.w = 1.0;
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
out highp vec2 vs_TEXCOORD0;
out highp vec4 vs_TEXCOORD1;
out highp vec4 vs_TEXCOORD2;
out highp vec4 vs_TEXCOORD3;
vec4 u_xlat0;
vec4 u_xlat1;
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
    vs_TEXCOORD0.xy = in_TEXCOORD0.xy;
    vs_TEXCOORD1.xy = (-_MainTex_TexelSize.xy) * vec2(0.5, 0.5) + in_TEXCOORD0.xy;
    vs_TEXCOORD1.zw = _MainTex_TexelSize.xy * vec2(0.5, 0.5) + in_TEXCOORD0.xy;
    u_xlat0 = _MainTex_TexelSize.xyxy * vec4(-0.5, -0.5, 0.5, 0.5);
    vs_TEXCOORD2 = u_xlat0;
    vs_TEXCOORD3.xy = _MainTex_TexelSize.xy * vec2(-2.0, -2.0);
    vs_TEXCOORD3.zw = _MainTex_TexelSize.xy + _MainTex_TexelSize.xy;
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
uniform 	mediump float _EdgeThresholdMin;
uniform 	mediump float _EdgeThreshold;
uniform 	mediump float _EdgeSharpness;
uniform 	mediump vec4 _MainTex_ST;
uniform mediump sampler2D _MainTex;
in highp vec2 vs_TEXCOORD0;
in highp vec4 vs_TEXCOORD1;
in highp vec4 vs_TEXCOORD2;
in highp vec4 vs_TEXCOORD3;
layout(location = 0) out mediump vec4 SV_Target0;
vec4 u_xlat0;
mediump vec3 u_xlat16_0;
vec2 u_xlat1;
mediump vec3 u_xlat16_1;
bool u_xlatb1;
mediump vec3 u_xlat16_2;
mediump float u_xlat16_3;
mediump vec3 u_xlat16_4;
vec2 u_xlat5;
mediump vec3 u_xlat16_5;
vec2 u_xlat6;
mediump vec3 u_xlat16_6;
vec2 u_xlat7;
mediump vec3 u_xlat16_7;
mediump vec3 u_xlat16_8;
mediump vec3 u_xlat16_11;
mediump vec3 u_xlat16_12;
mediump float u_xlat16_20;
mediump float u_xlat16_21;
bool u_xlatb27;
mediump float u_xlat16_29;
mediump float u_xlat16_30;
void main()
{
    u_xlat0 = vs_TEXCOORD1.xyxw * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_1.xyz = textureLod(_MainTex, u_xlat0.xy, 0.0).xyz;
    u_xlat16_2.x = dot(u_xlat16_1.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat16_0.xyz = textureLod(_MainTex, u_xlat0.zw, 0.0).xyz;
    u_xlat16_11.x = dot(u_xlat16_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat0 = vs_TEXCOORD1.zyzw * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_1.xyz = textureLod(_MainTex, u_xlat0.xy, 0.0).xyz;
    u_xlat16_20 = dot(u_xlat16_1.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat16_0.xyz = textureLod(_MainTex, u_xlat0.zw, 0.0).xyz;
    u_xlat16_11.z = dot(u_xlat16_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat0.xy = vs_TEXCOORD0.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_0.xyz = textureLod(_MainTex, u_xlat0.xy, 0.0).xyz;
    u_xlat16_3 = dot(u_xlat16_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat16_2.z = u_xlat16_20 + 0.00260416674;
    u_xlat16_21 = min(u_xlat16_11.x, u_xlat16_2.x);
    u_xlat16_12.xz = max(u_xlat16_11.xz, u_xlat16_2.xz);
    u_xlat16_4.x = min(u_xlat16_11.z, u_xlat16_2.z);
    u_xlat16_12.x = max(u_xlat16_12.x, u_xlat16_12.z);
    u_xlat16_21 = min(u_xlat16_21, u_xlat16_4.x);
    u_xlat16_30 = u_xlat16_12.x * _EdgeThreshold;
    u_xlat16_4.x = min(u_xlat16_3, u_xlat16_21);
    u_xlat16_30 = max(u_xlat16_30, _EdgeThresholdMin);
    u_xlat16_3 = max(u_xlat16_3, u_xlat16_12.x);
    u_xlat16_3 = (-u_xlat16_4.x) + u_xlat16_3;
#ifdef UNITY_ADRENO_ES3
    u_xlatb27 = !!(u_xlat16_3>=u_xlat16_30);
#else
    u_xlatb27 = u_xlat16_3>=u_xlat16_30;
#endif
    if(u_xlatb27){
        u_xlat16_2.xy = (-u_xlat16_2.xz) + u_xlat16_11.zx;
        u_xlat16_4.x = u_xlat16_2.x + u_xlat16_2.y;
        u_xlat16_4.y = (-u_xlat16_2.x) + u_xlat16_2.y;
        u_xlat16_2.x = dot(u_xlat16_4.xy, u_xlat16_4.xy);
        u_xlat16_2.x = inversesqrt(u_xlat16_2.x);
        u_xlat16_2.xy = u_xlat16_2.xx * u_xlat16_4.xy;
        u_xlat1.xy = (-u_xlat16_2.xy) * vs_TEXCOORD2.zw + vs_TEXCOORD0.xy;
        u_xlat1.xy = u_xlat1.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat16_1.xyz = textureLod(_MainTex, u_xlat1.xy, 0.0).xyz;
        u_xlat5.xy = u_xlat16_2.xy * vs_TEXCOORD2.zw + vs_TEXCOORD0.xy;
        u_xlat5.xy = u_xlat5.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat16_5.xyz = textureLod(_MainTex, u_xlat5.xy, 0.0).xyz;
        u_xlat16_20 = min(abs(u_xlat16_2.y), abs(u_xlat16_2.x));
        u_xlat16_20 = u_xlat16_20 * _EdgeSharpness;
        u_xlat16_2.xy = u_xlat16_2.xy / vec2(u_xlat16_20);
        u_xlat16_2.xy = max(u_xlat16_2.xy, vec2(-2.0, -2.0));
        u_xlat16_2.xy = min(u_xlat16_2.xy, vec2(2.0, 2.0));
        u_xlat6.xy = (-u_xlat16_2.xy) * vs_TEXCOORD3.zw + vs_TEXCOORD0.xy;
        u_xlat6.xy = u_xlat6.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat16_6.xyz = textureLod(_MainTex, u_xlat6.xy, 0.0).xyz;
        u_xlat7.xy = u_xlat16_2.xy * vs_TEXCOORD3.zw + vs_TEXCOORD0.xy;
        u_xlat7.xy = u_xlat7.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat16_7.xyz = textureLod(_MainTex, u_xlat7.xy, 0.0).xyz;
        u_xlat16_2.xyz = u_xlat16_1.xyz + u_xlat16_5.xyz;
        u_xlat16_4.xyz = u_xlat16_6.xyz + u_xlat16_7.xyz;
        u_xlat16_8.xyz = u_xlat16_2.xyz * vec3(0.25, 0.25, 0.25);
        u_xlat16_4.xyz = u_xlat16_4.xyz * vec3(0.25, 0.25, 0.25) + u_xlat16_8.xyz;
        u_xlat16_29 = dot(u_xlat16_2.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
#ifdef UNITY_ADRENO_ES3
        u_xlatb27 = !!(u_xlat16_29<u_xlat16_21);
#else
        u_xlatb27 = u_xlat16_29<u_xlat16_21;
#endif
        u_xlat16_29 = dot(u_xlat16_4.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
#ifdef UNITY_ADRENO_ES3
        u_xlatb1 = !!(u_xlat16_12.x<u_xlat16_29);
#else
        u_xlatb1 = u_xlat16_12.x<u_xlat16_29;
#endif
        u_xlatb27 = u_xlatb27 || u_xlatb1;
        u_xlat16_2.xyz = u_xlat16_2.xyz * vec3(0.5, 0.5, 0.5);
        u_xlat16_2.xyz = (bool(u_xlatb27)) ? u_xlat16_2.xyz : u_xlat16_4.xyz;
    } else {
        u_xlat16_2.xyz = u_xlat16_0.xyz;
    //ENDIF
    }
    SV_Target0.xyz = u_xlat16_2.xyz;
    SV_Target0.w = 1.0;
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
out highp vec2 vs_TEXCOORD0;
out highp vec4 vs_TEXCOORD1;
out highp vec4 vs_TEXCOORD2;
out highp vec4 vs_TEXCOORD3;
vec4 u_xlat0;
vec4 u_xlat1;
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
    vs_TEXCOORD0.xy = in_TEXCOORD0.xy;
    vs_TEXCOORD1.xy = (-_MainTex_TexelSize.xy) * vec2(0.5, 0.5) + in_TEXCOORD0.xy;
    vs_TEXCOORD1.zw = _MainTex_TexelSize.xy * vec2(0.5, 0.5) + in_TEXCOORD0.xy;
    u_xlat0 = _MainTex_TexelSize.xyxy * vec4(-0.5, -0.5, 0.5, 0.5);
    vs_TEXCOORD2 = u_xlat0;
    vs_TEXCOORD3.xy = _MainTex_TexelSize.xy * vec2(-2.0, -2.0);
    vs_TEXCOORD3.zw = _MainTex_TexelSize.xy + _MainTex_TexelSize.xy;
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
uniform 	mediump float _EdgeThresholdMin;
uniform 	mediump float _EdgeThreshold;
uniform 	mediump float _EdgeSharpness;
uniform 	mediump vec4 _MainTex_ST;
uniform mediump sampler2D _MainTex;
in highp vec2 vs_TEXCOORD0;
in highp vec4 vs_TEXCOORD1;
in highp vec4 vs_TEXCOORD2;
in highp vec4 vs_TEXCOORD3;
layout(location = 0) out mediump vec4 SV_Target0;
vec4 u_xlat0;
mediump vec3 u_xlat16_0;
vec2 u_xlat1;
mediump vec3 u_xlat16_1;
bool u_xlatb1;
mediump vec3 u_xlat16_2;
mediump float u_xlat16_3;
mediump vec3 u_xlat16_4;
vec2 u_xlat5;
mediump vec3 u_xlat16_5;
vec2 u_xlat6;
mediump vec3 u_xlat16_6;
vec2 u_xlat7;
mediump vec3 u_xlat16_7;
mediump vec3 u_xlat16_8;
mediump vec3 u_xlat16_11;
mediump vec3 u_xlat16_12;
mediump float u_xlat16_20;
mediump float u_xlat16_21;
bool u_xlatb27;
mediump float u_xlat16_29;
mediump float u_xlat16_30;
void main()
{
    u_xlat0 = vs_TEXCOORD1.xyxw * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_1.xyz = textureLod(_MainTex, u_xlat0.xy, 0.0).xyz;
    u_xlat16_2.x = dot(u_xlat16_1.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat16_0.xyz = textureLod(_MainTex, u_xlat0.zw, 0.0).xyz;
    u_xlat16_11.x = dot(u_xlat16_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat0 = vs_TEXCOORD1.zyzw * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_1.xyz = textureLod(_MainTex, u_xlat0.xy, 0.0).xyz;
    u_xlat16_20 = dot(u_xlat16_1.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat16_0.xyz = textureLod(_MainTex, u_xlat0.zw, 0.0).xyz;
    u_xlat16_11.z = dot(u_xlat16_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat0.xy = vs_TEXCOORD0.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_0.xyz = textureLod(_MainTex, u_xlat0.xy, 0.0).xyz;
    u_xlat16_3 = dot(u_xlat16_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat16_2.z = u_xlat16_20 + 0.00260416674;
    u_xlat16_21 = min(u_xlat16_11.x, u_xlat16_2.x);
    u_xlat16_12.xz = max(u_xlat16_11.xz, u_xlat16_2.xz);
    u_xlat16_4.x = min(u_xlat16_11.z, u_xlat16_2.z);
    u_xlat16_12.x = max(u_xlat16_12.x, u_xlat16_12.z);
    u_xlat16_21 = min(u_xlat16_21, u_xlat16_4.x);
    u_xlat16_30 = u_xlat16_12.x * _EdgeThreshold;
    u_xlat16_4.x = min(u_xlat16_3, u_xlat16_21);
    u_xlat16_30 = max(u_xlat16_30, _EdgeThresholdMin);
    u_xlat16_3 = max(u_xlat16_3, u_xlat16_12.x);
    u_xlat16_3 = (-u_xlat16_4.x) + u_xlat16_3;
#ifdef UNITY_ADRENO_ES3
    u_xlatb27 = !!(u_xlat16_3>=u_xlat16_30);
#else
    u_xlatb27 = u_xlat16_3>=u_xlat16_30;
#endif
    if(u_xlatb27){
        u_xlat16_2.xy = (-u_xlat16_2.xz) + u_xlat16_11.zx;
        u_xlat16_4.x = u_xlat16_2.x + u_xlat16_2.y;
        u_xlat16_4.y = (-u_xlat16_2.x) + u_xlat16_2.y;
        u_xlat16_2.x = dot(u_xlat16_4.xy, u_xlat16_4.xy);
        u_xlat16_2.x = inversesqrt(u_xlat16_2.x);
        u_xlat16_2.xy = u_xlat16_2.xx * u_xlat16_4.xy;
        u_xlat1.xy = (-u_xlat16_2.xy) * vs_TEXCOORD2.zw + vs_TEXCOORD0.xy;
        u_xlat1.xy = u_xlat1.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat16_1.xyz = textureLod(_MainTex, u_xlat1.xy, 0.0).xyz;
        u_xlat5.xy = u_xlat16_2.xy * vs_TEXCOORD2.zw + vs_TEXCOORD0.xy;
        u_xlat5.xy = u_xlat5.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat16_5.xyz = textureLod(_MainTex, u_xlat5.xy, 0.0).xyz;
        u_xlat16_20 = min(abs(u_xlat16_2.y), abs(u_xlat16_2.x));
        u_xlat16_20 = u_xlat16_20 * _EdgeSharpness;
        u_xlat16_2.xy = u_xlat16_2.xy / vec2(u_xlat16_20);
        u_xlat16_2.xy = max(u_xlat16_2.xy, vec2(-2.0, -2.0));
        u_xlat16_2.xy = min(u_xlat16_2.xy, vec2(2.0, 2.0));
        u_xlat6.xy = (-u_xlat16_2.xy) * vs_TEXCOORD3.zw + vs_TEXCOORD0.xy;
        u_xlat6.xy = u_xlat6.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat16_6.xyz = textureLod(_MainTex, u_xlat6.xy, 0.0).xyz;
        u_xlat7.xy = u_xlat16_2.xy * vs_TEXCOORD3.zw + vs_TEXCOORD0.xy;
        u_xlat7.xy = u_xlat7.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat16_7.xyz = textureLod(_MainTex, u_xlat7.xy, 0.0).xyz;
        u_xlat16_2.xyz = u_xlat16_1.xyz + u_xlat16_5.xyz;
        u_xlat16_4.xyz = u_xlat16_6.xyz + u_xlat16_7.xyz;
        u_xlat16_8.xyz = u_xlat16_2.xyz * vec3(0.25, 0.25, 0.25);
        u_xlat16_4.xyz = u_xlat16_4.xyz * vec3(0.25, 0.25, 0.25) + u_xlat16_8.xyz;
        u_xlat16_29 = dot(u_xlat16_2.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
#ifdef UNITY_ADRENO_ES3
        u_xlatb27 = !!(u_xlat16_29<u_xlat16_21);
#else
        u_xlatb27 = u_xlat16_29<u_xlat16_21;
#endif
        u_xlat16_29 = dot(u_xlat16_4.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
#ifdef UNITY_ADRENO_ES3
        u_xlatb1 = !!(u_xlat16_12.x<u_xlat16_29);
#else
        u_xlatb1 = u_xlat16_12.x<u_xlat16_29;
#endif
        u_xlatb27 = u_xlatb27 || u_xlatb1;
        u_xlat16_2.xyz = u_xlat16_2.xyz * vec3(0.5, 0.5, 0.5);
        u_xlat16_2.xyz = (bool(u_xlatb27)) ? u_xlat16_2.xyz : u_xlat16_4.xyz;
    } else {
        u_xlat16_2.xyz = u_xlat16_0.xyz;
    //ENDIF
    }
    SV_Target0.xyz = u_xlat16_2.xyz;
    SV_Target0.w = 1.0;
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
out highp vec2 vs_TEXCOORD0;
out highp vec4 vs_TEXCOORD1;
out highp vec4 vs_TEXCOORD2;
out highp vec4 vs_TEXCOORD3;
vec4 u_xlat0;
vec4 u_xlat1;
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
    vs_TEXCOORD0.xy = in_TEXCOORD0.xy;
    vs_TEXCOORD1.xy = (-_MainTex_TexelSize.xy) * vec2(0.5, 0.5) + in_TEXCOORD0.xy;
    vs_TEXCOORD1.zw = _MainTex_TexelSize.xy * vec2(0.5, 0.5) + in_TEXCOORD0.xy;
    u_xlat0 = _MainTex_TexelSize.xyxy * vec4(-0.5, -0.5, 0.5, 0.5);
    vs_TEXCOORD2 = u_xlat0;
    vs_TEXCOORD3.xy = _MainTex_TexelSize.xy * vec2(-2.0, -2.0);
    vs_TEXCOORD3.zw = _MainTex_TexelSize.xy + _MainTex_TexelSize.xy;
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
uniform 	mediump float _EdgeThresholdMin;
uniform 	mediump float _EdgeThreshold;
uniform 	mediump float _EdgeSharpness;
uniform 	mediump vec4 _MainTex_ST;
uniform mediump sampler2D _MainTex;
in highp vec2 vs_TEXCOORD0;
in highp vec4 vs_TEXCOORD1;
in highp vec4 vs_TEXCOORD2;
in highp vec4 vs_TEXCOORD3;
layout(location = 0) out mediump vec4 SV_Target0;
vec4 u_xlat0;
mediump vec3 u_xlat16_0;
vec2 u_xlat1;
mediump vec3 u_xlat16_1;
bool u_xlatb1;
mediump vec3 u_xlat16_2;
mediump float u_xlat16_3;
mediump vec3 u_xlat16_4;
vec2 u_xlat5;
mediump vec3 u_xlat16_5;
vec2 u_xlat6;
mediump vec3 u_xlat16_6;
vec2 u_xlat7;
mediump vec3 u_xlat16_7;
mediump vec3 u_xlat16_8;
mediump vec3 u_xlat16_11;
mediump vec3 u_xlat16_12;
mediump float u_xlat16_20;
mediump float u_xlat16_21;
bool u_xlatb27;
mediump float u_xlat16_29;
mediump float u_xlat16_30;
void main()
{
    u_xlat0 = vs_TEXCOORD1.xyxw * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_1.xyz = textureLod(_MainTex, u_xlat0.xy, 0.0).xyz;
    u_xlat16_2.x = dot(u_xlat16_1.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat16_0.xyz = textureLod(_MainTex, u_xlat0.zw, 0.0).xyz;
    u_xlat16_11.x = dot(u_xlat16_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat0 = vs_TEXCOORD1.zyzw * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_1.xyz = textureLod(_MainTex, u_xlat0.xy, 0.0).xyz;
    u_xlat16_20 = dot(u_xlat16_1.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat16_0.xyz = textureLod(_MainTex, u_xlat0.zw, 0.0).xyz;
    u_xlat16_11.z = dot(u_xlat16_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat0.xy = vs_TEXCOORD0.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_0.xyz = textureLod(_MainTex, u_xlat0.xy, 0.0).xyz;
    u_xlat16_3 = dot(u_xlat16_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat16_2.z = u_xlat16_20 + 0.00260416674;
    u_xlat16_21 = min(u_xlat16_11.x, u_xlat16_2.x);
    u_xlat16_12.xz = max(u_xlat16_11.xz, u_xlat16_2.xz);
    u_xlat16_4.x = min(u_xlat16_11.z, u_xlat16_2.z);
    u_xlat16_12.x = max(u_xlat16_12.x, u_xlat16_12.z);
    u_xlat16_21 = min(u_xlat16_21, u_xlat16_4.x);
    u_xlat16_30 = u_xlat16_12.x * _EdgeThreshold;
    u_xlat16_4.x = min(u_xlat16_3, u_xlat16_21);
    u_xlat16_30 = max(u_xlat16_30, _EdgeThresholdMin);
    u_xlat16_3 = max(u_xlat16_3, u_xlat16_12.x);
    u_xlat16_3 = (-u_xlat16_4.x) + u_xlat16_3;
#ifdef UNITY_ADRENO_ES3
    u_xlatb27 = !!(u_xlat16_3>=u_xlat16_30);
#else
    u_xlatb27 = u_xlat16_3>=u_xlat16_30;
#endif
    if(u_xlatb27){
        u_xlat16_2.xy = (-u_xlat16_2.xz) + u_xlat16_11.zx;
        u_xlat16_4.x = u_xlat16_2.x + u_xlat16_2.y;
        u_xlat16_4.y = (-u_xlat16_2.x) + u_xlat16_2.y;
        u_xlat16_2.x = dot(u_xlat16_4.xy, u_xlat16_4.xy);
        u_xlat16_2.x = inversesqrt(u_xlat16_2.x);
        u_xlat16_2.xy = u_xlat16_2.xx * u_xlat16_4.xy;
        u_xlat1.xy = (-u_xlat16_2.xy) * vs_TEXCOORD2.zw + vs_TEXCOORD0.xy;
        u_xlat1.xy = u_xlat1.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat16_1.xyz = textureLod(_MainTex, u_xlat1.xy, 0.0).xyz;
        u_xlat5.xy = u_xlat16_2.xy * vs_TEXCOORD2.zw + vs_TEXCOORD0.xy;
        u_xlat5.xy = u_xlat5.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat16_5.xyz = textureLod(_MainTex, u_xlat5.xy, 0.0).xyz;
        u_xlat16_20 = min(abs(u_xlat16_2.y), abs(u_xlat16_2.x));
        u_xlat16_20 = u_xlat16_20 * _EdgeSharpness;
        u_xlat16_2.xy = u_xlat16_2.xy / vec2(u_xlat16_20);
        u_xlat16_2.xy = max(u_xlat16_2.xy, vec2(-2.0, -2.0));
        u_xlat16_2.xy = min(u_xlat16_2.xy, vec2(2.0, 2.0));
        u_xlat6.xy = (-u_xlat16_2.xy) * vs_TEXCOORD3.zw + vs_TEXCOORD0.xy;
        u_xlat6.xy = u_xlat6.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat16_6.xyz = textureLod(_MainTex, u_xlat6.xy, 0.0).xyz;
        u_xlat7.xy = u_xlat16_2.xy * vs_TEXCOORD3.zw + vs_TEXCOORD0.xy;
        u_xlat7.xy = u_xlat7.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat16_7.xyz = textureLod(_MainTex, u_xlat7.xy, 0.0).xyz;
        u_xlat16_2.xyz = u_xlat16_1.xyz + u_xlat16_5.xyz;
        u_xlat16_4.xyz = u_xlat16_6.xyz + u_xlat16_7.xyz;
        u_xlat16_8.xyz = u_xlat16_2.xyz * vec3(0.25, 0.25, 0.25);
        u_xlat16_4.xyz = u_xlat16_4.xyz * vec3(0.25, 0.25, 0.25) + u_xlat16_8.xyz;
        u_xlat16_29 = dot(u_xlat16_2.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
#ifdef UNITY_ADRENO_ES3
        u_xlatb27 = !!(u_xlat16_29<u_xlat16_21);
#else
        u_xlatb27 = u_xlat16_29<u_xlat16_21;
#endif
        u_xlat16_29 = dot(u_xlat16_4.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
#ifdef UNITY_ADRENO_ES3
        u_xlatb1 = !!(u_xlat16_12.x<u_xlat16_29);
#else
        u_xlatb1 = u_xlat16_12.x<u_xlat16_29;
#endif
        u_xlatb27 = u_xlatb27 || u_xlatb1;
        u_xlat16_2.xyz = u_xlat16_2.xyz * vec3(0.5, 0.5, 0.5);
        u_xlat16_2.xyz = (bool(u_xlatb27)) ? u_xlat16_2.xyz : u_xlat16_4.xyz;
    } else {
        u_xlat16_2.xyz = u_xlat16_0.xyz;
    //ENDIF
    }
    SV_Target0.xyz = u_xlat16_2.xyz;
    SV_Target0.w = 1.0;
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