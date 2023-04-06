//////////////////////////////////////////
//
// NOTE: This is *not* a valid shader file
//
///////////////////////////////////////////
Shader "Hidden/NFAA" {
Properties {
_MainTex ("Base (RGB)", 2D) = "white" { }
_BlurTex ("Base (RGB)", 2D) = "white" { }
}
SubShader {
 Pass {
  ZTest Always
  ZWrite Off
  Cull Off
  GpuProgramID 46250
Program "vp" {
SubProgram "gles hw_tier00 " {
"#ifdef VERTEX
#version 100

uniform 	vec4 hlslcc_mtx4x4unity_ObjectToWorld[4];
uniform 	vec4 hlslcc_mtx4x4unity_MatrixVP[4];
uniform 	vec4 _MainTex_TexelSize;
uniform 	float _OffsetScale;
attribute highp vec4 in_POSITION0;
attribute mediump vec2 in_TEXCOORD0;
varying highp vec2 vs_TEXCOORD0;
varying highp vec2 vs_TEXCOORD1;
varying highp vec2 vs_TEXCOORD2;
varying highp vec2 vs_TEXCOORD3;
varying highp vec2 vs_TEXCOORD4;
varying highp vec2 vs_TEXCOORD5;
varying highp vec2 vs_TEXCOORD6;
varying highp vec2 vs_TEXCOORD7;
vec4 u_xlat0;
vec4 u_xlat1;
vec2 u_xlat4;
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
    u_xlat0.yz = _MainTex_TexelSize.yx * vec2(_OffsetScale);
    u_xlat0.x = float(0.0);
    u_xlat0.w = float(0.0);
    vs_TEXCOORD0.xy = u_xlat0.xy + in_TEXCOORD0.xy;
    vs_TEXCOORD1.xy = (-u_xlat0.xy) + in_TEXCOORD0.xy;
    u_xlat1.xy = u_xlat0.zw + in_TEXCOORD0.xy;
    vs_TEXCOORD2.xy = u_xlat1.xy;
    u_xlat4.xy = (-u_xlat0.zw) + in_TEXCOORD0.xy;
    vs_TEXCOORD3.xy = u_xlat4.xy;
    vs_TEXCOORD4.xy = u_xlat0.xy + u_xlat4.xy;
    vs_TEXCOORD5.xy = (-u_xlat0.xy) + u_xlat4.xy;
    vs_TEXCOORD6.xy = u_xlat0.xy + u_xlat1.xy;
    vs_TEXCOORD7.xy = (-u_xlat0.xy) + u_xlat1.xy;
    return;
}

#endif
#ifdef FRAGMENT
#version 100

#ifdef GL_FRAGMENT_PRECISION_HIGH
    precision highp float;
#else
    precision mediump float;
#endif
precision highp int;
uniform 	vec4 _MainTex_TexelSize;
uniform 	mediump vec4 _MainTex_ST;
uniform 	float _BlurRadius;
uniform lowp sampler2D _MainTex;
varying highp vec2 vs_TEXCOORD0;
varying highp vec2 vs_TEXCOORD1;
varying highp vec2 vs_TEXCOORD2;
varying highp vec2 vs_TEXCOORD3;
varying highp vec2 vs_TEXCOORD4;
varying highp vec2 vs_TEXCOORD5;
varying highp vec2 vs_TEXCOORD6;
varying highp vec2 vs_TEXCOORD7;
#define SV_Target0 gl_FragData[0]
vec4 u_xlat0;
lowp vec4 u_xlat10_0;
vec4 u_xlat1;
mediump vec3 u_xlat16_1;
lowp vec4 u_xlat10_1;
mediump vec3 u_xlat16_2;
lowp vec4 u_xlat10_2;
mediump vec3 u_xlat16_3;
vec2 u_xlat4;
lowp vec3 u_xlat10_4;
vec3 u_xlat5;
lowp vec3 u_xlat10_5;
vec2 u_xlat10;
vec2 u_xlat14;
float u_xlat15;
void main()
{
    u_xlat0.xy = vs_TEXCOORD2.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_0.xyz = texture2D(_MainTex, u_xlat0.xy).xyz;
    u_xlat16_1.y = dot(u_xlat10_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat0.xy = vs_TEXCOORD4.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_0.xyz = texture2D(_MainTex, u_xlat0.xy).xyz;
    u_xlat16_2.z = dot(u_xlat10_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat16_1.x = u_xlat16_2.z;
    u_xlat0.xy = vs_TEXCOORD5.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_0.xyz = texture2D(_MainTex, u_xlat0.xy).xyz;
    u_xlat16_1.z = dot(u_xlat10_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat0.x = dot(vec3(1.0, 1.0, 1.0), u_xlat16_1.xyz);
    u_xlat5.xy = vs_TEXCOORD3.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_5.xyz = texture2D(_MainTex, u_xlat5.xy).xyz;
    u_xlat16_3.y = dot(u_xlat10_5.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat5.xy = vs_TEXCOORD6.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_5.xyz = texture2D(_MainTex, u_xlat5.xy).xyz;
    u_xlat16_1.y = dot(u_xlat10_5.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat16_3.x = u_xlat16_1.y;
    u_xlat5.xy = vs_TEXCOORD7.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_5.xyz = texture2D(_MainTex, u_xlat5.xy).xyz;
    u_xlat16_2.x = dot(u_xlat10_5.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat16_3.z = u_xlat16_2.x;
    u_xlat5.x = dot(vec3(1.0, 1.0, 1.0), u_xlat16_3.xyz);
    u_xlat0.y = (-u_xlat0.x) + u_xlat5.x;
    u_xlat10.xy = vs_TEXCOORD1.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_4.xyz = texture2D(_MainTex, u_xlat10.xy).xyz;
    u_xlat16_2.y = dot(u_xlat10_4.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat10.x = dot(vec3(1.0, 1.0, 1.0), u_xlat16_2.xyz);
    u_xlat4.xy = vs_TEXCOORD0.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_4.xyz = texture2D(_MainTex, u_xlat4.xy).xyz;
    u_xlat16_1.x = dot(u_xlat10_4.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat15 = dot(vec3(1.0, 1.0, 1.0), u_xlat16_1.xyz);
    u_xlat0.x = (-u_xlat15) + u_xlat10.x;
    u_xlat10.xy = _MainTex_TexelSize.xy * vec2(vec2(_BlurRadius, _BlurRadius));
    u_xlat0.xy = u_xlat10.xy * u_xlat0.xy;
    u_xlat4.xy = vs_TEXCOORD0.xy + vs_TEXCOORD1.xy;
    u_xlat14.xy = u_xlat4.xy * vec2(0.5, 0.5) + u_xlat0.xy;
    u_xlat14.xy = u_xlat14.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_1 = texture2D(_MainTex, u_xlat14.xy);
    u_xlat14.xy = u_xlat4.xy * vec2(0.5, 0.5);
    u_xlat14.xy = u_xlat14.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_2 = texture2D(_MainTex, u_xlat14.xy);
    u_xlat1 = u_xlat10_1 + u_xlat10_2;
    u_xlat14.xy = u_xlat4.xy * vec2(0.5, 0.5) + (-u_xlat0.xy);
    u_xlat14.xy = u_xlat14.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_2 = texture2D(_MainTex, u_xlat14.xy);
    u_xlat1 = u_xlat1 + u_xlat10_2;
    u_xlat0.z = (-u_xlat0.y);
    u_xlat5.xz = u_xlat4.xy * vec2(0.5, 0.5) + u_xlat0.xz;
    u_xlat0.xz = u_xlat4.xy * vec2(0.5, 0.5) + (-u_xlat0.xz);
    u_xlat0.xz = u_xlat0.xz * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_2 = texture2D(_MainTex, u_xlat0.xz);
    u_xlat0.xy = u_xlat5.xz * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_0 = texture2D(_MainTex, u_xlat0.xy);
    u_xlat0 = u_xlat10_0 + u_xlat1;
    u_xlat0 = u_xlat10_2 + u_xlat0;
    u_xlat0 = u_xlat0 * vec4(0.200000003, 0.200000003, 0.200000003, 0.200000003);
    SV_Target0 = u_xlat0;
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
uniform 	float _OffsetScale;
attribute highp vec4 in_POSITION0;
attribute mediump vec2 in_TEXCOORD0;
varying highp vec2 vs_TEXCOORD0;
varying highp vec2 vs_TEXCOORD1;
varying highp vec2 vs_TEXCOORD2;
varying highp vec2 vs_TEXCOORD3;
varying highp vec2 vs_TEXCOORD4;
varying highp vec2 vs_TEXCOORD5;
varying highp vec2 vs_TEXCOORD6;
varying highp vec2 vs_TEXCOORD7;
vec4 u_xlat0;
vec4 u_xlat1;
vec2 u_xlat4;
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
    u_xlat0.yz = _MainTex_TexelSize.yx * vec2(_OffsetScale);
    u_xlat0.x = float(0.0);
    u_xlat0.w = float(0.0);
    vs_TEXCOORD0.xy = u_xlat0.xy + in_TEXCOORD0.xy;
    vs_TEXCOORD1.xy = (-u_xlat0.xy) + in_TEXCOORD0.xy;
    u_xlat1.xy = u_xlat0.zw + in_TEXCOORD0.xy;
    vs_TEXCOORD2.xy = u_xlat1.xy;
    u_xlat4.xy = (-u_xlat0.zw) + in_TEXCOORD0.xy;
    vs_TEXCOORD3.xy = u_xlat4.xy;
    vs_TEXCOORD4.xy = u_xlat0.xy + u_xlat4.xy;
    vs_TEXCOORD5.xy = (-u_xlat0.xy) + u_xlat4.xy;
    vs_TEXCOORD6.xy = u_xlat0.xy + u_xlat1.xy;
    vs_TEXCOORD7.xy = (-u_xlat0.xy) + u_xlat1.xy;
    return;
}

#endif
#ifdef FRAGMENT
#version 100

#ifdef GL_FRAGMENT_PRECISION_HIGH
    precision highp float;
#else
    precision mediump float;
#endif
precision highp int;
uniform 	vec4 _MainTex_TexelSize;
uniform 	mediump vec4 _MainTex_ST;
uniform 	float _BlurRadius;
uniform lowp sampler2D _MainTex;
varying highp vec2 vs_TEXCOORD0;
varying highp vec2 vs_TEXCOORD1;
varying highp vec2 vs_TEXCOORD2;
varying highp vec2 vs_TEXCOORD3;
varying highp vec2 vs_TEXCOORD4;
varying highp vec2 vs_TEXCOORD5;
varying highp vec2 vs_TEXCOORD6;
varying highp vec2 vs_TEXCOORD7;
#define SV_Target0 gl_FragData[0]
vec4 u_xlat0;
lowp vec4 u_xlat10_0;
vec4 u_xlat1;
mediump vec3 u_xlat16_1;
lowp vec4 u_xlat10_1;
mediump vec3 u_xlat16_2;
lowp vec4 u_xlat10_2;
mediump vec3 u_xlat16_3;
vec2 u_xlat4;
lowp vec3 u_xlat10_4;
vec3 u_xlat5;
lowp vec3 u_xlat10_5;
vec2 u_xlat10;
vec2 u_xlat14;
float u_xlat15;
void main()
{
    u_xlat0.xy = vs_TEXCOORD2.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_0.xyz = texture2D(_MainTex, u_xlat0.xy).xyz;
    u_xlat16_1.y = dot(u_xlat10_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat0.xy = vs_TEXCOORD4.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_0.xyz = texture2D(_MainTex, u_xlat0.xy).xyz;
    u_xlat16_2.z = dot(u_xlat10_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat16_1.x = u_xlat16_2.z;
    u_xlat0.xy = vs_TEXCOORD5.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_0.xyz = texture2D(_MainTex, u_xlat0.xy).xyz;
    u_xlat16_1.z = dot(u_xlat10_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat0.x = dot(vec3(1.0, 1.0, 1.0), u_xlat16_1.xyz);
    u_xlat5.xy = vs_TEXCOORD3.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_5.xyz = texture2D(_MainTex, u_xlat5.xy).xyz;
    u_xlat16_3.y = dot(u_xlat10_5.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat5.xy = vs_TEXCOORD6.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_5.xyz = texture2D(_MainTex, u_xlat5.xy).xyz;
    u_xlat16_1.y = dot(u_xlat10_5.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat16_3.x = u_xlat16_1.y;
    u_xlat5.xy = vs_TEXCOORD7.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_5.xyz = texture2D(_MainTex, u_xlat5.xy).xyz;
    u_xlat16_2.x = dot(u_xlat10_5.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat16_3.z = u_xlat16_2.x;
    u_xlat5.x = dot(vec3(1.0, 1.0, 1.0), u_xlat16_3.xyz);
    u_xlat0.y = (-u_xlat0.x) + u_xlat5.x;
    u_xlat10.xy = vs_TEXCOORD1.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_4.xyz = texture2D(_MainTex, u_xlat10.xy).xyz;
    u_xlat16_2.y = dot(u_xlat10_4.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat10.x = dot(vec3(1.0, 1.0, 1.0), u_xlat16_2.xyz);
    u_xlat4.xy = vs_TEXCOORD0.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_4.xyz = texture2D(_MainTex, u_xlat4.xy).xyz;
    u_xlat16_1.x = dot(u_xlat10_4.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat15 = dot(vec3(1.0, 1.0, 1.0), u_xlat16_1.xyz);
    u_xlat0.x = (-u_xlat15) + u_xlat10.x;
    u_xlat10.xy = _MainTex_TexelSize.xy * vec2(vec2(_BlurRadius, _BlurRadius));
    u_xlat0.xy = u_xlat10.xy * u_xlat0.xy;
    u_xlat4.xy = vs_TEXCOORD0.xy + vs_TEXCOORD1.xy;
    u_xlat14.xy = u_xlat4.xy * vec2(0.5, 0.5) + u_xlat0.xy;
    u_xlat14.xy = u_xlat14.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_1 = texture2D(_MainTex, u_xlat14.xy);
    u_xlat14.xy = u_xlat4.xy * vec2(0.5, 0.5);
    u_xlat14.xy = u_xlat14.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_2 = texture2D(_MainTex, u_xlat14.xy);
    u_xlat1 = u_xlat10_1 + u_xlat10_2;
    u_xlat14.xy = u_xlat4.xy * vec2(0.5, 0.5) + (-u_xlat0.xy);
    u_xlat14.xy = u_xlat14.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_2 = texture2D(_MainTex, u_xlat14.xy);
    u_xlat1 = u_xlat1 + u_xlat10_2;
    u_xlat0.z = (-u_xlat0.y);
    u_xlat5.xz = u_xlat4.xy * vec2(0.5, 0.5) + u_xlat0.xz;
    u_xlat0.xz = u_xlat4.xy * vec2(0.5, 0.5) + (-u_xlat0.xz);
    u_xlat0.xz = u_xlat0.xz * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_2 = texture2D(_MainTex, u_xlat0.xz);
    u_xlat0.xy = u_xlat5.xz * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_0 = texture2D(_MainTex, u_xlat0.xy);
    u_xlat0 = u_xlat10_0 + u_xlat1;
    u_xlat0 = u_xlat10_2 + u_xlat0;
    u_xlat0 = u_xlat0 * vec4(0.200000003, 0.200000003, 0.200000003, 0.200000003);
    SV_Target0 = u_xlat0;
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
uniform 	float _OffsetScale;
attribute highp vec4 in_POSITION0;
attribute mediump vec2 in_TEXCOORD0;
varying highp vec2 vs_TEXCOORD0;
varying highp vec2 vs_TEXCOORD1;
varying highp vec2 vs_TEXCOORD2;
varying highp vec2 vs_TEXCOORD3;
varying highp vec2 vs_TEXCOORD4;
varying highp vec2 vs_TEXCOORD5;
varying highp vec2 vs_TEXCOORD6;
varying highp vec2 vs_TEXCOORD7;
vec4 u_xlat0;
vec4 u_xlat1;
vec2 u_xlat4;
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
    u_xlat0.yz = _MainTex_TexelSize.yx * vec2(_OffsetScale);
    u_xlat0.x = float(0.0);
    u_xlat0.w = float(0.0);
    vs_TEXCOORD0.xy = u_xlat0.xy + in_TEXCOORD0.xy;
    vs_TEXCOORD1.xy = (-u_xlat0.xy) + in_TEXCOORD0.xy;
    u_xlat1.xy = u_xlat0.zw + in_TEXCOORD0.xy;
    vs_TEXCOORD2.xy = u_xlat1.xy;
    u_xlat4.xy = (-u_xlat0.zw) + in_TEXCOORD0.xy;
    vs_TEXCOORD3.xy = u_xlat4.xy;
    vs_TEXCOORD4.xy = u_xlat0.xy + u_xlat4.xy;
    vs_TEXCOORD5.xy = (-u_xlat0.xy) + u_xlat4.xy;
    vs_TEXCOORD6.xy = u_xlat0.xy + u_xlat1.xy;
    vs_TEXCOORD7.xy = (-u_xlat0.xy) + u_xlat1.xy;
    return;
}

#endif
#ifdef FRAGMENT
#version 100

#ifdef GL_FRAGMENT_PRECISION_HIGH
    precision highp float;
#else
    precision mediump float;
#endif
precision highp int;
uniform 	vec4 _MainTex_TexelSize;
uniform 	mediump vec4 _MainTex_ST;
uniform 	float _BlurRadius;
uniform lowp sampler2D _MainTex;
varying highp vec2 vs_TEXCOORD0;
varying highp vec2 vs_TEXCOORD1;
varying highp vec2 vs_TEXCOORD2;
varying highp vec2 vs_TEXCOORD3;
varying highp vec2 vs_TEXCOORD4;
varying highp vec2 vs_TEXCOORD5;
varying highp vec2 vs_TEXCOORD6;
varying highp vec2 vs_TEXCOORD7;
#define SV_Target0 gl_FragData[0]
vec4 u_xlat0;
lowp vec4 u_xlat10_0;
vec4 u_xlat1;
mediump vec3 u_xlat16_1;
lowp vec4 u_xlat10_1;
mediump vec3 u_xlat16_2;
lowp vec4 u_xlat10_2;
mediump vec3 u_xlat16_3;
vec2 u_xlat4;
lowp vec3 u_xlat10_4;
vec3 u_xlat5;
lowp vec3 u_xlat10_5;
vec2 u_xlat10;
vec2 u_xlat14;
float u_xlat15;
void main()
{
    u_xlat0.xy = vs_TEXCOORD2.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_0.xyz = texture2D(_MainTex, u_xlat0.xy).xyz;
    u_xlat16_1.y = dot(u_xlat10_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat0.xy = vs_TEXCOORD4.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_0.xyz = texture2D(_MainTex, u_xlat0.xy).xyz;
    u_xlat16_2.z = dot(u_xlat10_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat16_1.x = u_xlat16_2.z;
    u_xlat0.xy = vs_TEXCOORD5.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_0.xyz = texture2D(_MainTex, u_xlat0.xy).xyz;
    u_xlat16_1.z = dot(u_xlat10_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat0.x = dot(vec3(1.0, 1.0, 1.0), u_xlat16_1.xyz);
    u_xlat5.xy = vs_TEXCOORD3.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_5.xyz = texture2D(_MainTex, u_xlat5.xy).xyz;
    u_xlat16_3.y = dot(u_xlat10_5.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat5.xy = vs_TEXCOORD6.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_5.xyz = texture2D(_MainTex, u_xlat5.xy).xyz;
    u_xlat16_1.y = dot(u_xlat10_5.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat16_3.x = u_xlat16_1.y;
    u_xlat5.xy = vs_TEXCOORD7.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_5.xyz = texture2D(_MainTex, u_xlat5.xy).xyz;
    u_xlat16_2.x = dot(u_xlat10_5.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat16_3.z = u_xlat16_2.x;
    u_xlat5.x = dot(vec3(1.0, 1.0, 1.0), u_xlat16_3.xyz);
    u_xlat0.y = (-u_xlat0.x) + u_xlat5.x;
    u_xlat10.xy = vs_TEXCOORD1.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_4.xyz = texture2D(_MainTex, u_xlat10.xy).xyz;
    u_xlat16_2.y = dot(u_xlat10_4.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat10.x = dot(vec3(1.0, 1.0, 1.0), u_xlat16_2.xyz);
    u_xlat4.xy = vs_TEXCOORD0.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_4.xyz = texture2D(_MainTex, u_xlat4.xy).xyz;
    u_xlat16_1.x = dot(u_xlat10_4.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat15 = dot(vec3(1.0, 1.0, 1.0), u_xlat16_1.xyz);
    u_xlat0.x = (-u_xlat15) + u_xlat10.x;
    u_xlat10.xy = _MainTex_TexelSize.xy * vec2(vec2(_BlurRadius, _BlurRadius));
    u_xlat0.xy = u_xlat10.xy * u_xlat0.xy;
    u_xlat4.xy = vs_TEXCOORD0.xy + vs_TEXCOORD1.xy;
    u_xlat14.xy = u_xlat4.xy * vec2(0.5, 0.5) + u_xlat0.xy;
    u_xlat14.xy = u_xlat14.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_1 = texture2D(_MainTex, u_xlat14.xy);
    u_xlat14.xy = u_xlat4.xy * vec2(0.5, 0.5);
    u_xlat14.xy = u_xlat14.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_2 = texture2D(_MainTex, u_xlat14.xy);
    u_xlat1 = u_xlat10_1 + u_xlat10_2;
    u_xlat14.xy = u_xlat4.xy * vec2(0.5, 0.5) + (-u_xlat0.xy);
    u_xlat14.xy = u_xlat14.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_2 = texture2D(_MainTex, u_xlat14.xy);
    u_xlat1 = u_xlat1 + u_xlat10_2;
    u_xlat0.z = (-u_xlat0.y);
    u_xlat5.xz = u_xlat4.xy * vec2(0.5, 0.5) + u_xlat0.xz;
    u_xlat0.xz = u_xlat4.xy * vec2(0.5, 0.5) + (-u_xlat0.xz);
    u_xlat0.xz = u_xlat0.xz * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_2 = texture2D(_MainTex, u_xlat0.xz);
    u_xlat0.xy = u_xlat5.xz * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_0 = texture2D(_MainTex, u_xlat0.xy);
    u_xlat0 = u_xlat10_0 + u_xlat1;
    u_xlat0 = u_xlat10_2 + u_xlat0;
    u_xlat0 = u_xlat0 * vec4(0.200000003, 0.200000003, 0.200000003, 0.200000003);
    SV_Target0 = u_xlat0;
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
uniform 	float _OffsetScale;
in highp vec4 in_POSITION0;
in mediump vec2 in_TEXCOORD0;
out highp vec2 vs_TEXCOORD0;
out highp vec2 vs_TEXCOORD1;
out highp vec2 vs_TEXCOORD2;
out highp vec2 vs_TEXCOORD3;
out highp vec2 vs_TEXCOORD4;
out highp vec2 vs_TEXCOORD5;
out highp vec2 vs_TEXCOORD6;
out highp vec2 vs_TEXCOORD7;
vec4 u_xlat0;
vec4 u_xlat1;
vec2 u_xlat4;
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
    u_xlat0.yz = _MainTex_TexelSize.yx * vec2(_OffsetScale);
    u_xlat0.x = float(0.0);
    u_xlat0.w = float(0.0);
    vs_TEXCOORD0.xy = u_xlat0.xy + in_TEXCOORD0.xy;
    vs_TEXCOORD1.xy = (-u_xlat0.xy) + in_TEXCOORD0.xy;
    u_xlat1.xy = u_xlat0.zw + in_TEXCOORD0.xy;
    vs_TEXCOORD2.xy = u_xlat1.xy;
    u_xlat4.xy = (-u_xlat0.zw) + in_TEXCOORD0.xy;
    vs_TEXCOORD3.xy = u_xlat4.xy;
    vs_TEXCOORD4.xy = u_xlat0.xy + u_xlat4.xy;
    vs_TEXCOORD5.xy = (-u_xlat0.xy) + u_xlat4.xy;
    vs_TEXCOORD6.xy = u_xlat0.xy + u_xlat1.xy;
    vs_TEXCOORD7.xy = (-u_xlat0.xy) + u_xlat1.xy;
    return;
}

#endif
#ifdef FRAGMENT
#version 300 es

precision highp float;
precision highp int;
uniform 	vec4 _MainTex_TexelSize;
uniform 	mediump vec4 _MainTex_ST;
uniform 	float _BlurRadius;
uniform mediump sampler2D _MainTex;
in highp vec2 vs_TEXCOORD0;
in highp vec2 vs_TEXCOORD1;
in highp vec2 vs_TEXCOORD2;
in highp vec2 vs_TEXCOORD3;
in highp vec2 vs_TEXCOORD4;
in highp vec2 vs_TEXCOORD5;
in highp vec2 vs_TEXCOORD6;
in highp vec2 vs_TEXCOORD7;
layout(location = 0) out mediump vec4 SV_Target0;
vec4 u_xlat0;
mediump vec4 u_xlat16_0;
vec4 u_xlat1;
mediump vec4 u_xlat16_1;
mediump vec4 u_xlat16_2;
mediump vec3 u_xlat16_3;
vec2 u_xlat4;
mediump vec3 u_xlat16_4;
vec3 u_xlat5;
mediump vec3 u_xlat16_5;
vec2 u_xlat10;
vec2 u_xlat14;
float u_xlat15;
void main()
{
    u_xlat0.xy = vs_TEXCOORD2.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_0.xyz = texture(_MainTex, u_xlat0.xy).xyz;
    u_xlat16_1.y = dot(u_xlat16_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat0.xy = vs_TEXCOORD4.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_0.xyz = texture(_MainTex, u_xlat0.xy).xyz;
    u_xlat16_2.z = dot(u_xlat16_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat16_1.x = u_xlat16_2.z;
    u_xlat0.xy = vs_TEXCOORD5.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_0.xyz = texture(_MainTex, u_xlat0.xy).xyz;
    u_xlat16_1.z = dot(u_xlat16_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat0.x = dot(vec3(1.0, 1.0, 1.0), u_xlat16_1.xyz);
    u_xlat5.xy = vs_TEXCOORD3.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_5.xyz = texture(_MainTex, u_xlat5.xy).xyz;
    u_xlat16_3.y = dot(u_xlat16_5.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat5.xy = vs_TEXCOORD6.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_5.xyz = texture(_MainTex, u_xlat5.xy).xyz;
    u_xlat16_1.y = dot(u_xlat16_5.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat16_3.x = u_xlat16_1.y;
    u_xlat5.xy = vs_TEXCOORD7.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_5.xyz = texture(_MainTex, u_xlat5.xy).xyz;
    u_xlat16_2.x = dot(u_xlat16_5.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat16_3.z = u_xlat16_2.x;
    u_xlat5.x = dot(vec3(1.0, 1.0, 1.0), u_xlat16_3.xyz);
    u_xlat0.y = (-u_xlat0.x) + u_xlat5.x;
    u_xlat10.xy = vs_TEXCOORD1.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_4.xyz = texture(_MainTex, u_xlat10.xy).xyz;
    u_xlat16_2.y = dot(u_xlat16_4.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat10.x = dot(vec3(1.0, 1.0, 1.0), u_xlat16_2.xyz);
    u_xlat4.xy = vs_TEXCOORD0.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_4.xyz = texture(_MainTex, u_xlat4.xy).xyz;
    u_xlat16_1.x = dot(u_xlat16_4.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat15 = dot(vec3(1.0, 1.0, 1.0), u_xlat16_1.xyz);
    u_xlat0.x = (-u_xlat15) + u_xlat10.x;
    u_xlat10.xy = _MainTex_TexelSize.xy * vec2(vec2(_BlurRadius, _BlurRadius));
    u_xlat0.xy = u_xlat10.xy * u_xlat0.xy;
    u_xlat4.xy = vs_TEXCOORD0.xy + vs_TEXCOORD1.xy;
    u_xlat14.xy = u_xlat4.xy * vec2(0.5, 0.5) + u_xlat0.xy;
    u_xlat14.xy = u_xlat14.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_1 = texture(_MainTex, u_xlat14.xy);
    u_xlat14.xy = u_xlat4.xy * vec2(0.5, 0.5);
    u_xlat14.xy = u_xlat14.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_2 = texture(_MainTex, u_xlat14.xy);
    u_xlat1 = u_xlat16_1 + u_xlat16_2;
    u_xlat14.xy = u_xlat4.xy * vec2(0.5, 0.5) + (-u_xlat0.xy);
    u_xlat14.xy = u_xlat14.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_2 = texture(_MainTex, u_xlat14.xy);
    u_xlat1 = u_xlat1 + u_xlat16_2;
    u_xlat0.z = (-u_xlat0.y);
    u_xlat5.xz = u_xlat4.xy * vec2(0.5, 0.5) + u_xlat0.xz;
    u_xlat0.xz = u_xlat4.xy * vec2(0.5, 0.5) + (-u_xlat0.xz);
    u_xlat0.xz = u_xlat0.xz * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_2 = texture(_MainTex, u_xlat0.xz);
    u_xlat0.xy = u_xlat5.xz * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_0 = texture(_MainTex, u_xlat0.xy);
    u_xlat0 = u_xlat16_0 + u_xlat1;
    u_xlat0 = u_xlat16_2 + u_xlat0;
    u_xlat0 = u_xlat0 * vec4(0.200000003, 0.200000003, 0.200000003, 0.200000003);
    SV_Target0 = u_xlat0;
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
uniform 	float _OffsetScale;
in highp vec4 in_POSITION0;
in mediump vec2 in_TEXCOORD0;
out highp vec2 vs_TEXCOORD0;
out highp vec2 vs_TEXCOORD1;
out highp vec2 vs_TEXCOORD2;
out highp vec2 vs_TEXCOORD3;
out highp vec2 vs_TEXCOORD4;
out highp vec2 vs_TEXCOORD5;
out highp vec2 vs_TEXCOORD6;
out highp vec2 vs_TEXCOORD7;
vec4 u_xlat0;
vec4 u_xlat1;
vec2 u_xlat4;
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
    u_xlat0.yz = _MainTex_TexelSize.yx * vec2(_OffsetScale);
    u_xlat0.x = float(0.0);
    u_xlat0.w = float(0.0);
    vs_TEXCOORD0.xy = u_xlat0.xy + in_TEXCOORD0.xy;
    vs_TEXCOORD1.xy = (-u_xlat0.xy) + in_TEXCOORD0.xy;
    u_xlat1.xy = u_xlat0.zw + in_TEXCOORD0.xy;
    vs_TEXCOORD2.xy = u_xlat1.xy;
    u_xlat4.xy = (-u_xlat0.zw) + in_TEXCOORD0.xy;
    vs_TEXCOORD3.xy = u_xlat4.xy;
    vs_TEXCOORD4.xy = u_xlat0.xy + u_xlat4.xy;
    vs_TEXCOORD5.xy = (-u_xlat0.xy) + u_xlat4.xy;
    vs_TEXCOORD6.xy = u_xlat0.xy + u_xlat1.xy;
    vs_TEXCOORD7.xy = (-u_xlat0.xy) + u_xlat1.xy;
    return;
}

#endif
#ifdef FRAGMENT
#version 300 es

precision highp float;
precision highp int;
uniform 	vec4 _MainTex_TexelSize;
uniform 	mediump vec4 _MainTex_ST;
uniform 	float _BlurRadius;
uniform mediump sampler2D _MainTex;
in highp vec2 vs_TEXCOORD0;
in highp vec2 vs_TEXCOORD1;
in highp vec2 vs_TEXCOORD2;
in highp vec2 vs_TEXCOORD3;
in highp vec2 vs_TEXCOORD4;
in highp vec2 vs_TEXCOORD5;
in highp vec2 vs_TEXCOORD6;
in highp vec2 vs_TEXCOORD7;
layout(location = 0) out mediump vec4 SV_Target0;
vec4 u_xlat0;
mediump vec4 u_xlat16_0;
vec4 u_xlat1;
mediump vec4 u_xlat16_1;
mediump vec4 u_xlat16_2;
mediump vec3 u_xlat16_3;
vec2 u_xlat4;
mediump vec3 u_xlat16_4;
vec3 u_xlat5;
mediump vec3 u_xlat16_5;
vec2 u_xlat10;
vec2 u_xlat14;
float u_xlat15;
void main()
{
    u_xlat0.xy = vs_TEXCOORD2.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_0.xyz = texture(_MainTex, u_xlat0.xy).xyz;
    u_xlat16_1.y = dot(u_xlat16_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat0.xy = vs_TEXCOORD4.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_0.xyz = texture(_MainTex, u_xlat0.xy).xyz;
    u_xlat16_2.z = dot(u_xlat16_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat16_1.x = u_xlat16_2.z;
    u_xlat0.xy = vs_TEXCOORD5.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_0.xyz = texture(_MainTex, u_xlat0.xy).xyz;
    u_xlat16_1.z = dot(u_xlat16_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat0.x = dot(vec3(1.0, 1.0, 1.0), u_xlat16_1.xyz);
    u_xlat5.xy = vs_TEXCOORD3.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_5.xyz = texture(_MainTex, u_xlat5.xy).xyz;
    u_xlat16_3.y = dot(u_xlat16_5.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat5.xy = vs_TEXCOORD6.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_5.xyz = texture(_MainTex, u_xlat5.xy).xyz;
    u_xlat16_1.y = dot(u_xlat16_5.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat16_3.x = u_xlat16_1.y;
    u_xlat5.xy = vs_TEXCOORD7.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_5.xyz = texture(_MainTex, u_xlat5.xy).xyz;
    u_xlat16_2.x = dot(u_xlat16_5.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat16_3.z = u_xlat16_2.x;
    u_xlat5.x = dot(vec3(1.0, 1.0, 1.0), u_xlat16_3.xyz);
    u_xlat0.y = (-u_xlat0.x) + u_xlat5.x;
    u_xlat10.xy = vs_TEXCOORD1.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_4.xyz = texture(_MainTex, u_xlat10.xy).xyz;
    u_xlat16_2.y = dot(u_xlat16_4.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat10.x = dot(vec3(1.0, 1.0, 1.0), u_xlat16_2.xyz);
    u_xlat4.xy = vs_TEXCOORD0.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_4.xyz = texture(_MainTex, u_xlat4.xy).xyz;
    u_xlat16_1.x = dot(u_xlat16_4.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat15 = dot(vec3(1.0, 1.0, 1.0), u_xlat16_1.xyz);
    u_xlat0.x = (-u_xlat15) + u_xlat10.x;
    u_xlat10.xy = _MainTex_TexelSize.xy * vec2(vec2(_BlurRadius, _BlurRadius));
    u_xlat0.xy = u_xlat10.xy * u_xlat0.xy;
    u_xlat4.xy = vs_TEXCOORD0.xy + vs_TEXCOORD1.xy;
    u_xlat14.xy = u_xlat4.xy * vec2(0.5, 0.5) + u_xlat0.xy;
    u_xlat14.xy = u_xlat14.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_1 = texture(_MainTex, u_xlat14.xy);
    u_xlat14.xy = u_xlat4.xy * vec2(0.5, 0.5);
    u_xlat14.xy = u_xlat14.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_2 = texture(_MainTex, u_xlat14.xy);
    u_xlat1 = u_xlat16_1 + u_xlat16_2;
    u_xlat14.xy = u_xlat4.xy * vec2(0.5, 0.5) + (-u_xlat0.xy);
    u_xlat14.xy = u_xlat14.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_2 = texture(_MainTex, u_xlat14.xy);
    u_xlat1 = u_xlat1 + u_xlat16_2;
    u_xlat0.z = (-u_xlat0.y);
    u_xlat5.xz = u_xlat4.xy * vec2(0.5, 0.5) + u_xlat0.xz;
    u_xlat0.xz = u_xlat4.xy * vec2(0.5, 0.5) + (-u_xlat0.xz);
    u_xlat0.xz = u_xlat0.xz * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_2 = texture(_MainTex, u_xlat0.xz);
    u_xlat0.xy = u_xlat5.xz * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_0 = texture(_MainTex, u_xlat0.xy);
    u_xlat0 = u_xlat16_0 + u_xlat1;
    u_xlat0 = u_xlat16_2 + u_xlat0;
    u_xlat0 = u_xlat0 * vec4(0.200000003, 0.200000003, 0.200000003, 0.200000003);
    SV_Target0 = u_xlat0;
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
uniform 	float _OffsetScale;
in highp vec4 in_POSITION0;
in mediump vec2 in_TEXCOORD0;
out highp vec2 vs_TEXCOORD0;
out highp vec2 vs_TEXCOORD1;
out highp vec2 vs_TEXCOORD2;
out highp vec2 vs_TEXCOORD3;
out highp vec2 vs_TEXCOORD4;
out highp vec2 vs_TEXCOORD5;
out highp vec2 vs_TEXCOORD6;
out highp vec2 vs_TEXCOORD7;
vec4 u_xlat0;
vec4 u_xlat1;
vec2 u_xlat4;
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
    u_xlat0.yz = _MainTex_TexelSize.yx * vec2(_OffsetScale);
    u_xlat0.x = float(0.0);
    u_xlat0.w = float(0.0);
    vs_TEXCOORD0.xy = u_xlat0.xy + in_TEXCOORD0.xy;
    vs_TEXCOORD1.xy = (-u_xlat0.xy) + in_TEXCOORD0.xy;
    u_xlat1.xy = u_xlat0.zw + in_TEXCOORD0.xy;
    vs_TEXCOORD2.xy = u_xlat1.xy;
    u_xlat4.xy = (-u_xlat0.zw) + in_TEXCOORD0.xy;
    vs_TEXCOORD3.xy = u_xlat4.xy;
    vs_TEXCOORD4.xy = u_xlat0.xy + u_xlat4.xy;
    vs_TEXCOORD5.xy = (-u_xlat0.xy) + u_xlat4.xy;
    vs_TEXCOORD6.xy = u_xlat0.xy + u_xlat1.xy;
    vs_TEXCOORD7.xy = (-u_xlat0.xy) + u_xlat1.xy;
    return;
}

#endif
#ifdef FRAGMENT
#version 300 es

precision highp float;
precision highp int;
uniform 	vec4 _MainTex_TexelSize;
uniform 	mediump vec4 _MainTex_ST;
uniform 	float _BlurRadius;
uniform mediump sampler2D _MainTex;
in highp vec2 vs_TEXCOORD0;
in highp vec2 vs_TEXCOORD1;
in highp vec2 vs_TEXCOORD2;
in highp vec2 vs_TEXCOORD3;
in highp vec2 vs_TEXCOORD4;
in highp vec2 vs_TEXCOORD5;
in highp vec2 vs_TEXCOORD6;
in highp vec2 vs_TEXCOORD7;
layout(location = 0) out mediump vec4 SV_Target0;
vec4 u_xlat0;
mediump vec4 u_xlat16_0;
vec4 u_xlat1;
mediump vec4 u_xlat16_1;
mediump vec4 u_xlat16_2;
mediump vec3 u_xlat16_3;
vec2 u_xlat4;
mediump vec3 u_xlat16_4;
vec3 u_xlat5;
mediump vec3 u_xlat16_5;
vec2 u_xlat10;
vec2 u_xlat14;
float u_xlat15;
void main()
{
    u_xlat0.xy = vs_TEXCOORD2.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_0.xyz = texture(_MainTex, u_xlat0.xy).xyz;
    u_xlat16_1.y = dot(u_xlat16_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat0.xy = vs_TEXCOORD4.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_0.xyz = texture(_MainTex, u_xlat0.xy).xyz;
    u_xlat16_2.z = dot(u_xlat16_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat16_1.x = u_xlat16_2.z;
    u_xlat0.xy = vs_TEXCOORD5.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_0.xyz = texture(_MainTex, u_xlat0.xy).xyz;
    u_xlat16_1.z = dot(u_xlat16_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat0.x = dot(vec3(1.0, 1.0, 1.0), u_xlat16_1.xyz);
    u_xlat5.xy = vs_TEXCOORD3.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_5.xyz = texture(_MainTex, u_xlat5.xy).xyz;
    u_xlat16_3.y = dot(u_xlat16_5.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat5.xy = vs_TEXCOORD6.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_5.xyz = texture(_MainTex, u_xlat5.xy).xyz;
    u_xlat16_1.y = dot(u_xlat16_5.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat16_3.x = u_xlat16_1.y;
    u_xlat5.xy = vs_TEXCOORD7.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_5.xyz = texture(_MainTex, u_xlat5.xy).xyz;
    u_xlat16_2.x = dot(u_xlat16_5.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat16_3.z = u_xlat16_2.x;
    u_xlat5.x = dot(vec3(1.0, 1.0, 1.0), u_xlat16_3.xyz);
    u_xlat0.y = (-u_xlat0.x) + u_xlat5.x;
    u_xlat10.xy = vs_TEXCOORD1.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_4.xyz = texture(_MainTex, u_xlat10.xy).xyz;
    u_xlat16_2.y = dot(u_xlat16_4.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat10.x = dot(vec3(1.0, 1.0, 1.0), u_xlat16_2.xyz);
    u_xlat4.xy = vs_TEXCOORD0.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_4.xyz = texture(_MainTex, u_xlat4.xy).xyz;
    u_xlat16_1.x = dot(u_xlat16_4.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat15 = dot(vec3(1.0, 1.0, 1.0), u_xlat16_1.xyz);
    u_xlat0.x = (-u_xlat15) + u_xlat10.x;
    u_xlat10.xy = _MainTex_TexelSize.xy * vec2(vec2(_BlurRadius, _BlurRadius));
    u_xlat0.xy = u_xlat10.xy * u_xlat0.xy;
    u_xlat4.xy = vs_TEXCOORD0.xy + vs_TEXCOORD1.xy;
    u_xlat14.xy = u_xlat4.xy * vec2(0.5, 0.5) + u_xlat0.xy;
    u_xlat14.xy = u_xlat14.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_1 = texture(_MainTex, u_xlat14.xy);
    u_xlat14.xy = u_xlat4.xy * vec2(0.5, 0.5);
    u_xlat14.xy = u_xlat14.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_2 = texture(_MainTex, u_xlat14.xy);
    u_xlat1 = u_xlat16_1 + u_xlat16_2;
    u_xlat14.xy = u_xlat4.xy * vec2(0.5, 0.5) + (-u_xlat0.xy);
    u_xlat14.xy = u_xlat14.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_2 = texture(_MainTex, u_xlat14.xy);
    u_xlat1 = u_xlat1 + u_xlat16_2;
    u_xlat0.z = (-u_xlat0.y);
    u_xlat5.xz = u_xlat4.xy * vec2(0.5, 0.5) + u_xlat0.xz;
    u_xlat0.xz = u_xlat4.xy * vec2(0.5, 0.5) + (-u_xlat0.xz);
    u_xlat0.xz = u_xlat0.xz * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_2 = texture(_MainTex, u_xlat0.xz);
    u_xlat0.xy = u_xlat5.xz * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_0 = texture(_MainTex, u_xlat0.xy);
    u_xlat0 = u_xlat16_0 + u_xlat1;
    u_xlat0 = u_xlat16_2 + u_xlat0;
    u_xlat0 = u_xlat0 * vec4(0.200000003, 0.200000003, 0.200000003, 0.200000003);
    SV_Target0 = u_xlat0;
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
 Pass {
  ZTest Always
  ZWrite Off
  Cull Off
  GpuProgramID 72458
Program "vp" {
SubProgram "gles hw_tier00 " {
"#ifdef VERTEX
#version 100

uniform 	vec4 hlslcc_mtx4x4unity_ObjectToWorld[4];
uniform 	vec4 hlslcc_mtx4x4unity_MatrixVP[4];
uniform 	vec4 _MainTex_TexelSize;
uniform 	float _OffsetScale;
attribute highp vec4 in_POSITION0;
attribute mediump vec2 in_TEXCOORD0;
varying highp vec2 vs_TEXCOORD0;
varying highp vec2 vs_TEXCOORD1;
varying highp vec2 vs_TEXCOORD2;
varying highp vec2 vs_TEXCOORD3;
varying highp vec2 vs_TEXCOORD4;
varying highp vec2 vs_TEXCOORD5;
varying highp vec2 vs_TEXCOORD6;
varying highp vec2 vs_TEXCOORD7;
vec4 u_xlat0;
vec4 u_xlat1;
vec2 u_xlat4;
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
    u_xlat0.yz = _MainTex_TexelSize.yx * vec2(_OffsetScale);
    u_xlat0.x = float(0.0);
    u_xlat0.w = float(0.0);
    vs_TEXCOORD0.xy = u_xlat0.xy + in_TEXCOORD0.xy;
    vs_TEXCOORD1.xy = (-u_xlat0.xy) + in_TEXCOORD0.xy;
    u_xlat1.xy = u_xlat0.zw + in_TEXCOORD0.xy;
    vs_TEXCOORD2.xy = u_xlat1.xy;
    u_xlat4.xy = (-u_xlat0.zw) + in_TEXCOORD0.xy;
    vs_TEXCOORD3.xy = u_xlat4.xy;
    vs_TEXCOORD4.xy = u_xlat0.xy + u_xlat4.xy;
    vs_TEXCOORD5.xy = (-u_xlat0.xy) + u_xlat4.xy;
    vs_TEXCOORD6.xy = u_xlat0.xy + u_xlat1.xy;
    vs_TEXCOORD7.xy = (-u_xlat0.xy) + u_xlat1.xy;
    return;
}

#endif
#ifdef FRAGMENT
#version 100

#ifdef GL_FRAGMENT_PRECISION_HIGH
    precision highp float;
#else
    precision mediump float;
#endif
precision highp int;
uniform 	mediump vec4 _MainTex_ST;
uniform 	float _BlurRadius;
uniform lowp sampler2D _MainTex;
varying highp vec2 vs_TEXCOORD0;
varying highp vec2 vs_TEXCOORD1;
varying highp vec2 vs_TEXCOORD2;
varying highp vec2 vs_TEXCOORD3;
varying highp vec2 vs_TEXCOORD4;
varying highp vec2 vs_TEXCOORD5;
varying highp vec2 vs_TEXCOORD6;
varying highp vec2 vs_TEXCOORD7;
#define SV_Target0 gl_FragData[0]
vec2 u_xlat0;
lowp vec3 u_xlat10_0;
mediump vec3 u_xlat16_1;
mediump vec3 u_xlat16_2;
mediump vec3 u_xlat16_3;
vec2 u_xlat4;
lowp vec3 u_xlat10_4;
vec2 u_xlat5;
lowp vec3 u_xlat10_5;
vec2 u_xlat10;
float u_xlat15;
mediump float u_xlat16_16;
void main()
{
    u_xlat0.xy = vs_TEXCOORD2.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_0.xyz = texture2D(_MainTex, u_xlat0.xy).xyz;
    u_xlat16_1.y = dot(u_xlat10_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat0.xy = vs_TEXCOORD4.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_0.xyz = texture2D(_MainTex, u_xlat0.xy).xyz;
    u_xlat16_2.z = dot(u_xlat10_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat16_1.x = u_xlat16_2.z;
    u_xlat0.xy = vs_TEXCOORD5.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_0.xyz = texture2D(_MainTex, u_xlat0.xy).xyz;
    u_xlat16_1.z = dot(u_xlat10_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat0.x = dot(vec3(1.0, 1.0, 1.0), u_xlat16_1.xyz);
    u_xlat5.xy = vs_TEXCOORD3.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_5.xyz = texture2D(_MainTex, u_xlat5.xy).xyz;
    u_xlat16_3.y = dot(u_xlat10_5.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat5.xy = vs_TEXCOORD6.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_5.xyz = texture2D(_MainTex, u_xlat5.xy).xyz;
    u_xlat16_1.y = dot(u_xlat10_5.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat16_3.x = u_xlat16_1.y;
    u_xlat5.xy = vs_TEXCOORD7.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_5.xyz = texture2D(_MainTex, u_xlat5.xy).xyz;
    u_xlat16_2.x = dot(u_xlat10_5.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat16_3.z = u_xlat16_2.x;
    u_xlat5.x = dot(vec3(1.0, 1.0, 1.0), u_xlat16_3.xyz);
    u_xlat0.y = (-u_xlat0.x) + u_xlat5.x;
    u_xlat10.xy = vs_TEXCOORD1.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_4.xyz = texture2D(_MainTex, u_xlat10.xy).xyz;
    u_xlat16_2.y = dot(u_xlat10_4.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat10.x = dot(vec3(1.0, 1.0, 1.0), u_xlat16_2.xyz);
    u_xlat4.xy = vs_TEXCOORD0.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_4.xyz = texture2D(_MainTex, u_xlat4.xy).xyz;
    u_xlat16_1.x = dot(u_xlat10_4.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat15 = dot(vec3(1.0, 1.0, 1.0), u_xlat16_1.xyz);
    u_xlat0.x = (-u_xlat15) + u_xlat10.x;
    u_xlat0.xy = u_xlat0.xy * vec2(vec2(_BlurRadius, _BlurRadius));
    u_xlat16_1.xy = u_xlat0.xy * vec2(0.5, 0.5);
    u_xlat16_1.z = 0.5;
    u_xlat16_1.xyz = u_xlat16_1.xyz + vec3(0.5, 0.5, 0.5);
    u_xlat16_16 = dot(u_xlat16_1.xyz, u_xlat16_1.xyz);
    u_xlat16_16 = inversesqrt(u_xlat16_16);
    SV_Target0.xyz = vec3(u_xlat16_16) * u_xlat16_1.xyz;
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
uniform 	float _OffsetScale;
attribute highp vec4 in_POSITION0;
attribute mediump vec2 in_TEXCOORD0;
varying highp vec2 vs_TEXCOORD0;
varying highp vec2 vs_TEXCOORD1;
varying highp vec2 vs_TEXCOORD2;
varying highp vec2 vs_TEXCOORD3;
varying highp vec2 vs_TEXCOORD4;
varying highp vec2 vs_TEXCOORD5;
varying highp vec2 vs_TEXCOORD6;
varying highp vec2 vs_TEXCOORD7;
vec4 u_xlat0;
vec4 u_xlat1;
vec2 u_xlat4;
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
    u_xlat0.yz = _MainTex_TexelSize.yx * vec2(_OffsetScale);
    u_xlat0.x = float(0.0);
    u_xlat0.w = float(0.0);
    vs_TEXCOORD0.xy = u_xlat0.xy + in_TEXCOORD0.xy;
    vs_TEXCOORD1.xy = (-u_xlat0.xy) + in_TEXCOORD0.xy;
    u_xlat1.xy = u_xlat0.zw + in_TEXCOORD0.xy;
    vs_TEXCOORD2.xy = u_xlat1.xy;
    u_xlat4.xy = (-u_xlat0.zw) + in_TEXCOORD0.xy;
    vs_TEXCOORD3.xy = u_xlat4.xy;
    vs_TEXCOORD4.xy = u_xlat0.xy + u_xlat4.xy;
    vs_TEXCOORD5.xy = (-u_xlat0.xy) + u_xlat4.xy;
    vs_TEXCOORD6.xy = u_xlat0.xy + u_xlat1.xy;
    vs_TEXCOORD7.xy = (-u_xlat0.xy) + u_xlat1.xy;
    return;
}

#endif
#ifdef FRAGMENT
#version 100

#ifdef GL_FRAGMENT_PRECISION_HIGH
    precision highp float;
#else
    precision mediump float;
#endif
precision highp int;
uniform 	mediump vec4 _MainTex_ST;
uniform 	float _BlurRadius;
uniform lowp sampler2D _MainTex;
varying highp vec2 vs_TEXCOORD0;
varying highp vec2 vs_TEXCOORD1;
varying highp vec2 vs_TEXCOORD2;
varying highp vec2 vs_TEXCOORD3;
varying highp vec2 vs_TEXCOORD4;
varying highp vec2 vs_TEXCOORD5;
varying highp vec2 vs_TEXCOORD6;
varying highp vec2 vs_TEXCOORD7;
#define SV_Target0 gl_FragData[0]
vec2 u_xlat0;
lowp vec3 u_xlat10_0;
mediump vec3 u_xlat16_1;
mediump vec3 u_xlat16_2;
mediump vec3 u_xlat16_3;
vec2 u_xlat4;
lowp vec3 u_xlat10_4;
vec2 u_xlat5;
lowp vec3 u_xlat10_5;
vec2 u_xlat10;
float u_xlat15;
mediump float u_xlat16_16;
void main()
{
    u_xlat0.xy = vs_TEXCOORD2.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_0.xyz = texture2D(_MainTex, u_xlat0.xy).xyz;
    u_xlat16_1.y = dot(u_xlat10_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat0.xy = vs_TEXCOORD4.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_0.xyz = texture2D(_MainTex, u_xlat0.xy).xyz;
    u_xlat16_2.z = dot(u_xlat10_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat16_1.x = u_xlat16_2.z;
    u_xlat0.xy = vs_TEXCOORD5.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_0.xyz = texture2D(_MainTex, u_xlat0.xy).xyz;
    u_xlat16_1.z = dot(u_xlat10_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat0.x = dot(vec3(1.0, 1.0, 1.0), u_xlat16_1.xyz);
    u_xlat5.xy = vs_TEXCOORD3.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_5.xyz = texture2D(_MainTex, u_xlat5.xy).xyz;
    u_xlat16_3.y = dot(u_xlat10_5.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat5.xy = vs_TEXCOORD6.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_5.xyz = texture2D(_MainTex, u_xlat5.xy).xyz;
    u_xlat16_1.y = dot(u_xlat10_5.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat16_3.x = u_xlat16_1.y;
    u_xlat5.xy = vs_TEXCOORD7.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_5.xyz = texture2D(_MainTex, u_xlat5.xy).xyz;
    u_xlat16_2.x = dot(u_xlat10_5.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat16_3.z = u_xlat16_2.x;
    u_xlat5.x = dot(vec3(1.0, 1.0, 1.0), u_xlat16_3.xyz);
    u_xlat0.y = (-u_xlat0.x) + u_xlat5.x;
    u_xlat10.xy = vs_TEXCOORD1.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_4.xyz = texture2D(_MainTex, u_xlat10.xy).xyz;
    u_xlat16_2.y = dot(u_xlat10_4.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat10.x = dot(vec3(1.0, 1.0, 1.0), u_xlat16_2.xyz);
    u_xlat4.xy = vs_TEXCOORD0.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_4.xyz = texture2D(_MainTex, u_xlat4.xy).xyz;
    u_xlat16_1.x = dot(u_xlat10_4.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat15 = dot(vec3(1.0, 1.0, 1.0), u_xlat16_1.xyz);
    u_xlat0.x = (-u_xlat15) + u_xlat10.x;
    u_xlat0.xy = u_xlat0.xy * vec2(vec2(_BlurRadius, _BlurRadius));
    u_xlat16_1.xy = u_xlat0.xy * vec2(0.5, 0.5);
    u_xlat16_1.z = 0.5;
    u_xlat16_1.xyz = u_xlat16_1.xyz + vec3(0.5, 0.5, 0.5);
    u_xlat16_16 = dot(u_xlat16_1.xyz, u_xlat16_1.xyz);
    u_xlat16_16 = inversesqrt(u_xlat16_16);
    SV_Target0.xyz = vec3(u_xlat16_16) * u_xlat16_1.xyz;
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
uniform 	float _OffsetScale;
attribute highp vec4 in_POSITION0;
attribute mediump vec2 in_TEXCOORD0;
varying highp vec2 vs_TEXCOORD0;
varying highp vec2 vs_TEXCOORD1;
varying highp vec2 vs_TEXCOORD2;
varying highp vec2 vs_TEXCOORD3;
varying highp vec2 vs_TEXCOORD4;
varying highp vec2 vs_TEXCOORD5;
varying highp vec2 vs_TEXCOORD6;
varying highp vec2 vs_TEXCOORD7;
vec4 u_xlat0;
vec4 u_xlat1;
vec2 u_xlat4;
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
    u_xlat0.yz = _MainTex_TexelSize.yx * vec2(_OffsetScale);
    u_xlat0.x = float(0.0);
    u_xlat0.w = float(0.0);
    vs_TEXCOORD0.xy = u_xlat0.xy + in_TEXCOORD0.xy;
    vs_TEXCOORD1.xy = (-u_xlat0.xy) + in_TEXCOORD0.xy;
    u_xlat1.xy = u_xlat0.zw + in_TEXCOORD0.xy;
    vs_TEXCOORD2.xy = u_xlat1.xy;
    u_xlat4.xy = (-u_xlat0.zw) + in_TEXCOORD0.xy;
    vs_TEXCOORD3.xy = u_xlat4.xy;
    vs_TEXCOORD4.xy = u_xlat0.xy + u_xlat4.xy;
    vs_TEXCOORD5.xy = (-u_xlat0.xy) + u_xlat4.xy;
    vs_TEXCOORD6.xy = u_xlat0.xy + u_xlat1.xy;
    vs_TEXCOORD7.xy = (-u_xlat0.xy) + u_xlat1.xy;
    return;
}

#endif
#ifdef FRAGMENT
#version 100

#ifdef GL_FRAGMENT_PRECISION_HIGH
    precision highp float;
#else
    precision mediump float;
#endif
precision highp int;
uniform 	mediump vec4 _MainTex_ST;
uniform 	float _BlurRadius;
uniform lowp sampler2D _MainTex;
varying highp vec2 vs_TEXCOORD0;
varying highp vec2 vs_TEXCOORD1;
varying highp vec2 vs_TEXCOORD2;
varying highp vec2 vs_TEXCOORD3;
varying highp vec2 vs_TEXCOORD4;
varying highp vec2 vs_TEXCOORD5;
varying highp vec2 vs_TEXCOORD6;
varying highp vec2 vs_TEXCOORD7;
#define SV_Target0 gl_FragData[0]
vec2 u_xlat0;
lowp vec3 u_xlat10_0;
mediump vec3 u_xlat16_1;
mediump vec3 u_xlat16_2;
mediump vec3 u_xlat16_3;
vec2 u_xlat4;
lowp vec3 u_xlat10_4;
vec2 u_xlat5;
lowp vec3 u_xlat10_5;
vec2 u_xlat10;
float u_xlat15;
mediump float u_xlat16_16;
void main()
{
    u_xlat0.xy = vs_TEXCOORD2.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_0.xyz = texture2D(_MainTex, u_xlat0.xy).xyz;
    u_xlat16_1.y = dot(u_xlat10_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat0.xy = vs_TEXCOORD4.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_0.xyz = texture2D(_MainTex, u_xlat0.xy).xyz;
    u_xlat16_2.z = dot(u_xlat10_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat16_1.x = u_xlat16_2.z;
    u_xlat0.xy = vs_TEXCOORD5.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_0.xyz = texture2D(_MainTex, u_xlat0.xy).xyz;
    u_xlat16_1.z = dot(u_xlat10_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat0.x = dot(vec3(1.0, 1.0, 1.0), u_xlat16_1.xyz);
    u_xlat5.xy = vs_TEXCOORD3.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_5.xyz = texture2D(_MainTex, u_xlat5.xy).xyz;
    u_xlat16_3.y = dot(u_xlat10_5.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat5.xy = vs_TEXCOORD6.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_5.xyz = texture2D(_MainTex, u_xlat5.xy).xyz;
    u_xlat16_1.y = dot(u_xlat10_5.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat16_3.x = u_xlat16_1.y;
    u_xlat5.xy = vs_TEXCOORD7.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_5.xyz = texture2D(_MainTex, u_xlat5.xy).xyz;
    u_xlat16_2.x = dot(u_xlat10_5.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat16_3.z = u_xlat16_2.x;
    u_xlat5.x = dot(vec3(1.0, 1.0, 1.0), u_xlat16_3.xyz);
    u_xlat0.y = (-u_xlat0.x) + u_xlat5.x;
    u_xlat10.xy = vs_TEXCOORD1.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_4.xyz = texture2D(_MainTex, u_xlat10.xy).xyz;
    u_xlat16_2.y = dot(u_xlat10_4.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat10.x = dot(vec3(1.0, 1.0, 1.0), u_xlat16_2.xyz);
    u_xlat4.xy = vs_TEXCOORD0.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_4.xyz = texture2D(_MainTex, u_xlat4.xy).xyz;
    u_xlat16_1.x = dot(u_xlat10_4.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat15 = dot(vec3(1.0, 1.0, 1.0), u_xlat16_1.xyz);
    u_xlat0.x = (-u_xlat15) + u_xlat10.x;
    u_xlat0.xy = u_xlat0.xy * vec2(vec2(_BlurRadius, _BlurRadius));
    u_xlat16_1.xy = u_xlat0.xy * vec2(0.5, 0.5);
    u_xlat16_1.z = 0.5;
    u_xlat16_1.xyz = u_xlat16_1.xyz + vec3(0.5, 0.5, 0.5);
    u_xlat16_16 = dot(u_xlat16_1.xyz, u_xlat16_1.xyz);
    u_xlat16_16 = inversesqrt(u_xlat16_16);
    SV_Target0.xyz = vec3(u_xlat16_16) * u_xlat16_1.xyz;
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
uniform 	float _OffsetScale;
in highp vec4 in_POSITION0;
in mediump vec2 in_TEXCOORD0;
out highp vec2 vs_TEXCOORD0;
out highp vec2 vs_TEXCOORD1;
out highp vec2 vs_TEXCOORD2;
out highp vec2 vs_TEXCOORD3;
out highp vec2 vs_TEXCOORD4;
out highp vec2 vs_TEXCOORD5;
out highp vec2 vs_TEXCOORD6;
out highp vec2 vs_TEXCOORD7;
vec4 u_xlat0;
vec4 u_xlat1;
vec2 u_xlat4;
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
    u_xlat0.yz = _MainTex_TexelSize.yx * vec2(_OffsetScale);
    u_xlat0.x = float(0.0);
    u_xlat0.w = float(0.0);
    vs_TEXCOORD0.xy = u_xlat0.xy + in_TEXCOORD0.xy;
    vs_TEXCOORD1.xy = (-u_xlat0.xy) + in_TEXCOORD0.xy;
    u_xlat1.xy = u_xlat0.zw + in_TEXCOORD0.xy;
    vs_TEXCOORD2.xy = u_xlat1.xy;
    u_xlat4.xy = (-u_xlat0.zw) + in_TEXCOORD0.xy;
    vs_TEXCOORD3.xy = u_xlat4.xy;
    vs_TEXCOORD4.xy = u_xlat0.xy + u_xlat4.xy;
    vs_TEXCOORD5.xy = (-u_xlat0.xy) + u_xlat4.xy;
    vs_TEXCOORD6.xy = u_xlat0.xy + u_xlat1.xy;
    vs_TEXCOORD7.xy = (-u_xlat0.xy) + u_xlat1.xy;
    return;
}

#endif
#ifdef FRAGMENT
#version 300 es

precision highp float;
precision highp int;
uniform 	mediump vec4 _MainTex_ST;
uniform 	float _BlurRadius;
uniform mediump sampler2D _MainTex;
in highp vec2 vs_TEXCOORD0;
in highp vec2 vs_TEXCOORD1;
in highp vec2 vs_TEXCOORD2;
in highp vec2 vs_TEXCOORD3;
in highp vec2 vs_TEXCOORD4;
in highp vec2 vs_TEXCOORD5;
in highp vec2 vs_TEXCOORD6;
in highp vec2 vs_TEXCOORD7;
layout(location = 0) out mediump vec4 SV_Target0;
vec2 u_xlat0;
mediump vec3 u_xlat16_0;
mediump vec3 u_xlat16_1;
mediump vec3 u_xlat16_2;
mediump vec3 u_xlat16_3;
vec2 u_xlat4;
mediump vec3 u_xlat16_4;
vec2 u_xlat5;
mediump vec3 u_xlat16_5;
vec2 u_xlat10;
float u_xlat15;
mediump float u_xlat16_16;
void main()
{
    u_xlat0.xy = vs_TEXCOORD2.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_0.xyz = texture(_MainTex, u_xlat0.xy).xyz;
    u_xlat16_1.y = dot(u_xlat16_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat0.xy = vs_TEXCOORD4.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_0.xyz = texture(_MainTex, u_xlat0.xy).xyz;
    u_xlat16_2.z = dot(u_xlat16_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat16_1.x = u_xlat16_2.z;
    u_xlat0.xy = vs_TEXCOORD5.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_0.xyz = texture(_MainTex, u_xlat0.xy).xyz;
    u_xlat16_1.z = dot(u_xlat16_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat0.x = dot(vec3(1.0, 1.0, 1.0), u_xlat16_1.xyz);
    u_xlat5.xy = vs_TEXCOORD3.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_5.xyz = texture(_MainTex, u_xlat5.xy).xyz;
    u_xlat16_3.y = dot(u_xlat16_5.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat5.xy = vs_TEXCOORD6.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_5.xyz = texture(_MainTex, u_xlat5.xy).xyz;
    u_xlat16_1.y = dot(u_xlat16_5.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat16_3.x = u_xlat16_1.y;
    u_xlat5.xy = vs_TEXCOORD7.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_5.xyz = texture(_MainTex, u_xlat5.xy).xyz;
    u_xlat16_2.x = dot(u_xlat16_5.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat16_3.z = u_xlat16_2.x;
    u_xlat5.x = dot(vec3(1.0, 1.0, 1.0), u_xlat16_3.xyz);
    u_xlat0.y = (-u_xlat0.x) + u_xlat5.x;
    u_xlat10.xy = vs_TEXCOORD1.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_4.xyz = texture(_MainTex, u_xlat10.xy).xyz;
    u_xlat16_2.y = dot(u_xlat16_4.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat10.x = dot(vec3(1.0, 1.0, 1.0), u_xlat16_2.xyz);
    u_xlat4.xy = vs_TEXCOORD0.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_4.xyz = texture(_MainTex, u_xlat4.xy).xyz;
    u_xlat16_1.x = dot(u_xlat16_4.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat15 = dot(vec3(1.0, 1.0, 1.0), u_xlat16_1.xyz);
    u_xlat0.x = (-u_xlat15) + u_xlat10.x;
    u_xlat0.xy = u_xlat0.xy * vec2(vec2(_BlurRadius, _BlurRadius));
    u_xlat16_1.xy = u_xlat0.xy * vec2(0.5, 0.5);
    u_xlat16_1.z = 0.5;
    u_xlat16_1.xyz = u_xlat16_1.xyz + vec3(0.5, 0.5, 0.5);
    u_xlat16_16 = dot(u_xlat16_1.xyz, u_xlat16_1.xyz);
    u_xlat16_16 = inversesqrt(u_xlat16_16);
    SV_Target0.xyz = vec3(u_xlat16_16) * u_xlat16_1.xyz;
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
uniform 	float _OffsetScale;
in highp vec4 in_POSITION0;
in mediump vec2 in_TEXCOORD0;
out highp vec2 vs_TEXCOORD0;
out highp vec2 vs_TEXCOORD1;
out highp vec2 vs_TEXCOORD2;
out highp vec2 vs_TEXCOORD3;
out highp vec2 vs_TEXCOORD4;
out highp vec2 vs_TEXCOORD5;
out highp vec2 vs_TEXCOORD6;
out highp vec2 vs_TEXCOORD7;
vec4 u_xlat0;
vec4 u_xlat1;
vec2 u_xlat4;
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
    u_xlat0.yz = _MainTex_TexelSize.yx * vec2(_OffsetScale);
    u_xlat0.x = float(0.0);
    u_xlat0.w = float(0.0);
    vs_TEXCOORD0.xy = u_xlat0.xy + in_TEXCOORD0.xy;
    vs_TEXCOORD1.xy = (-u_xlat0.xy) + in_TEXCOORD0.xy;
    u_xlat1.xy = u_xlat0.zw + in_TEXCOORD0.xy;
    vs_TEXCOORD2.xy = u_xlat1.xy;
    u_xlat4.xy = (-u_xlat0.zw) + in_TEXCOORD0.xy;
    vs_TEXCOORD3.xy = u_xlat4.xy;
    vs_TEXCOORD4.xy = u_xlat0.xy + u_xlat4.xy;
    vs_TEXCOORD5.xy = (-u_xlat0.xy) + u_xlat4.xy;
    vs_TEXCOORD6.xy = u_xlat0.xy + u_xlat1.xy;
    vs_TEXCOORD7.xy = (-u_xlat0.xy) + u_xlat1.xy;
    return;
}

#endif
#ifdef FRAGMENT
#version 300 es

precision highp float;
precision highp int;
uniform 	mediump vec4 _MainTex_ST;
uniform 	float _BlurRadius;
uniform mediump sampler2D _MainTex;
in highp vec2 vs_TEXCOORD0;
in highp vec2 vs_TEXCOORD1;
in highp vec2 vs_TEXCOORD2;
in highp vec2 vs_TEXCOORD3;
in highp vec2 vs_TEXCOORD4;
in highp vec2 vs_TEXCOORD5;
in highp vec2 vs_TEXCOORD6;
in highp vec2 vs_TEXCOORD7;
layout(location = 0) out mediump vec4 SV_Target0;
vec2 u_xlat0;
mediump vec3 u_xlat16_0;
mediump vec3 u_xlat16_1;
mediump vec3 u_xlat16_2;
mediump vec3 u_xlat16_3;
vec2 u_xlat4;
mediump vec3 u_xlat16_4;
vec2 u_xlat5;
mediump vec3 u_xlat16_5;
vec2 u_xlat10;
float u_xlat15;
mediump float u_xlat16_16;
void main()
{
    u_xlat0.xy = vs_TEXCOORD2.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_0.xyz = texture(_MainTex, u_xlat0.xy).xyz;
    u_xlat16_1.y = dot(u_xlat16_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat0.xy = vs_TEXCOORD4.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_0.xyz = texture(_MainTex, u_xlat0.xy).xyz;
    u_xlat16_2.z = dot(u_xlat16_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat16_1.x = u_xlat16_2.z;
    u_xlat0.xy = vs_TEXCOORD5.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_0.xyz = texture(_MainTex, u_xlat0.xy).xyz;
    u_xlat16_1.z = dot(u_xlat16_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat0.x = dot(vec3(1.0, 1.0, 1.0), u_xlat16_1.xyz);
    u_xlat5.xy = vs_TEXCOORD3.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_5.xyz = texture(_MainTex, u_xlat5.xy).xyz;
    u_xlat16_3.y = dot(u_xlat16_5.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat5.xy = vs_TEXCOORD6.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_5.xyz = texture(_MainTex, u_xlat5.xy).xyz;
    u_xlat16_1.y = dot(u_xlat16_5.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat16_3.x = u_xlat16_1.y;
    u_xlat5.xy = vs_TEXCOORD7.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_5.xyz = texture(_MainTex, u_xlat5.xy).xyz;
    u_xlat16_2.x = dot(u_xlat16_5.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat16_3.z = u_xlat16_2.x;
    u_xlat5.x = dot(vec3(1.0, 1.0, 1.0), u_xlat16_3.xyz);
    u_xlat0.y = (-u_xlat0.x) + u_xlat5.x;
    u_xlat10.xy = vs_TEXCOORD1.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_4.xyz = texture(_MainTex, u_xlat10.xy).xyz;
    u_xlat16_2.y = dot(u_xlat16_4.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat10.x = dot(vec3(1.0, 1.0, 1.0), u_xlat16_2.xyz);
    u_xlat4.xy = vs_TEXCOORD0.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_4.xyz = texture(_MainTex, u_xlat4.xy).xyz;
    u_xlat16_1.x = dot(u_xlat16_4.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat15 = dot(vec3(1.0, 1.0, 1.0), u_xlat16_1.xyz);
    u_xlat0.x = (-u_xlat15) + u_xlat10.x;
    u_xlat0.xy = u_xlat0.xy * vec2(vec2(_BlurRadius, _BlurRadius));
    u_xlat16_1.xy = u_xlat0.xy * vec2(0.5, 0.5);
    u_xlat16_1.z = 0.5;
    u_xlat16_1.xyz = u_xlat16_1.xyz + vec3(0.5, 0.5, 0.5);
    u_xlat16_16 = dot(u_xlat16_1.xyz, u_xlat16_1.xyz);
    u_xlat16_16 = inversesqrt(u_xlat16_16);
    SV_Target0.xyz = vec3(u_xlat16_16) * u_xlat16_1.xyz;
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
uniform 	float _OffsetScale;
in highp vec4 in_POSITION0;
in mediump vec2 in_TEXCOORD0;
out highp vec2 vs_TEXCOORD0;
out highp vec2 vs_TEXCOORD1;
out highp vec2 vs_TEXCOORD2;
out highp vec2 vs_TEXCOORD3;
out highp vec2 vs_TEXCOORD4;
out highp vec2 vs_TEXCOORD5;
out highp vec2 vs_TEXCOORD6;
out highp vec2 vs_TEXCOORD7;
vec4 u_xlat0;
vec4 u_xlat1;
vec2 u_xlat4;
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
    u_xlat0.yz = _MainTex_TexelSize.yx * vec2(_OffsetScale);
    u_xlat0.x = float(0.0);
    u_xlat0.w = float(0.0);
    vs_TEXCOORD0.xy = u_xlat0.xy + in_TEXCOORD0.xy;
    vs_TEXCOORD1.xy = (-u_xlat0.xy) + in_TEXCOORD0.xy;
    u_xlat1.xy = u_xlat0.zw + in_TEXCOORD0.xy;
    vs_TEXCOORD2.xy = u_xlat1.xy;
    u_xlat4.xy = (-u_xlat0.zw) + in_TEXCOORD0.xy;
    vs_TEXCOORD3.xy = u_xlat4.xy;
    vs_TEXCOORD4.xy = u_xlat0.xy + u_xlat4.xy;
    vs_TEXCOORD5.xy = (-u_xlat0.xy) + u_xlat4.xy;
    vs_TEXCOORD6.xy = u_xlat0.xy + u_xlat1.xy;
    vs_TEXCOORD7.xy = (-u_xlat0.xy) + u_xlat1.xy;
    return;
}

#endif
#ifdef FRAGMENT
#version 300 es

precision highp float;
precision highp int;
uniform 	mediump vec4 _MainTex_ST;
uniform 	float _BlurRadius;
uniform mediump sampler2D _MainTex;
in highp vec2 vs_TEXCOORD0;
in highp vec2 vs_TEXCOORD1;
in highp vec2 vs_TEXCOORD2;
in highp vec2 vs_TEXCOORD3;
in highp vec2 vs_TEXCOORD4;
in highp vec2 vs_TEXCOORD5;
in highp vec2 vs_TEXCOORD6;
in highp vec2 vs_TEXCOORD7;
layout(location = 0) out mediump vec4 SV_Target0;
vec2 u_xlat0;
mediump vec3 u_xlat16_0;
mediump vec3 u_xlat16_1;
mediump vec3 u_xlat16_2;
mediump vec3 u_xlat16_3;
vec2 u_xlat4;
mediump vec3 u_xlat16_4;
vec2 u_xlat5;
mediump vec3 u_xlat16_5;
vec2 u_xlat10;
float u_xlat15;
mediump float u_xlat16_16;
void main()
{
    u_xlat0.xy = vs_TEXCOORD2.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_0.xyz = texture(_MainTex, u_xlat0.xy).xyz;
    u_xlat16_1.y = dot(u_xlat16_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat0.xy = vs_TEXCOORD4.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_0.xyz = texture(_MainTex, u_xlat0.xy).xyz;
    u_xlat16_2.z = dot(u_xlat16_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat16_1.x = u_xlat16_2.z;
    u_xlat0.xy = vs_TEXCOORD5.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_0.xyz = texture(_MainTex, u_xlat0.xy).xyz;
    u_xlat16_1.z = dot(u_xlat16_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat0.x = dot(vec3(1.0, 1.0, 1.0), u_xlat16_1.xyz);
    u_xlat5.xy = vs_TEXCOORD3.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_5.xyz = texture(_MainTex, u_xlat5.xy).xyz;
    u_xlat16_3.y = dot(u_xlat16_5.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat5.xy = vs_TEXCOORD6.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_5.xyz = texture(_MainTex, u_xlat5.xy).xyz;
    u_xlat16_1.y = dot(u_xlat16_5.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat16_3.x = u_xlat16_1.y;
    u_xlat5.xy = vs_TEXCOORD7.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_5.xyz = texture(_MainTex, u_xlat5.xy).xyz;
    u_xlat16_2.x = dot(u_xlat16_5.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat16_3.z = u_xlat16_2.x;
    u_xlat5.x = dot(vec3(1.0, 1.0, 1.0), u_xlat16_3.xyz);
    u_xlat0.y = (-u_xlat0.x) + u_xlat5.x;
    u_xlat10.xy = vs_TEXCOORD1.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_4.xyz = texture(_MainTex, u_xlat10.xy).xyz;
    u_xlat16_2.y = dot(u_xlat16_4.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat10.x = dot(vec3(1.0, 1.0, 1.0), u_xlat16_2.xyz);
    u_xlat4.xy = vs_TEXCOORD0.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_4.xyz = texture(_MainTex, u_xlat4.xy).xyz;
    u_xlat16_1.x = dot(u_xlat16_4.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat15 = dot(vec3(1.0, 1.0, 1.0), u_xlat16_1.xyz);
    u_xlat0.x = (-u_xlat15) + u_xlat10.x;
    u_xlat0.xy = u_xlat0.xy * vec2(vec2(_BlurRadius, _BlurRadius));
    u_xlat16_1.xy = u_xlat0.xy * vec2(0.5, 0.5);
    u_xlat16_1.z = 0.5;
    u_xlat16_1.xyz = u_xlat16_1.xyz + vec3(0.5, 0.5, 0.5);
    u_xlat16_16 = dot(u_xlat16_1.xyz, u_xlat16_1.xyz);
    u_xlat16_16 = inversesqrt(u_xlat16_16);
    SV_Target0.xyz = vec3(u_xlat16_16) * u_xlat16_1.xyz;
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