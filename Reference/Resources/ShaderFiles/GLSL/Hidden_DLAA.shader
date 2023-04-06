//////////////////////////////////////////
//
// NOTE: This is *not* a valid shader file
//
///////////////////////////////////////////
Shader "Hidden/DLAA" {
Properties {
_MainTex ("Base (RGB)", 2D) = "white" { }
}
SubShader {
 Pass {
  ZTest Always
  ZWrite Off
  Cull Off
  GpuProgramID 4243
Program "vp" {
SubProgram "gles hw_tier00 " {
"#ifdef VERTEX
#version 100

uniform 	vec4 hlslcc_mtx4x4unity_ObjectToWorld[4];
uniform 	vec4 hlslcc_mtx4x4unity_MatrixVP[4];
attribute highp vec4 in_POSITION0;
attribute mediump vec2 in_TEXCOORD0;
varying highp vec2 vs_TEXCOORD0;
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
uniform lowp sampler2D _MainTex;
varying highp vec2 vs_TEXCOORD0;
#define SV_Target0 gl_FragData[0]
vec3 u_xlat0;
lowp vec3 u_xlat10_0;
vec4 u_xlat1;
lowp vec3 u_xlat10_1;
lowp vec3 u_xlat10_2;
void main()
{
    u_xlat0.xy = vs_TEXCOORD0.xy + (-_MainTex_TexelSize.xy);
    u_xlat0.xy = u_xlat0.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_0.xyz = texture2D(_MainTex, u_xlat0.xy).xyz;
    u_xlat1 = _MainTex_TexelSize.xyxy * vec4(1.0, -1.0, -1.0, 1.0) + vs_TEXCOORD0.xyxy;
    u_xlat1 = u_xlat1 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_2.xyz = texture2D(_MainTex, u_xlat1.xy).xyz;
    u_xlat10_1.xyz = texture2D(_MainTex, u_xlat1.zw).xyz;
    u_xlat0.xyz = u_xlat10_0.xyz + u_xlat10_2.xyz;
    u_xlat0.xyz = u_xlat10_1.xyz + u_xlat0.xyz;
    u_xlat1.xy = vs_TEXCOORD0.xy + _MainTex_TexelSize.xy;
    u_xlat1.xy = u_xlat1.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_1.xyz = texture2D(_MainTex, u_xlat1.xy).xyz;
    u_xlat0.xyz = u_xlat0.xyz + u_xlat10_1.xyz;
    u_xlat1.xy = vs_TEXCOORD0.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat1.xyz = texture2D(_MainTex, u_xlat1.xy).xyz;
    u_xlat0.xyz = (-u_xlat1.xyz) * vec3(4.0, 4.0, 4.0) + u_xlat0.xyz;
    u_xlat0.xyz = abs(u_xlat0.xyz) * vec3(4.0, 4.0, 4.0);
    u_xlat1.w = dot(u_xlat0.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    SV_Target0 = u_xlat1;
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
attribute highp vec4 in_POSITION0;
attribute mediump vec2 in_TEXCOORD0;
varying highp vec2 vs_TEXCOORD0;
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
uniform lowp sampler2D _MainTex;
varying highp vec2 vs_TEXCOORD0;
#define SV_Target0 gl_FragData[0]
vec3 u_xlat0;
lowp vec3 u_xlat10_0;
vec4 u_xlat1;
lowp vec3 u_xlat10_1;
lowp vec3 u_xlat10_2;
void main()
{
    u_xlat0.xy = vs_TEXCOORD0.xy + (-_MainTex_TexelSize.xy);
    u_xlat0.xy = u_xlat0.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_0.xyz = texture2D(_MainTex, u_xlat0.xy).xyz;
    u_xlat1 = _MainTex_TexelSize.xyxy * vec4(1.0, -1.0, -1.0, 1.0) + vs_TEXCOORD0.xyxy;
    u_xlat1 = u_xlat1 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_2.xyz = texture2D(_MainTex, u_xlat1.xy).xyz;
    u_xlat10_1.xyz = texture2D(_MainTex, u_xlat1.zw).xyz;
    u_xlat0.xyz = u_xlat10_0.xyz + u_xlat10_2.xyz;
    u_xlat0.xyz = u_xlat10_1.xyz + u_xlat0.xyz;
    u_xlat1.xy = vs_TEXCOORD0.xy + _MainTex_TexelSize.xy;
    u_xlat1.xy = u_xlat1.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_1.xyz = texture2D(_MainTex, u_xlat1.xy).xyz;
    u_xlat0.xyz = u_xlat0.xyz + u_xlat10_1.xyz;
    u_xlat1.xy = vs_TEXCOORD0.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat1.xyz = texture2D(_MainTex, u_xlat1.xy).xyz;
    u_xlat0.xyz = (-u_xlat1.xyz) * vec3(4.0, 4.0, 4.0) + u_xlat0.xyz;
    u_xlat0.xyz = abs(u_xlat0.xyz) * vec3(4.0, 4.0, 4.0);
    u_xlat1.w = dot(u_xlat0.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    SV_Target0 = u_xlat1;
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
attribute highp vec4 in_POSITION0;
attribute mediump vec2 in_TEXCOORD0;
varying highp vec2 vs_TEXCOORD0;
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
uniform lowp sampler2D _MainTex;
varying highp vec2 vs_TEXCOORD0;
#define SV_Target0 gl_FragData[0]
vec3 u_xlat0;
lowp vec3 u_xlat10_0;
vec4 u_xlat1;
lowp vec3 u_xlat10_1;
lowp vec3 u_xlat10_2;
void main()
{
    u_xlat0.xy = vs_TEXCOORD0.xy + (-_MainTex_TexelSize.xy);
    u_xlat0.xy = u_xlat0.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_0.xyz = texture2D(_MainTex, u_xlat0.xy).xyz;
    u_xlat1 = _MainTex_TexelSize.xyxy * vec4(1.0, -1.0, -1.0, 1.0) + vs_TEXCOORD0.xyxy;
    u_xlat1 = u_xlat1 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_2.xyz = texture2D(_MainTex, u_xlat1.xy).xyz;
    u_xlat10_1.xyz = texture2D(_MainTex, u_xlat1.zw).xyz;
    u_xlat0.xyz = u_xlat10_0.xyz + u_xlat10_2.xyz;
    u_xlat0.xyz = u_xlat10_1.xyz + u_xlat0.xyz;
    u_xlat1.xy = vs_TEXCOORD0.xy + _MainTex_TexelSize.xy;
    u_xlat1.xy = u_xlat1.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_1.xyz = texture2D(_MainTex, u_xlat1.xy).xyz;
    u_xlat0.xyz = u_xlat0.xyz + u_xlat10_1.xyz;
    u_xlat1.xy = vs_TEXCOORD0.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat1.xyz = texture2D(_MainTex, u_xlat1.xy).xyz;
    u_xlat0.xyz = (-u_xlat1.xyz) * vec3(4.0, 4.0, 4.0) + u_xlat0.xyz;
    u_xlat0.xyz = abs(u_xlat0.xyz) * vec3(4.0, 4.0, 4.0);
    u_xlat1.w = dot(u_xlat0.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    SV_Target0 = u_xlat1;
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
in highp vec4 in_POSITION0;
in mediump vec2 in_TEXCOORD0;
out highp vec2 vs_TEXCOORD0;
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
    return;
}

#endif
#ifdef FRAGMENT
#version 300 es

precision highp float;
precision highp int;
uniform 	vec4 _MainTex_TexelSize;
uniform 	mediump vec4 _MainTex_ST;
uniform mediump sampler2D _MainTex;
in highp vec2 vs_TEXCOORD0;
layout(location = 0) out mediump vec4 SV_Target0;
vec3 u_xlat0;
mediump vec3 u_xlat16_0;
vec4 u_xlat1;
mediump vec3 u_xlat16_1;
mediump vec3 u_xlat16_2;
void main()
{
    u_xlat0.xy = vs_TEXCOORD0.xy + (-_MainTex_TexelSize.xy);
    u_xlat0.xy = u_xlat0.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_0.xyz = texture(_MainTex, u_xlat0.xy).xyz;
    u_xlat1 = _MainTex_TexelSize.xyxy * vec4(1.0, -1.0, -1.0, 1.0) + vs_TEXCOORD0.xyxy;
    u_xlat1 = u_xlat1 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_2.xyz = texture(_MainTex, u_xlat1.xy).xyz;
    u_xlat16_1.xyz = texture(_MainTex, u_xlat1.zw).xyz;
    u_xlat0.xyz = u_xlat16_0.xyz + u_xlat16_2.xyz;
    u_xlat0.xyz = u_xlat16_1.xyz + u_xlat0.xyz;
    u_xlat1.xy = vs_TEXCOORD0.xy + _MainTex_TexelSize.xy;
    u_xlat1.xy = u_xlat1.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_1.xyz = texture(_MainTex, u_xlat1.xy).xyz;
    u_xlat0.xyz = u_xlat0.xyz + u_xlat16_1.xyz;
    u_xlat1.xy = vs_TEXCOORD0.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat1.xyz = texture(_MainTex, u_xlat1.xy).xyz;
    u_xlat0.xyz = (-u_xlat1.xyz) * vec3(4.0, 4.0, 4.0) + u_xlat0.xyz;
    u_xlat0.xyz = abs(u_xlat0.xyz) * vec3(4.0, 4.0, 4.0);
    u_xlat1.w = dot(u_xlat0.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    SV_Target0 = u_xlat1;
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
in highp vec4 in_POSITION0;
in mediump vec2 in_TEXCOORD0;
out highp vec2 vs_TEXCOORD0;
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
    return;
}

#endif
#ifdef FRAGMENT
#version 300 es

precision highp float;
precision highp int;
uniform 	vec4 _MainTex_TexelSize;
uniform 	mediump vec4 _MainTex_ST;
uniform mediump sampler2D _MainTex;
in highp vec2 vs_TEXCOORD0;
layout(location = 0) out mediump vec4 SV_Target0;
vec3 u_xlat0;
mediump vec3 u_xlat16_0;
vec4 u_xlat1;
mediump vec3 u_xlat16_1;
mediump vec3 u_xlat16_2;
void main()
{
    u_xlat0.xy = vs_TEXCOORD0.xy + (-_MainTex_TexelSize.xy);
    u_xlat0.xy = u_xlat0.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_0.xyz = texture(_MainTex, u_xlat0.xy).xyz;
    u_xlat1 = _MainTex_TexelSize.xyxy * vec4(1.0, -1.0, -1.0, 1.0) + vs_TEXCOORD0.xyxy;
    u_xlat1 = u_xlat1 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_2.xyz = texture(_MainTex, u_xlat1.xy).xyz;
    u_xlat16_1.xyz = texture(_MainTex, u_xlat1.zw).xyz;
    u_xlat0.xyz = u_xlat16_0.xyz + u_xlat16_2.xyz;
    u_xlat0.xyz = u_xlat16_1.xyz + u_xlat0.xyz;
    u_xlat1.xy = vs_TEXCOORD0.xy + _MainTex_TexelSize.xy;
    u_xlat1.xy = u_xlat1.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_1.xyz = texture(_MainTex, u_xlat1.xy).xyz;
    u_xlat0.xyz = u_xlat0.xyz + u_xlat16_1.xyz;
    u_xlat1.xy = vs_TEXCOORD0.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat1.xyz = texture(_MainTex, u_xlat1.xy).xyz;
    u_xlat0.xyz = (-u_xlat1.xyz) * vec3(4.0, 4.0, 4.0) + u_xlat0.xyz;
    u_xlat0.xyz = abs(u_xlat0.xyz) * vec3(4.0, 4.0, 4.0);
    u_xlat1.w = dot(u_xlat0.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    SV_Target0 = u_xlat1;
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
in highp vec4 in_POSITION0;
in mediump vec2 in_TEXCOORD0;
out highp vec2 vs_TEXCOORD0;
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
    return;
}

#endif
#ifdef FRAGMENT
#version 300 es

precision highp float;
precision highp int;
uniform 	vec4 _MainTex_TexelSize;
uniform 	mediump vec4 _MainTex_ST;
uniform mediump sampler2D _MainTex;
in highp vec2 vs_TEXCOORD0;
layout(location = 0) out mediump vec4 SV_Target0;
vec3 u_xlat0;
mediump vec3 u_xlat16_0;
vec4 u_xlat1;
mediump vec3 u_xlat16_1;
mediump vec3 u_xlat16_2;
void main()
{
    u_xlat0.xy = vs_TEXCOORD0.xy + (-_MainTex_TexelSize.xy);
    u_xlat0.xy = u_xlat0.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_0.xyz = texture(_MainTex, u_xlat0.xy).xyz;
    u_xlat1 = _MainTex_TexelSize.xyxy * vec4(1.0, -1.0, -1.0, 1.0) + vs_TEXCOORD0.xyxy;
    u_xlat1 = u_xlat1 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_2.xyz = texture(_MainTex, u_xlat1.xy).xyz;
    u_xlat16_1.xyz = texture(_MainTex, u_xlat1.zw).xyz;
    u_xlat0.xyz = u_xlat16_0.xyz + u_xlat16_2.xyz;
    u_xlat0.xyz = u_xlat16_1.xyz + u_xlat0.xyz;
    u_xlat1.xy = vs_TEXCOORD0.xy + _MainTex_TexelSize.xy;
    u_xlat1.xy = u_xlat1.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_1.xyz = texture(_MainTex, u_xlat1.xy).xyz;
    u_xlat0.xyz = u_xlat0.xyz + u_xlat16_1.xyz;
    u_xlat1.xy = vs_TEXCOORD0.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat1.xyz = texture(_MainTex, u_xlat1.xy).xyz;
    u_xlat0.xyz = (-u_xlat1.xyz) * vec3(4.0, 4.0, 4.0) + u_xlat0.xyz;
    u_xlat0.xyz = abs(u_xlat0.xyz) * vec3(4.0, 4.0, 4.0);
    u_xlat1.w = dot(u_xlat0.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    SV_Target0 = u_xlat1;
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
  GpuProgramID 68889
Program "vp" {
SubProgram "gles hw_tier00 " {
"#ifdef VERTEX
#version 100

uniform 	vec4 hlslcc_mtx4x4unity_ObjectToWorld[4];
uniform 	vec4 hlslcc_mtx4x4unity_MatrixVP[4];
attribute highp vec4 in_POSITION0;
attribute mediump vec2 in_TEXCOORD0;
varying highp vec2 vs_TEXCOORD0;
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
uniform lowp sampler2D _MainTex;
varying highp vec2 vs_TEXCOORD0;
#define SV_Target0 gl_FragData[0]
vec4 u_xlat0;
lowp vec4 u_xlat10_0;
bool u_xlatb0;
vec4 u_xlat1;
lowp vec4 u_xlat10_1;
vec4 u_xlat2;
vec4 u_xlat3;
lowp vec4 u_xlat10_3;
vec4 u_xlat4;
lowp vec4 u_xlat10_4;
vec4 u_xlat5;
lowp vec4 u_xlat10_5;
lowp vec4 u_xlat10_6;
vec4 u_xlat7;
lowp vec4 u_xlat10_7;
vec4 u_xlat8;
lowp vec4 u_xlat10_8;
vec4 u_xlat9;
vec3 u_xlat10;
bool u_xlatb10;
vec3 u_xlat13;
float u_xlat17;
float u_xlat20;
bool u_xlatb20;
float u_xlat23;
float u_xlat30;
void main()
{
    u_xlat0 = _MainTex_TexelSize.xyxy * vec4(0.0, -1.5, 0.0, 1.5) + vs_TEXCOORD0.xyxy;
    u_xlat0 = u_xlat0 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_1 = texture2D(_MainTex, u_xlat0.xy);
    u_xlat10_0 = texture2D(_MainTex, u_xlat0.zw);
    u_xlat2 = u_xlat10_0 + u_xlat10_1;
    u_xlat3.xyz = u_xlat2.xyz + u_xlat2.xyz;
    u_xlat4.xy = vs_TEXCOORD0.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_4 = texture2D(_MainTex, u_xlat4.xy);
    u_xlat3.xyz = (-u_xlat10_4.xyz) * vec3(4.0, 4.0, 4.0) + u_xlat3.xyz;
    u_xlat3.xyz = abs(u_xlat3.xyz) * vec3(0.25, 0.25, 0.25);
    u_xlat3.x = dot(u_xlat3.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat3.x = u_xlat3.x * 3.0 + -0.100000001;
    u_xlat5 = _MainTex_TexelSize.xyxy * vec4(-1.5, 0.0, 1.5, 0.0) + vs_TEXCOORD0.xyxy;
    u_xlat5 = u_xlat5 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_6 = texture2D(_MainTex, u_xlat5.xy);
    u_xlat10_5 = texture2D(_MainTex, u_xlat5.zw);
    u_xlat7 = u_xlat10_5 + u_xlat10_6;
    u_xlat8 = u_xlat10_4 + u_xlat10_4;
    u_xlat9 = u_xlat7 * vec4(2.0, 2.0, 2.0, 2.0) + u_xlat8;
    u_xlat13.xyz = u_xlat7.xyz + u_xlat7.xyz;
    u_xlat13.xyz = (-u_xlat10_4.xyz) * vec3(4.0, 4.0, 4.0) + u_xlat13.xyz;
    u_xlat13.xyz = abs(u_xlat13.xyz) * vec3(0.25, 0.25, 0.25);
    u_xlat13.x = dot(u_xlat13.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat13.x = u_xlat13.x * 3.0 + -0.100000001;
    u_xlat2 = u_xlat2 * vec4(2.0, 2.0, 2.0, 2.0) + u_xlat8;
    u_xlat7.xyz = u_xlat9.xyz * vec3(0.166666672, 0.166666672, 0.166666672);
    u_xlat8 = u_xlat9 * vec4(0.166666672, 0.166666672, 0.166666672, 0.166666672) + (-u_xlat10_4);
    u_xlat23 = dot(u_xlat7.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat3.x = u_xlat3.x / u_xlat23;
    u_xlat3.x = clamp(u_xlat3.x, 0.0, 1.0);
    u_xlat7 = u_xlat3.xxxx * u_xlat8 + u_xlat10_4;
    u_xlat8 = u_xlat2 * vec4(0.166666672, 0.166666672, 0.166666672, 0.166666672) + (-u_xlat7);
    u_xlat2.xyz = u_xlat2.xyz * vec3(0.166666672, 0.166666672, 0.166666672);
    u_xlat2.x = dot(u_xlat2.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat2.x = u_xlat13.x / u_xlat2.x;
    u_xlat2.x = clamp(u_xlat2.x, 0.0, 1.0);
    u_xlat2 = u_xlat2.xxxx * u_xlat8 + u_xlat7;
    u_xlat3 = _MainTex_TexelSize.xyxy * vec4(0.0, 3.5, 0.0, 5.5) + vs_TEXCOORD0.xyxy;
    u_xlat3 = u_xlat3 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_7 = texture2D(_MainTex, u_xlat3.xy);
    u_xlat10_3 = texture2D(_MainTex, u_xlat3.zw);
    u_xlat0 = u_xlat10_0.wxyz + u_xlat10_7.wxyz;
    u_xlat0 = u_xlat10_3.wxyz + u_xlat0;
    u_xlat3 = _MainTex_TexelSize.xyxy * vec4(0.0, 7.5, 0.0, -3.5) + vs_TEXCOORD0.xyxy;
    u_xlat3 = u_xlat3 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_7 = texture2D(_MainTex, u_xlat3.xy);
    u_xlat10_3 = texture2D(_MainTex, u_xlat3.zw);
    u_xlat0 = u_xlat0 + u_xlat10_7.wxyz;
    u_xlat0 = u_xlat10_1.wxyz + u_xlat0;
    u_xlat0 = u_xlat10_3.wxyz + u_xlat0;
    u_xlat1 = _MainTex_TexelSize.xyxy * vec4(0.0, -5.5, 0.0, -7.5) + vs_TEXCOORD0.xyxy;
    u_xlat1 = u_xlat1 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_3 = texture2D(_MainTex, u_xlat1.xy);
    u_xlat10_1 = texture2D(_MainTex, u_xlat1.zw);
    u_xlat0 = u_xlat0 + u_xlat10_3.wxyz;
    u_xlat0 = u_xlat10_1.wxyz + u_xlat0;
    u_xlat10.xyz = u_xlat0.yzw * vec3(0.125, 0.125, 0.125);
    u_xlat0.x = u_xlat0.x * 0.25 + -1.0;
    u_xlat0.x = clamp(u_xlat0.x, 0.0, 1.0);
    u_xlat10.x = dot(u_xlat10.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat1 = _MainTex_TexelSize.xyxy * vec4(-1.0, 0.0, 1.0, 0.0) + vs_TEXCOORD0.xyxy;
    u_xlat1 = u_xlat1 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_3 = texture2D(_MainTex, u_xlat1.xy);
    u_xlat10_1 = texture2D(_MainTex, u_xlat1.zw);
    u_xlat20 = dot(u_xlat10_3.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat30 = (-u_xlat20) + u_xlat10.x;
    u_xlat7.x = dot(u_xlat10_4.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat17 = (-u_xlat20) + u_xlat7.x;
    u_xlatb20 = u_xlat20==u_xlat7.x;
    u_xlat30 = u_xlat30 / u_xlat17;
    u_xlat30 = clamp(u_xlat30, 0.0, 1.0);
    u_xlat20 = (u_xlatb20) ? 0.0 : u_xlat30;
    u_xlat8 = (-u_xlat10_3) + u_xlat10_4;
    u_xlat3 = vec4(u_xlat20) * u_xlat8 + u_xlat10_3;
    u_xlat3 = (-u_xlat10_1) + u_xlat3;
    u_xlat10.x = u_xlat10.x + (-u_xlat7.x);
    u_xlat20 = dot(u_xlat10_1.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat30 = (-u_xlat20) + u_xlat7.x;
    u_xlatb20 = u_xlat20==u_xlat7.x;
    u_xlat10.x = u_xlat10.x / u_xlat30;
    u_xlat10.x = u_xlat10.x + 1.0;
    u_xlat10.x = clamp(u_xlat10.x, 0.0, 1.0);
    u_xlat10.x = (u_xlatb20) ? 0.0 : u_xlat10.x;
    u_xlat1 = u_xlat10.xxxx * u_xlat3 + u_xlat10_1;
    u_xlat1 = (-u_xlat2) + u_xlat1;
    u_xlat1 = u_xlat0.xxxx * u_xlat1 + u_xlat2;
    u_xlatb0 = 0.0<u_xlat0.x;
    u_xlat3 = _MainTex_TexelSize.xyxy * vec4(3.5, 0.0, 5.5, 0.0) + vs_TEXCOORD0.xyxy;
    u_xlat3 = u_xlat3 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_8 = texture2D(_MainTex, u_xlat3.xy);
    u_xlat10_3 = texture2D(_MainTex, u_xlat3.zw);
    u_xlat5 = u_xlat10_5.wxyz + u_xlat10_8.wxyz;
    u_xlat3 = u_xlat10_3.wxyz + u_xlat5;
    u_xlat5 = _MainTex_TexelSize.xyxy * vec4(7.5, 0.0, -3.5, 0.0) + vs_TEXCOORD0.xyxy;
    u_xlat5 = u_xlat5 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_8 = texture2D(_MainTex, u_xlat5.xy);
    u_xlat10_5 = texture2D(_MainTex, u_xlat5.zw);
    u_xlat3 = u_xlat3 + u_xlat10_8.wxyz;
    u_xlat3 = u_xlat10_6.wxyz + u_xlat3;
    u_xlat3 = u_xlat10_5.wxyz + u_xlat3;
    u_xlat5 = _MainTex_TexelSize.xyxy * vec4(-5.5, 0.0, -7.5, 0.0) + vs_TEXCOORD0.xyxy;
    u_xlat5 = u_xlat5 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_6 = texture2D(_MainTex, u_xlat5.xy);
    u_xlat10_5 = texture2D(_MainTex, u_xlat5.zw);
    u_xlat3 = u_xlat3 + u_xlat10_6.wxyz;
    u_xlat3 = u_xlat10_5.wxyz + u_xlat3;
    u_xlat10.xyz = u_xlat3.yzw * vec3(0.125, 0.125, 0.125);
    u_xlat3.x = u_xlat3.x * 0.25 + -1.0;
    u_xlat3.x = clamp(u_xlat3.x, 0.0, 1.0);
    u_xlat10.x = dot(u_xlat10.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat5 = _MainTex_TexelSize.xyxy * vec4(0.0, -1.0, 0.0, 1.0) + vs_TEXCOORD0.xyxy;
    u_xlat5 = u_xlat5 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_6 = texture2D(_MainTex, u_xlat5.xy);
    u_xlat10_5 = texture2D(_MainTex, u_xlat5.zw);
    u_xlat20 = dot(u_xlat10_6.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat30 = (-u_xlat20) + u_xlat10.x;
    u_xlat10.x = (-u_xlat7.x) + u_xlat10.x;
    u_xlat13.x = (-u_xlat20) + u_xlat7.x;
    u_xlatb20 = u_xlat20==u_xlat7.x;
    u_xlat30 = u_xlat30 / u_xlat13.x;
    u_xlat30 = clamp(u_xlat30, 0.0, 1.0);
    u_xlat20 = (u_xlatb20) ? 0.0 : u_xlat30;
    u_xlat4 = u_xlat10_4 + (-u_xlat10_6);
    u_xlat4 = vec4(u_xlat20) * u_xlat4 + u_xlat10_6;
    u_xlat4 = (-u_xlat10_5) + u_xlat4;
    u_xlat20 = dot(u_xlat10_5.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat30 = (-u_xlat20) + u_xlat7.x;
    u_xlatb20 = u_xlat20==u_xlat7.x;
    u_xlat10.x = u_xlat10.x / u_xlat30;
    u_xlat10.x = u_xlat10.x + 1.0;
    u_xlat10.x = clamp(u_xlat10.x, 0.0, 1.0);
    u_xlat10.x = (u_xlatb20) ? 0.0 : u_xlat10.x;
    u_xlat4 = u_xlat10.xxxx * u_xlat4 + u_xlat10_5;
    u_xlat4 = (-u_xlat1) + u_xlat4;
    u_xlat1 = u_xlat3.xxxx * u_xlat4 + u_xlat1;
    u_xlatb10 = 0.0<u_xlat3.x;
    u_xlatb0 = u_xlatb0 || u_xlatb10;
    u_xlat0 = (bool(u_xlatb0)) ? u_xlat1 : u_xlat2;
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
attribute highp vec4 in_POSITION0;
attribute mediump vec2 in_TEXCOORD0;
varying highp vec2 vs_TEXCOORD0;
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
uniform lowp sampler2D _MainTex;
varying highp vec2 vs_TEXCOORD0;
#define SV_Target0 gl_FragData[0]
vec4 u_xlat0;
lowp vec4 u_xlat10_0;
bool u_xlatb0;
vec4 u_xlat1;
lowp vec4 u_xlat10_1;
vec4 u_xlat2;
vec4 u_xlat3;
lowp vec4 u_xlat10_3;
vec4 u_xlat4;
lowp vec4 u_xlat10_4;
vec4 u_xlat5;
lowp vec4 u_xlat10_5;
lowp vec4 u_xlat10_6;
vec4 u_xlat7;
lowp vec4 u_xlat10_7;
vec4 u_xlat8;
lowp vec4 u_xlat10_8;
vec4 u_xlat9;
vec3 u_xlat10;
bool u_xlatb10;
vec3 u_xlat13;
float u_xlat17;
float u_xlat20;
bool u_xlatb20;
float u_xlat23;
float u_xlat30;
void main()
{
    u_xlat0 = _MainTex_TexelSize.xyxy * vec4(0.0, -1.5, 0.0, 1.5) + vs_TEXCOORD0.xyxy;
    u_xlat0 = u_xlat0 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_1 = texture2D(_MainTex, u_xlat0.xy);
    u_xlat10_0 = texture2D(_MainTex, u_xlat0.zw);
    u_xlat2 = u_xlat10_0 + u_xlat10_1;
    u_xlat3.xyz = u_xlat2.xyz + u_xlat2.xyz;
    u_xlat4.xy = vs_TEXCOORD0.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_4 = texture2D(_MainTex, u_xlat4.xy);
    u_xlat3.xyz = (-u_xlat10_4.xyz) * vec3(4.0, 4.0, 4.0) + u_xlat3.xyz;
    u_xlat3.xyz = abs(u_xlat3.xyz) * vec3(0.25, 0.25, 0.25);
    u_xlat3.x = dot(u_xlat3.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat3.x = u_xlat3.x * 3.0 + -0.100000001;
    u_xlat5 = _MainTex_TexelSize.xyxy * vec4(-1.5, 0.0, 1.5, 0.0) + vs_TEXCOORD0.xyxy;
    u_xlat5 = u_xlat5 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_6 = texture2D(_MainTex, u_xlat5.xy);
    u_xlat10_5 = texture2D(_MainTex, u_xlat5.zw);
    u_xlat7 = u_xlat10_5 + u_xlat10_6;
    u_xlat8 = u_xlat10_4 + u_xlat10_4;
    u_xlat9 = u_xlat7 * vec4(2.0, 2.0, 2.0, 2.0) + u_xlat8;
    u_xlat13.xyz = u_xlat7.xyz + u_xlat7.xyz;
    u_xlat13.xyz = (-u_xlat10_4.xyz) * vec3(4.0, 4.0, 4.0) + u_xlat13.xyz;
    u_xlat13.xyz = abs(u_xlat13.xyz) * vec3(0.25, 0.25, 0.25);
    u_xlat13.x = dot(u_xlat13.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat13.x = u_xlat13.x * 3.0 + -0.100000001;
    u_xlat2 = u_xlat2 * vec4(2.0, 2.0, 2.0, 2.0) + u_xlat8;
    u_xlat7.xyz = u_xlat9.xyz * vec3(0.166666672, 0.166666672, 0.166666672);
    u_xlat8 = u_xlat9 * vec4(0.166666672, 0.166666672, 0.166666672, 0.166666672) + (-u_xlat10_4);
    u_xlat23 = dot(u_xlat7.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat3.x = u_xlat3.x / u_xlat23;
    u_xlat3.x = clamp(u_xlat3.x, 0.0, 1.0);
    u_xlat7 = u_xlat3.xxxx * u_xlat8 + u_xlat10_4;
    u_xlat8 = u_xlat2 * vec4(0.166666672, 0.166666672, 0.166666672, 0.166666672) + (-u_xlat7);
    u_xlat2.xyz = u_xlat2.xyz * vec3(0.166666672, 0.166666672, 0.166666672);
    u_xlat2.x = dot(u_xlat2.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat2.x = u_xlat13.x / u_xlat2.x;
    u_xlat2.x = clamp(u_xlat2.x, 0.0, 1.0);
    u_xlat2 = u_xlat2.xxxx * u_xlat8 + u_xlat7;
    u_xlat3 = _MainTex_TexelSize.xyxy * vec4(0.0, 3.5, 0.0, 5.5) + vs_TEXCOORD0.xyxy;
    u_xlat3 = u_xlat3 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_7 = texture2D(_MainTex, u_xlat3.xy);
    u_xlat10_3 = texture2D(_MainTex, u_xlat3.zw);
    u_xlat0 = u_xlat10_0.wxyz + u_xlat10_7.wxyz;
    u_xlat0 = u_xlat10_3.wxyz + u_xlat0;
    u_xlat3 = _MainTex_TexelSize.xyxy * vec4(0.0, 7.5, 0.0, -3.5) + vs_TEXCOORD0.xyxy;
    u_xlat3 = u_xlat3 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_7 = texture2D(_MainTex, u_xlat3.xy);
    u_xlat10_3 = texture2D(_MainTex, u_xlat3.zw);
    u_xlat0 = u_xlat0 + u_xlat10_7.wxyz;
    u_xlat0 = u_xlat10_1.wxyz + u_xlat0;
    u_xlat0 = u_xlat10_3.wxyz + u_xlat0;
    u_xlat1 = _MainTex_TexelSize.xyxy * vec4(0.0, -5.5, 0.0, -7.5) + vs_TEXCOORD0.xyxy;
    u_xlat1 = u_xlat1 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_3 = texture2D(_MainTex, u_xlat1.xy);
    u_xlat10_1 = texture2D(_MainTex, u_xlat1.zw);
    u_xlat0 = u_xlat0 + u_xlat10_3.wxyz;
    u_xlat0 = u_xlat10_1.wxyz + u_xlat0;
    u_xlat10.xyz = u_xlat0.yzw * vec3(0.125, 0.125, 0.125);
    u_xlat0.x = u_xlat0.x * 0.25 + -1.0;
    u_xlat0.x = clamp(u_xlat0.x, 0.0, 1.0);
    u_xlat10.x = dot(u_xlat10.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat1 = _MainTex_TexelSize.xyxy * vec4(-1.0, 0.0, 1.0, 0.0) + vs_TEXCOORD0.xyxy;
    u_xlat1 = u_xlat1 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_3 = texture2D(_MainTex, u_xlat1.xy);
    u_xlat10_1 = texture2D(_MainTex, u_xlat1.zw);
    u_xlat20 = dot(u_xlat10_3.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat30 = (-u_xlat20) + u_xlat10.x;
    u_xlat7.x = dot(u_xlat10_4.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat17 = (-u_xlat20) + u_xlat7.x;
    u_xlatb20 = u_xlat20==u_xlat7.x;
    u_xlat30 = u_xlat30 / u_xlat17;
    u_xlat30 = clamp(u_xlat30, 0.0, 1.0);
    u_xlat20 = (u_xlatb20) ? 0.0 : u_xlat30;
    u_xlat8 = (-u_xlat10_3) + u_xlat10_4;
    u_xlat3 = vec4(u_xlat20) * u_xlat8 + u_xlat10_3;
    u_xlat3 = (-u_xlat10_1) + u_xlat3;
    u_xlat10.x = u_xlat10.x + (-u_xlat7.x);
    u_xlat20 = dot(u_xlat10_1.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat30 = (-u_xlat20) + u_xlat7.x;
    u_xlatb20 = u_xlat20==u_xlat7.x;
    u_xlat10.x = u_xlat10.x / u_xlat30;
    u_xlat10.x = u_xlat10.x + 1.0;
    u_xlat10.x = clamp(u_xlat10.x, 0.0, 1.0);
    u_xlat10.x = (u_xlatb20) ? 0.0 : u_xlat10.x;
    u_xlat1 = u_xlat10.xxxx * u_xlat3 + u_xlat10_1;
    u_xlat1 = (-u_xlat2) + u_xlat1;
    u_xlat1 = u_xlat0.xxxx * u_xlat1 + u_xlat2;
    u_xlatb0 = 0.0<u_xlat0.x;
    u_xlat3 = _MainTex_TexelSize.xyxy * vec4(3.5, 0.0, 5.5, 0.0) + vs_TEXCOORD0.xyxy;
    u_xlat3 = u_xlat3 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_8 = texture2D(_MainTex, u_xlat3.xy);
    u_xlat10_3 = texture2D(_MainTex, u_xlat3.zw);
    u_xlat5 = u_xlat10_5.wxyz + u_xlat10_8.wxyz;
    u_xlat3 = u_xlat10_3.wxyz + u_xlat5;
    u_xlat5 = _MainTex_TexelSize.xyxy * vec4(7.5, 0.0, -3.5, 0.0) + vs_TEXCOORD0.xyxy;
    u_xlat5 = u_xlat5 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_8 = texture2D(_MainTex, u_xlat5.xy);
    u_xlat10_5 = texture2D(_MainTex, u_xlat5.zw);
    u_xlat3 = u_xlat3 + u_xlat10_8.wxyz;
    u_xlat3 = u_xlat10_6.wxyz + u_xlat3;
    u_xlat3 = u_xlat10_5.wxyz + u_xlat3;
    u_xlat5 = _MainTex_TexelSize.xyxy * vec4(-5.5, 0.0, -7.5, 0.0) + vs_TEXCOORD0.xyxy;
    u_xlat5 = u_xlat5 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_6 = texture2D(_MainTex, u_xlat5.xy);
    u_xlat10_5 = texture2D(_MainTex, u_xlat5.zw);
    u_xlat3 = u_xlat3 + u_xlat10_6.wxyz;
    u_xlat3 = u_xlat10_5.wxyz + u_xlat3;
    u_xlat10.xyz = u_xlat3.yzw * vec3(0.125, 0.125, 0.125);
    u_xlat3.x = u_xlat3.x * 0.25 + -1.0;
    u_xlat3.x = clamp(u_xlat3.x, 0.0, 1.0);
    u_xlat10.x = dot(u_xlat10.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat5 = _MainTex_TexelSize.xyxy * vec4(0.0, -1.0, 0.0, 1.0) + vs_TEXCOORD0.xyxy;
    u_xlat5 = u_xlat5 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_6 = texture2D(_MainTex, u_xlat5.xy);
    u_xlat10_5 = texture2D(_MainTex, u_xlat5.zw);
    u_xlat20 = dot(u_xlat10_6.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat30 = (-u_xlat20) + u_xlat10.x;
    u_xlat10.x = (-u_xlat7.x) + u_xlat10.x;
    u_xlat13.x = (-u_xlat20) + u_xlat7.x;
    u_xlatb20 = u_xlat20==u_xlat7.x;
    u_xlat30 = u_xlat30 / u_xlat13.x;
    u_xlat30 = clamp(u_xlat30, 0.0, 1.0);
    u_xlat20 = (u_xlatb20) ? 0.0 : u_xlat30;
    u_xlat4 = u_xlat10_4 + (-u_xlat10_6);
    u_xlat4 = vec4(u_xlat20) * u_xlat4 + u_xlat10_6;
    u_xlat4 = (-u_xlat10_5) + u_xlat4;
    u_xlat20 = dot(u_xlat10_5.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat30 = (-u_xlat20) + u_xlat7.x;
    u_xlatb20 = u_xlat20==u_xlat7.x;
    u_xlat10.x = u_xlat10.x / u_xlat30;
    u_xlat10.x = u_xlat10.x + 1.0;
    u_xlat10.x = clamp(u_xlat10.x, 0.0, 1.0);
    u_xlat10.x = (u_xlatb20) ? 0.0 : u_xlat10.x;
    u_xlat4 = u_xlat10.xxxx * u_xlat4 + u_xlat10_5;
    u_xlat4 = (-u_xlat1) + u_xlat4;
    u_xlat1 = u_xlat3.xxxx * u_xlat4 + u_xlat1;
    u_xlatb10 = 0.0<u_xlat3.x;
    u_xlatb0 = u_xlatb0 || u_xlatb10;
    u_xlat0 = (bool(u_xlatb0)) ? u_xlat1 : u_xlat2;
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
attribute highp vec4 in_POSITION0;
attribute mediump vec2 in_TEXCOORD0;
varying highp vec2 vs_TEXCOORD0;
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
uniform lowp sampler2D _MainTex;
varying highp vec2 vs_TEXCOORD0;
#define SV_Target0 gl_FragData[0]
vec4 u_xlat0;
lowp vec4 u_xlat10_0;
bool u_xlatb0;
vec4 u_xlat1;
lowp vec4 u_xlat10_1;
vec4 u_xlat2;
vec4 u_xlat3;
lowp vec4 u_xlat10_3;
vec4 u_xlat4;
lowp vec4 u_xlat10_4;
vec4 u_xlat5;
lowp vec4 u_xlat10_5;
lowp vec4 u_xlat10_6;
vec4 u_xlat7;
lowp vec4 u_xlat10_7;
vec4 u_xlat8;
lowp vec4 u_xlat10_8;
vec4 u_xlat9;
vec3 u_xlat10;
bool u_xlatb10;
vec3 u_xlat13;
float u_xlat17;
float u_xlat20;
bool u_xlatb20;
float u_xlat23;
float u_xlat30;
void main()
{
    u_xlat0 = _MainTex_TexelSize.xyxy * vec4(0.0, -1.5, 0.0, 1.5) + vs_TEXCOORD0.xyxy;
    u_xlat0 = u_xlat0 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_1 = texture2D(_MainTex, u_xlat0.xy);
    u_xlat10_0 = texture2D(_MainTex, u_xlat0.zw);
    u_xlat2 = u_xlat10_0 + u_xlat10_1;
    u_xlat3.xyz = u_xlat2.xyz + u_xlat2.xyz;
    u_xlat4.xy = vs_TEXCOORD0.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_4 = texture2D(_MainTex, u_xlat4.xy);
    u_xlat3.xyz = (-u_xlat10_4.xyz) * vec3(4.0, 4.0, 4.0) + u_xlat3.xyz;
    u_xlat3.xyz = abs(u_xlat3.xyz) * vec3(0.25, 0.25, 0.25);
    u_xlat3.x = dot(u_xlat3.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat3.x = u_xlat3.x * 3.0 + -0.100000001;
    u_xlat5 = _MainTex_TexelSize.xyxy * vec4(-1.5, 0.0, 1.5, 0.0) + vs_TEXCOORD0.xyxy;
    u_xlat5 = u_xlat5 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_6 = texture2D(_MainTex, u_xlat5.xy);
    u_xlat10_5 = texture2D(_MainTex, u_xlat5.zw);
    u_xlat7 = u_xlat10_5 + u_xlat10_6;
    u_xlat8 = u_xlat10_4 + u_xlat10_4;
    u_xlat9 = u_xlat7 * vec4(2.0, 2.0, 2.0, 2.0) + u_xlat8;
    u_xlat13.xyz = u_xlat7.xyz + u_xlat7.xyz;
    u_xlat13.xyz = (-u_xlat10_4.xyz) * vec3(4.0, 4.0, 4.0) + u_xlat13.xyz;
    u_xlat13.xyz = abs(u_xlat13.xyz) * vec3(0.25, 0.25, 0.25);
    u_xlat13.x = dot(u_xlat13.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat13.x = u_xlat13.x * 3.0 + -0.100000001;
    u_xlat2 = u_xlat2 * vec4(2.0, 2.0, 2.0, 2.0) + u_xlat8;
    u_xlat7.xyz = u_xlat9.xyz * vec3(0.166666672, 0.166666672, 0.166666672);
    u_xlat8 = u_xlat9 * vec4(0.166666672, 0.166666672, 0.166666672, 0.166666672) + (-u_xlat10_4);
    u_xlat23 = dot(u_xlat7.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat3.x = u_xlat3.x / u_xlat23;
    u_xlat3.x = clamp(u_xlat3.x, 0.0, 1.0);
    u_xlat7 = u_xlat3.xxxx * u_xlat8 + u_xlat10_4;
    u_xlat8 = u_xlat2 * vec4(0.166666672, 0.166666672, 0.166666672, 0.166666672) + (-u_xlat7);
    u_xlat2.xyz = u_xlat2.xyz * vec3(0.166666672, 0.166666672, 0.166666672);
    u_xlat2.x = dot(u_xlat2.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat2.x = u_xlat13.x / u_xlat2.x;
    u_xlat2.x = clamp(u_xlat2.x, 0.0, 1.0);
    u_xlat2 = u_xlat2.xxxx * u_xlat8 + u_xlat7;
    u_xlat3 = _MainTex_TexelSize.xyxy * vec4(0.0, 3.5, 0.0, 5.5) + vs_TEXCOORD0.xyxy;
    u_xlat3 = u_xlat3 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_7 = texture2D(_MainTex, u_xlat3.xy);
    u_xlat10_3 = texture2D(_MainTex, u_xlat3.zw);
    u_xlat0 = u_xlat10_0.wxyz + u_xlat10_7.wxyz;
    u_xlat0 = u_xlat10_3.wxyz + u_xlat0;
    u_xlat3 = _MainTex_TexelSize.xyxy * vec4(0.0, 7.5, 0.0, -3.5) + vs_TEXCOORD0.xyxy;
    u_xlat3 = u_xlat3 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_7 = texture2D(_MainTex, u_xlat3.xy);
    u_xlat10_3 = texture2D(_MainTex, u_xlat3.zw);
    u_xlat0 = u_xlat0 + u_xlat10_7.wxyz;
    u_xlat0 = u_xlat10_1.wxyz + u_xlat0;
    u_xlat0 = u_xlat10_3.wxyz + u_xlat0;
    u_xlat1 = _MainTex_TexelSize.xyxy * vec4(0.0, -5.5, 0.0, -7.5) + vs_TEXCOORD0.xyxy;
    u_xlat1 = u_xlat1 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_3 = texture2D(_MainTex, u_xlat1.xy);
    u_xlat10_1 = texture2D(_MainTex, u_xlat1.zw);
    u_xlat0 = u_xlat0 + u_xlat10_3.wxyz;
    u_xlat0 = u_xlat10_1.wxyz + u_xlat0;
    u_xlat10.xyz = u_xlat0.yzw * vec3(0.125, 0.125, 0.125);
    u_xlat0.x = u_xlat0.x * 0.25 + -1.0;
    u_xlat0.x = clamp(u_xlat0.x, 0.0, 1.0);
    u_xlat10.x = dot(u_xlat10.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat1 = _MainTex_TexelSize.xyxy * vec4(-1.0, 0.0, 1.0, 0.0) + vs_TEXCOORD0.xyxy;
    u_xlat1 = u_xlat1 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_3 = texture2D(_MainTex, u_xlat1.xy);
    u_xlat10_1 = texture2D(_MainTex, u_xlat1.zw);
    u_xlat20 = dot(u_xlat10_3.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat30 = (-u_xlat20) + u_xlat10.x;
    u_xlat7.x = dot(u_xlat10_4.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat17 = (-u_xlat20) + u_xlat7.x;
    u_xlatb20 = u_xlat20==u_xlat7.x;
    u_xlat30 = u_xlat30 / u_xlat17;
    u_xlat30 = clamp(u_xlat30, 0.0, 1.0);
    u_xlat20 = (u_xlatb20) ? 0.0 : u_xlat30;
    u_xlat8 = (-u_xlat10_3) + u_xlat10_4;
    u_xlat3 = vec4(u_xlat20) * u_xlat8 + u_xlat10_3;
    u_xlat3 = (-u_xlat10_1) + u_xlat3;
    u_xlat10.x = u_xlat10.x + (-u_xlat7.x);
    u_xlat20 = dot(u_xlat10_1.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat30 = (-u_xlat20) + u_xlat7.x;
    u_xlatb20 = u_xlat20==u_xlat7.x;
    u_xlat10.x = u_xlat10.x / u_xlat30;
    u_xlat10.x = u_xlat10.x + 1.0;
    u_xlat10.x = clamp(u_xlat10.x, 0.0, 1.0);
    u_xlat10.x = (u_xlatb20) ? 0.0 : u_xlat10.x;
    u_xlat1 = u_xlat10.xxxx * u_xlat3 + u_xlat10_1;
    u_xlat1 = (-u_xlat2) + u_xlat1;
    u_xlat1 = u_xlat0.xxxx * u_xlat1 + u_xlat2;
    u_xlatb0 = 0.0<u_xlat0.x;
    u_xlat3 = _MainTex_TexelSize.xyxy * vec4(3.5, 0.0, 5.5, 0.0) + vs_TEXCOORD0.xyxy;
    u_xlat3 = u_xlat3 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_8 = texture2D(_MainTex, u_xlat3.xy);
    u_xlat10_3 = texture2D(_MainTex, u_xlat3.zw);
    u_xlat5 = u_xlat10_5.wxyz + u_xlat10_8.wxyz;
    u_xlat3 = u_xlat10_3.wxyz + u_xlat5;
    u_xlat5 = _MainTex_TexelSize.xyxy * vec4(7.5, 0.0, -3.5, 0.0) + vs_TEXCOORD0.xyxy;
    u_xlat5 = u_xlat5 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_8 = texture2D(_MainTex, u_xlat5.xy);
    u_xlat10_5 = texture2D(_MainTex, u_xlat5.zw);
    u_xlat3 = u_xlat3 + u_xlat10_8.wxyz;
    u_xlat3 = u_xlat10_6.wxyz + u_xlat3;
    u_xlat3 = u_xlat10_5.wxyz + u_xlat3;
    u_xlat5 = _MainTex_TexelSize.xyxy * vec4(-5.5, 0.0, -7.5, 0.0) + vs_TEXCOORD0.xyxy;
    u_xlat5 = u_xlat5 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_6 = texture2D(_MainTex, u_xlat5.xy);
    u_xlat10_5 = texture2D(_MainTex, u_xlat5.zw);
    u_xlat3 = u_xlat3 + u_xlat10_6.wxyz;
    u_xlat3 = u_xlat10_5.wxyz + u_xlat3;
    u_xlat10.xyz = u_xlat3.yzw * vec3(0.125, 0.125, 0.125);
    u_xlat3.x = u_xlat3.x * 0.25 + -1.0;
    u_xlat3.x = clamp(u_xlat3.x, 0.0, 1.0);
    u_xlat10.x = dot(u_xlat10.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat5 = _MainTex_TexelSize.xyxy * vec4(0.0, -1.0, 0.0, 1.0) + vs_TEXCOORD0.xyxy;
    u_xlat5 = u_xlat5 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_6 = texture2D(_MainTex, u_xlat5.xy);
    u_xlat10_5 = texture2D(_MainTex, u_xlat5.zw);
    u_xlat20 = dot(u_xlat10_6.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat30 = (-u_xlat20) + u_xlat10.x;
    u_xlat10.x = (-u_xlat7.x) + u_xlat10.x;
    u_xlat13.x = (-u_xlat20) + u_xlat7.x;
    u_xlatb20 = u_xlat20==u_xlat7.x;
    u_xlat30 = u_xlat30 / u_xlat13.x;
    u_xlat30 = clamp(u_xlat30, 0.0, 1.0);
    u_xlat20 = (u_xlatb20) ? 0.0 : u_xlat30;
    u_xlat4 = u_xlat10_4 + (-u_xlat10_6);
    u_xlat4 = vec4(u_xlat20) * u_xlat4 + u_xlat10_6;
    u_xlat4 = (-u_xlat10_5) + u_xlat4;
    u_xlat20 = dot(u_xlat10_5.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat30 = (-u_xlat20) + u_xlat7.x;
    u_xlatb20 = u_xlat20==u_xlat7.x;
    u_xlat10.x = u_xlat10.x / u_xlat30;
    u_xlat10.x = u_xlat10.x + 1.0;
    u_xlat10.x = clamp(u_xlat10.x, 0.0, 1.0);
    u_xlat10.x = (u_xlatb20) ? 0.0 : u_xlat10.x;
    u_xlat4 = u_xlat10.xxxx * u_xlat4 + u_xlat10_5;
    u_xlat4 = (-u_xlat1) + u_xlat4;
    u_xlat1 = u_xlat3.xxxx * u_xlat4 + u_xlat1;
    u_xlatb10 = 0.0<u_xlat3.x;
    u_xlatb0 = u_xlatb0 || u_xlatb10;
    u_xlat0 = (bool(u_xlatb0)) ? u_xlat1 : u_xlat2;
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
in highp vec4 in_POSITION0;
in mediump vec2 in_TEXCOORD0;
out highp vec2 vs_TEXCOORD0;
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
    return;
}

#endif
#ifdef FRAGMENT
#version 300 es

precision highp float;
precision highp int;
uniform 	vec4 _MainTex_TexelSize;
uniform 	mediump vec4 _MainTex_ST;
uniform mediump sampler2D _MainTex;
in highp vec2 vs_TEXCOORD0;
layout(location = 0) out mediump vec4 SV_Target0;
vec4 u_xlat0;
mediump vec4 u_xlat16_0;
bool u_xlatb0;
vec4 u_xlat1;
mediump vec4 u_xlat16_1;
vec4 u_xlat2;
vec4 u_xlat3;
mediump vec4 u_xlat16_3;
vec4 u_xlat4;
mediump vec4 u_xlat16_4;
vec4 u_xlat5;
mediump vec4 u_xlat16_5;
mediump vec4 u_xlat16_6;
vec4 u_xlat7;
mediump vec4 u_xlat16_7;
vec4 u_xlat8;
mediump vec4 u_xlat16_8;
vec4 u_xlat9;
vec3 u_xlat10;
bool u_xlatb10;
vec3 u_xlat13;
float u_xlat17;
float u_xlat20;
bool u_xlatb20;
float u_xlat23;
float u_xlat30;
void main()
{
    u_xlat0 = _MainTex_TexelSize.xyxy * vec4(0.0, -1.5, 0.0, 1.5) + vs_TEXCOORD0.xyxy;
    u_xlat0 = u_xlat0 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_1 = texture(_MainTex, u_xlat0.xy);
    u_xlat16_0 = texture(_MainTex, u_xlat0.zw);
    u_xlat2 = u_xlat16_0 + u_xlat16_1;
    u_xlat3.xyz = u_xlat2.xyz + u_xlat2.xyz;
    u_xlat4.xy = vs_TEXCOORD0.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_4 = texture(_MainTex, u_xlat4.xy);
    u_xlat3.xyz = (-u_xlat16_4.xyz) * vec3(4.0, 4.0, 4.0) + u_xlat3.xyz;
    u_xlat3.xyz = abs(u_xlat3.xyz) * vec3(0.25, 0.25, 0.25);
    u_xlat3.x = dot(u_xlat3.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat3.x = u_xlat3.x * 3.0 + -0.100000001;
    u_xlat5 = _MainTex_TexelSize.xyxy * vec4(-1.5, 0.0, 1.5, 0.0) + vs_TEXCOORD0.xyxy;
    u_xlat5 = u_xlat5 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_6 = texture(_MainTex, u_xlat5.xy);
    u_xlat16_5 = texture(_MainTex, u_xlat5.zw);
    u_xlat7 = u_xlat16_5 + u_xlat16_6;
    u_xlat8 = u_xlat16_4 + u_xlat16_4;
    u_xlat9 = u_xlat7 * vec4(2.0, 2.0, 2.0, 2.0) + u_xlat8;
    u_xlat13.xyz = u_xlat7.xyz + u_xlat7.xyz;
    u_xlat13.xyz = (-u_xlat16_4.xyz) * vec3(4.0, 4.0, 4.0) + u_xlat13.xyz;
    u_xlat13.xyz = abs(u_xlat13.xyz) * vec3(0.25, 0.25, 0.25);
    u_xlat13.x = dot(u_xlat13.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat13.x = u_xlat13.x * 3.0 + -0.100000001;
    u_xlat2 = u_xlat2 * vec4(2.0, 2.0, 2.0, 2.0) + u_xlat8;
    u_xlat7.xyz = u_xlat9.xyz * vec3(0.166666672, 0.166666672, 0.166666672);
    u_xlat8 = u_xlat9 * vec4(0.166666672, 0.166666672, 0.166666672, 0.166666672) + (-u_xlat16_4);
    u_xlat23 = dot(u_xlat7.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat3.x = u_xlat3.x / u_xlat23;
#ifdef UNITY_ADRENO_ES3
    u_xlat3.x = min(max(u_xlat3.x, 0.0), 1.0);
#else
    u_xlat3.x = clamp(u_xlat3.x, 0.0, 1.0);
#endif
    u_xlat7 = u_xlat3.xxxx * u_xlat8 + u_xlat16_4;
    u_xlat8 = u_xlat2 * vec4(0.166666672, 0.166666672, 0.166666672, 0.166666672) + (-u_xlat7);
    u_xlat2.xyz = u_xlat2.xyz * vec3(0.166666672, 0.166666672, 0.166666672);
    u_xlat2.x = dot(u_xlat2.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat2.x = u_xlat13.x / u_xlat2.x;
#ifdef UNITY_ADRENO_ES3
    u_xlat2.x = min(max(u_xlat2.x, 0.0), 1.0);
#else
    u_xlat2.x = clamp(u_xlat2.x, 0.0, 1.0);
#endif
    u_xlat2 = u_xlat2.xxxx * u_xlat8 + u_xlat7;
    u_xlat3 = _MainTex_TexelSize.xyxy * vec4(0.0, 3.5, 0.0, 5.5) + vs_TEXCOORD0.xyxy;
    u_xlat3 = u_xlat3 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_7 = texture(_MainTex, u_xlat3.xy);
    u_xlat16_3 = texture(_MainTex, u_xlat3.zw);
    u_xlat0 = u_xlat16_0.wxyz + u_xlat16_7.wxyz;
    u_xlat0 = u_xlat16_3.wxyz + u_xlat0;
    u_xlat3 = _MainTex_TexelSize.xyxy * vec4(0.0, 7.5, 0.0, -3.5) + vs_TEXCOORD0.xyxy;
    u_xlat3 = u_xlat3 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_7 = texture(_MainTex, u_xlat3.xy);
    u_xlat16_3 = texture(_MainTex, u_xlat3.zw);
    u_xlat0 = u_xlat0 + u_xlat16_7.wxyz;
    u_xlat0 = u_xlat16_1.wxyz + u_xlat0;
    u_xlat0 = u_xlat16_3.wxyz + u_xlat0;
    u_xlat1 = _MainTex_TexelSize.xyxy * vec4(0.0, -5.5, 0.0, -7.5) + vs_TEXCOORD0.xyxy;
    u_xlat1 = u_xlat1 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_3 = texture(_MainTex, u_xlat1.xy);
    u_xlat16_1 = texture(_MainTex, u_xlat1.zw);
    u_xlat0 = u_xlat0 + u_xlat16_3.wxyz;
    u_xlat0 = u_xlat16_1.wxyz + u_xlat0;
    u_xlat10.xyz = u_xlat0.yzw * vec3(0.125, 0.125, 0.125);
    u_xlat0.x = u_xlat0.x * 0.25 + -1.0;
#ifdef UNITY_ADRENO_ES3
    u_xlat0.x = min(max(u_xlat0.x, 0.0), 1.0);
#else
    u_xlat0.x = clamp(u_xlat0.x, 0.0, 1.0);
#endif
    u_xlat10.x = dot(u_xlat10.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat1 = _MainTex_TexelSize.xyxy * vec4(-1.0, 0.0, 1.0, 0.0) + vs_TEXCOORD0.xyxy;
    u_xlat1 = u_xlat1 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_3 = texture(_MainTex, u_xlat1.xy);
    u_xlat16_1 = texture(_MainTex, u_xlat1.zw);
    u_xlat20 = dot(u_xlat16_3.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat30 = (-u_xlat20) + u_xlat10.x;
    u_xlat7.x = dot(u_xlat16_4.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat17 = (-u_xlat20) + u_xlat7.x;
#ifdef UNITY_ADRENO_ES3
    u_xlatb20 = !!(u_xlat20==u_xlat7.x);
#else
    u_xlatb20 = u_xlat20==u_xlat7.x;
#endif
    u_xlat30 = u_xlat30 / u_xlat17;
#ifdef UNITY_ADRENO_ES3
    u_xlat30 = min(max(u_xlat30, 0.0), 1.0);
#else
    u_xlat30 = clamp(u_xlat30, 0.0, 1.0);
#endif
    u_xlat20 = (u_xlatb20) ? 0.0 : u_xlat30;
    u_xlat8 = (-u_xlat16_3) + u_xlat16_4;
    u_xlat3 = vec4(u_xlat20) * u_xlat8 + u_xlat16_3;
    u_xlat3 = (-u_xlat16_1) + u_xlat3;
    u_xlat10.x = u_xlat10.x + (-u_xlat7.x);
    u_xlat20 = dot(u_xlat16_1.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat30 = (-u_xlat20) + u_xlat7.x;
#ifdef UNITY_ADRENO_ES3
    u_xlatb20 = !!(u_xlat20==u_xlat7.x);
#else
    u_xlatb20 = u_xlat20==u_xlat7.x;
#endif
    u_xlat10.x = u_xlat10.x / u_xlat30;
    u_xlat10.x = u_xlat10.x + 1.0;
#ifdef UNITY_ADRENO_ES3
    u_xlat10.x = min(max(u_xlat10.x, 0.0), 1.0);
#else
    u_xlat10.x = clamp(u_xlat10.x, 0.0, 1.0);
#endif
    u_xlat10.x = (u_xlatb20) ? 0.0 : u_xlat10.x;
    u_xlat1 = u_xlat10.xxxx * u_xlat3 + u_xlat16_1;
    u_xlat1 = (-u_xlat2) + u_xlat1;
    u_xlat1 = u_xlat0.xxxx * u_xlat1 + u_xlat2;
#ifdef UNITY_ADRENO_ES3
    u_xlatb0 = !!(0.0<u_xlat0.x);
#else
    u_xlatb0 = 0.0<u_xlat0.x;
#endif
    u_xlat3 = _MainTex_TexelSize.xyxy * vec4(3.5, 0.0, 5.5, 0.0) + vs_TEXCOORD0.xyxy;
    u_xlat3 = u_xlat3 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_8 = texture(_MainTex, u_xlat3.xy);
    u_xlat16_3 = texture(_MainTex, u_xlat3.zw);
    u_xlat5 = u_xlat16_5.wxyz + u_xlat16_8.wxyz;
    u_xlat3 = u_xlat16_3.wxyz + u_xlat5;
    u_xlat5 = _MainTex_TexelSize.xyxy * vec4(7.5, 0.0, -3.5, 0.0) + vs_TEXCOORD0.xyxy;
    u_xlat5 = u_xlat5 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_8 = texture(_MainTex, u_xlat5.xy);
    u_xlat16_5 = texture(_MainTex, u_xlat5.zw);
    u_xlat3 = u_xlat3 + u_xlat16_8.wxyz;
    u_xlat3 = u_xlat16_6.wxyz + u_xlat3;
    u_xlat3 = u_xlat16_5.wxyz + u_xlat3;
    u_xlat5 = _MainTex_TexelSize.xyxy * vec4(-5.5, 0.0, -7.5, 0.0) + vs_TEXCOORD0.xyxy;
    u_xlat5 = u_xlat5 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_6 = texture(_MainTex, u_xlat5.xy);
    u_xlat16_5 = texture(_MainTex, u_xlat5.zw);
    u_xlat3 = u_xlat3 + u_xlat16_6.wxyz;
    u_xlat3 = u_xlat16_5.wxyz + u_xlat3;
    u_xlat10.xyz = u_xlat3.yzw * vec3(0.125, 0.125, 0.125);
    u_xlat3.x = u_xlat3.x * 0.25 + -1.0;
#ifdef UNITY_ADRENO_ES3
    u_xlat3.x = min(max(u_xlat3.x, 0.0), 1.0);
#else
    u_xlat3.x = clamp(u_xlat3.x, 0.0, 1.0);
#endif
    u_xlat10.x = dot(u_xlat10.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat5 = _MainTex_TexelSize.xyxy * vec4(0.0, -1.0, 0.0, 1.0) + vs_TEXCOORD0.xyxy;
    u_xlat5 = u_xlat5 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_6 = texture(_MainTex, u_xlat5.xy);
    u_xlat16_5 = texture(_MainTex, u_xlat5.zw);
    u_xlat20 = dot(u_xlat16_6.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat30 = (-u_xlat20) + u_xlat10.x;
    u_xlat10.x = (-u_xlat7.x) + u_xlat10.x;
    u_xlat13.x = (-u_xlat20) + u_xlat7.x;
#ifdef UNITY_ADRENO_ES3
    u_xlatb20 = !!(u_xlat20==u_xlat7.x);
#else
    u_xlatb20 = u_xlat20==u_xlat7.x;
#endif
    u_xlat30 = u_xlat30 / u_xlat13.x;
#ifdef UNITY_ADRENO_ES3
    u_xlat30 = min(max(u_xlat30, 0.0), 1.0);
#else
    u_xlat30 = clamp(u_xlat30, 0.0, 1.0);
#endif
    u_xlat20 = (u_xlatb20) ? 0.0 : u_xlat30;
    u_xlat4 = u_xlat16_4 + (-u_xlat16_6);
    u_xlat4 = vec4(u_xlat20) * u_xlat4 + u_xlat16_6;
    u_xlat4 = (-u_xlat16_5) + u_xlat4;
    u_xlat20 = dot(u_xlat16_5.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat30 = (-u_xlat20) + u_xlat7.x;
#ifdef UNITY_ADRENO_ES3
    u_xlatb20 = !!(u_xlat20==u_xlat7.x);
#else
    u_xlatb20 = u_xlat20==u_xlat7.x;
#endif
    u_xlat10.x = u_xlat10.x / u_xlat30;
    u_xlat10.x = u_xlat10.x + 1.0;
#ifdef UNITY_ADRENO_ES3
    u_xlat10.x = min(max(u_xlat10.x, 0.0), 1.0);
#else
    u_xlat10.x = clamp(u_xlat10.x, 0.0, 1.0);
#endif
    u_xlat10.x = (u_xlatb20) ? 0.0 : u_xlat10.x;
    u_xlat4 = u_xlat10.xxxx * u_xlat4 + u_xlat16_5;
    u_xlat4 = (-u_xlat1) + u_xlat4;
    u_xlat1 = u_xlat3.xxxx * u_xlat4 + u_xlat1;
#ifdef UNITY_ADRENO_ES3
    u_xlatb10 = !!(0.0<u_xlat3.x);
#else
    u_xlatb10 = 0.0<u_xlat3.x;
#endif
    u_xlatb0 = u_xlatb0 || u_xlatb10;
    u_xlat0 = (bool(u_xlatb0)) ? u_xlat1 : u_xlat2;
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
in highp vec4 in_POSITION0;
in mediump vec2 in_TEXCOORD0;
out highp vec2 vs_TEXCOORD0;
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
    return;
}

#endif
#ifdef FRAGMENT
#version 300 es

precision highp float;
precision highp int;
uniform 	vec4 _MainTex_TexelSize;
uniform 	mediump vec4 _MainTex_ST;
uniform mediump sampler2D _MainTex;
in highp vec2 vs_TEXCOORD0;
layout(location = 0) out mediump vec4 SV_Target0;
vec4 u_xlat0;
mediump vec4 u_xlat16_0;
bool u_xlatb0;
vec4 u_xlat1;
mediump vec4 u_xlat16_1;
vec4 u_xlat2;
vec4 u_xlat3;
mediump vec4 u_xlat16_3;
vec4 u_xlat4;
mediump vec4 u_xlat16_4;
vec4 u_xlat5;
mediump vec4 u_xlat16_5;
mediump vec4 u_xlat16_6;
vec4 u_xlat7;
mediump vec4 u_xlat16_7;
vec4 u_xlat8;
mediump vec4 u_xlat16_8;
vec4 u_xlat9;
vec3 u_xlat10;
bool u_xlatb10;
vec3 u_xlat13;
float u_xlat17;
float u_xlat20;
bool u_xlatb20;
float u_xlat23;
float u_xlat30;
void main()
{
    u_xlat0 = _MainTex_TexelSize.xyxy * vec4(0.0, -1.5, 0.0, 1.5) + vs_TEXCOORD0.xyxy;
    u_xlat0 = u_xlat0 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_1 = texture(_MainTex, u_xlat0.xy);
    u_xlat16_0 = texture(_MainTex, u_xlat0.zw);
    u_xlat2 = u_xlat16_0 + u_xlat16_1;
    u_xlat3.xyz = u_xlat2.xyz + u_xlat2.xyz;
    u_xlat4.xy = vs_TEXCOORD0.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_4 = texture(_MainTex, u_xlat4.xy);
    u_xlat3.xyz = (-u_xlat16_4.xyz) * vec3(4.0, 4.0, 4.0) + u_xlat3.xyz;
    u_xlat3.xyz = abs(u_xlat3.xyz) * vec3(0.25, 0.25, 0.25);
    u_xlat3.x = dot(u_xlat3.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat3.x = u_xlat3.x * 3.0 + -0.100000001;
    u_xlat5 = _MainTex_TexelSize.xyxy * vec4(-1.5, 0.0, 1.5, 0.0) + vs_TEXCOORD0.xyxy;
    u_xlat5 = u_xlat5 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_6 = texture(_MainTex, u_xlat5.xy);
    u_xlat16_5 = texture(_MainTex, u_xlat5.zw);
    u_xlat7 = u_xlat16_5 + u_xlat16_6;
    u_xlat8 = u_xlat16_4 + u_xlat16_4;
    u_xlat9 = u_xlat7 * vec4(2.0, 2.0, 2.0, 2.0) + u_xlat8;
    u_xlat13.xyz = u_xlat7.xyz + u_xlat7.xyz;
    u_xlat13.xyz = (-u_xlat16_4.xyz) * vec3(4.0, 4.0, 4.0) + u_xlat13.xyz;
    u_xlat13.xyz = abs(u_xlat13.xyz) * vec3(0.25, 0.25, 0.25);
    u_xlat13.x = dot(u_xlat13.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat13.x = u_xlat13.x * 3.0 + -0.100000001;
    u_xlat2 = u_xlat2 * vec4(2.0, 2.0, 2.0, 2.0) + u_xlat8;
    u_xlat7.xyz = u_xlat9.xyz * vec3(0.166666672, 0.166666672, 0.166666672);
    u_xlat8 = u_xlat9 * vec4(0.166666672, 0.166666672, 0.166666672, 0.166666672) + (-u_xlat16_4);
    u_xlat23 = dot(u_xlat7.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat3.x = u_xlat3.x / u_xlat23;
#ifdef UNITY_ADRENO_ES3
    u_xlat3.x = min(max(u_xlat3.x, 0.0), 1.0);
#else
    u_xlat3.x = clamp(u_xlat3.x, 0.0, 1.0);
#endif
    u_xlat7 = u_xlat3.xxxx * u_xlat8 + u_xlat16_4;
    u_xlat8 = u_xlat2 * vec4(0.166666672, 0.166666672, 0.166666672, 0.166666672) + (-u_xlat7);
    u_xlat2.xyz = u_xlat2.xyz * vec3(0.166666672, 0.166666672, 0.166666672);
    u_xlat2.x = dot(u_xlat2.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat2.x = u_xlat13.x / u_xlat2.x;
#ifdef UNITY_ADRENO_ES3
    u_xlat2.x = min(max(u_xlat2.x, 0.0), 1.0);
#else
    u_xlat2.x = clamp(u_xlat2.x, 0.0, 1.0);
#endif
    u_xlat2 = u_xlat2.xxxx * u_xlat8 + u_xlat7;
    u_xlat3 = _MainTex_TexelSize.xyxy * vec4(0.0, 3.5, 0.0, 5.5) + vs_TEXCOORD0.xyxy;
    u_xlat3 = u_xlat3 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_7 = texture(_MainTex, u_xlat3.xy);
    u_xlat16_3 = texture(_MainTex, u_xlat3.zw);
    u_xlat0 = u_xlat16_0.wxyz + u_xlat16_7.wxyz;
    u_xlat0 = u_xlat16_3.wxyz + u_xlat0;
    u_xlat3 = _MainTex_TexelSize.xyxy * vec4(0.0, 7.5, 0.0, -3.5) + vs_TEXCOORD0.xyxy;
    u_xlat3 = u_xlat3 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_7 = texture(_MainTex, u_xlat3.xy);
    u_xlat16_3 = texture(_MainTex, u_xlat3.zw);
    u_xlat0 = u_xlat0 + u_xlat16_7.wxyz;
    u_xlat0 = u_xlat16_1.wxyz + u_xlat0;
    u_xlat0 = u_xlat16_3.wxyz + u_xlat0;
    u_xlat1 = _MainTex_TexelSize.xyxy * vec4(0.0, -5.5, 0.0, -7.5) + vs_TEXCOORD0.xyxy;
    u_xlat1 = u_xlat1 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_3 = texture(_MainTex, u_xlat1.xy);
    u_xlat16_1 = texture(_MainTex, u_xlat1.zw);
    u_xlat0 = u_xlat0 + u_xlat16_3.wxyz;
    u_xlat0 = u_xlat16_1.wxyz + u_xlat0;
    u_xlat10.xyz = u_xlat0.yzw * vec3(0.125, 0.125, 0.125);
    u_xlat0.x = u_xlat0.x * 0.25 + -1.0;
#ifdef UNITY_ADRENO_ES3
    u_xlat0.x = min(max(u_xlat0.x, 0.0), 1.0);
#else
    u_xlat0.x = clamp(u_xlat0.x, 0.0, 1.0);
#endif
    u_xlat10.x = dot(u_xlat10.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat1 = _MainTex_TexelSize.xyxy * vec4(-1.0, 0.0, 1.0, 0.0) + vs_TEXCOORD0.xyxy;
    u_xlat1 = u_xlat1 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_3 = texture(_MainTex, u_xlat1.xy);
    u_xlat16_1 = texture(_MainTex, u_xlat1.zw);
    u_xlat20 = dot(u_xlat16_3.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat30 = (-u_xlat20) + u_xlat10.x;
    u_xlat7.x = dot(u_xlat16_4.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat17 = (-u_xlat20) + u_xlat7.x;
#ifdef UNITY_ADRENO_ES3
    u_xlatb20 = !!(u_xlat20==u_xlat7.x);
#else
    u_xlatb20 = u_xlat20==u_xlat7.x;
#endif
    u_xlat30 = u_xlat30 / u_xlat17;
#ifdef UNITY_ADRENO_ES3
    u_xlat30 = min(max(u_xlat30, 0.0), 1.0);
#else
    u_xlat30 = clamp(u_xlat30, 0.0, 1.0);
#endif
    u_xlat20 = (u_xlatb20) ? 0.0 : u_xlat30;
    u_xlat8 = (-u_xlat16_3) + u_xlat16_4;
    u_xlat3 = vec4(u_xlat20) * u_xlat8 + u_xlat16_3;
    u_xlat3 = (-u_xlat16_1) + u_xlat3;
    u_xlat10.x = u_xlat10.x + (-u_xlat7.x);
    u_xlat20 = dot(u_xlat16_1.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat30 = (-u_xlat20) + u_xlat7.x;
#ifdef UNITY_ADRENO_ES3
    u_xlatb20 = !!(u_xlat20==u_xlat7.x);
#else
    u_xlatb20 = u_xlat20==u_xlat7.x;
#endif
    u_xlat10.x = u_xlat10.x / u_xlat30;
    u_xlat10.x = u_xlat10.x + 1.0;
#ifdef UNITY_ADRENO_ES3
    u_xlat10.x = min(max(u_xlat10.x, 0.0), 1.0);
#else
    u_xlat10.x = clamp(u_xlat10.x, 0.0, 1.0);
#endif
    u_xlat10.x = (u_xlatb20) ? 0.0 : u_xlat10.x;
    u_xlat1 = u_xlat10.xxxx * u_xlat3 + u_xlat16_1;
    u_xlat1 = (-u_xlat2) + u_xlat1;
    u_xlat1 = u_xlat0.xxxx * u_xlat1 + u_xlat2;
#ifdef UNITY_ADRENO_ES3
    u_xlatb0 = !!(0.0<u_xlat0.x);
#else
    u_xlatb0 = 0.0<u_xlat0.x;
#endif
    u_xlat3 = _MainTex_TexelSize.xyxy * vec4(3.5, 0.0, 5.5, 0.0) + vs_TEXCOORD0.xyxy;
    u_xlat3 = u_xlat3 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_8 = texture(_MainTex, u_xlat3.xy);
    u_xlat16_3 = texture(_MainTex, u_xlat3.zw);
    u_xlat5 = u_xlat16_5.wxyz + u_xlat16_8.wxyz;
    u_xlat3 = u_xlat16_3.wxyz + u_xlat5;
    u_xlat5 = _MainTex_TexelSize.xyxy * vec4(7.5, 0.0, -3.5, 0.0) + vs_TEXCOORD0.xyxy;
    u_xlat5 = u_xlat5 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_8 = texture(_MainTex, u_xlat5.xy);
    u_xlat16_5 = texture(_MainTex, u_xlat5.zw);
    u_xlat3 = u_xlat3 + u_xlat16_8.wxyz;
    u_xlat3 = u_xlat16_6.wxyz + u_xlat3;
    u_xlat3 = u_xlat16_5.wxyz + u_xlat3;
    u_xlat5 = _MainTex_TexelSize.xyxy * vec4(-5.5, 0.0, -7.5, 0.0) + vs_TEXCOORD0.xyxy;
    u_xlat5 = u_xlat5 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_6 = texture(_MainTex, u_xlat5.xy);
    u_xlat16_5 = texture(_MainTex, u_xlat5.zw);
    u_xlat3 = u_xlat3 + u_xlat16_6.wxyz;
    u_xlat3 = u_xlat16_5.wxyz + u_xlat3;
    u_xlat10.xyz = u_xlat3.yzw * vec3(0.125, 0.125, 0.125);
    u_xlat3.x = u_xlat3.x * 0.25 + -1.0;
#ifdef UNITY_ADRENO_ES3
    u_xlat3.x = min(max(u_xlat3.x, 0.0), 1.0);
#else
    u_xlat3.x = clamp(u_xlat3.x, 0.0, 1.0);
#endif
    u_xlat10.x = dot(u_xlat10.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat5 = _MainTex_TexelSize.xyxy * vec4(0.0, -1.0, 0.0, 1.0) + vs_TEXCOORD0.xyxy;
    u_xlat5 = u_xlat5 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_6 = texture(_MainTex, u_xlat5.xy);
    u_xlat16_5 = texture(_MainTex, u_xlat5.zw);
    u_xlat20 = dot(u_xlat16_6.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat30 = (-u_xlat20) + u_xlat10.x;
    u_xlat10.x = (-u_xlat7.x) + u_xlat10.x;
    u_xlat13.x = (-u_xlat20) + u_xlat7.x;
#ifdef UNITY_ADRENO_ES3
    u_xlatb20 = !!(u_xlat20==u_xlat7.x);
#else
    u_xlatb20 = u_xlat20==u_xlat7.x;
#endif
    u_xlat30 = u_xlat30 / u_xlat13.x;
#ifdef UNITY_ADRENO_ES3
    u_xlat30 = min(max(u_xlat30, 0.0), 1.0);
#else
    u_xlat30 = clamp(u_xlat30, 0.0, 1.0);
#endif
    u_xlat20 = (u_xlatb20) ? 0.0 : u_xlat30;
    u_xlat4 = u_xlat16_4 + (-u_xlat16_6);
    u_xlat4 = vec4(u_xlat20) * u_xlat4 + u_xlat16_6;
    u_xlat4 = (-u_xlat16_5) + u_xlat4;
    u_xlat20 = dot(u_xlat16_5.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat30 = (-u_xlat20) + u_xlat7.x;
#ifdef UNITY_ADRENO_ES3
    u_xlatb20 = !!(u_xlat20==u_xlat7.x);
#else
    u_xlatb20 = u_xlat20==u_xlat7.x;
#endif
    u_xlat10.x = u_xlat10.x / u_xlat30;
    u_xlat10.x = u_xlat10.x + 1.0;
#ifdef UNITY_ADRENO_ES3
    u_xlat10.x = min(max(u_xlat10.x, 0.0), 1.0);
#else
    u_xlat10.x = clamp(u_xlat10.x, 0.0, 1.0);
#endif
    u_xlat10.x = (u_xlatb20) ? 0.0 : u_xlat10.x;
    u_xlat4 = u_xlat10.xxxx * u_xlat4 + u_xlat16_5;
    u_xlat4 = (-u_xlat1) + u_xlat4;
    u_xlat1 = u_xlat3.xxxx * u_xlat4 + u_xlat1;
#ifdef UNITY_ADRENO_ES3
    u_xlatb10 = !!(0.0<u_xlat3.x);
#else
    u_xlatb10 = 0.0<u_xlat3.x;
#endif
    u_xlatb0 = u_xlatb0 || u_xlatb10;
    u_xlat0 = (bool(u_xlatb0)) ? u_xlat1 : u_xlat2;
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
in highp vec4 in_POSITION0;
in mediump vec2 in_TEXCOORD0;
out highp vec2 vs_TEXCOORD0;
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
    return;
}

#endif
#ifdef FRAGMENT
#version 300 es

precision highp float;
precision highp int;
uniform 	vec4 _MainTex_TexelSize;
uniform 	mediump vec4 _MainTex_ST;
uniform mediump sampler2D _MainTex;
in highp vec2 vs_TEXCOORD0;
layout(location = 0) out mediump vec4 SV_Target0;
vec4 u_xlat0;
mediump vec4 u_xlat16_0;
bool u_xlatb0;
vec4 u_xlat1;
mediump vec4 u_xlat16_1;
vec4 u_xlat2;
vec4 u_xlat3;
mediump vec4 u_xlat16_3;
vec4 u_xlat4;
mediump vec4 u_xlat16_4;
vec4 u_xlat5;
mediump vec4 u_xlat16_5;
mediump vec4 u_xlat16_6;
vec4 u_xlat7;
mediump vec4 u_xlat16_7;
vec4 u_xlat8;
mediump vec4 u_xlat16_8;
vec4 u_xlat9;
vec3 u_xlat10;
bool u_xlatb10;
vec3 u_xlat13;
float u_xlat17;
float u_xlat20;
bool u_xlatb20;
float u_xlat23;
float u_xlat30;
void main()
{
    u_xlat0 = _MainTex_TexelSize.xyxy * vec4(0.0, -1.5, 0.0, 1.5) + vs_TEXCOORD0.xyxy;
    u_xlat0 = u_xlat0 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_1 = texture(_MainTex, u_xlat0.xy);
    u_xlat16_0 = texture(_MainTex, u_xlat0.zw);
    u_xlat2 = u_xlat16_0 + u_xlat16_1;
    u_xlat3.xyz = u_xlat2.xyz + u_xlat2.xyz;
    u_xlat4.xy = vs_TEXCOORD0.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_4 = texture(_MainTex, u_xlat4.xy);
    u_xlat3.xyz = (-u_xlat16_4.xyz) * vec3(4.0, 4.0, 4.0) + u_xlat3.xyz;
    u_xlat3.xyz = abs(u_xlat3.xyz) * vec3(0.25, 0.25, 0.25);
    u_xlat3.x = dot(u_xlat3.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat3.x = u_xlat3.x * 3.0 + -0.100000001;
    u_xlat5 = _MainTex_TexelSize.xyxy * vec4(-1.5, 0.0, 1.5, 0.0) + vs_TEXCOORD0.xyxy;
    u_xlat5 = u_xlat5 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_6 = texture(_MainTex, u_xlat5.xy);
    u_xlat16_5 = texture(_MainTex, u_xlat5.zw);
    u_xlat7 = u_xlat16_5 + u_xlat16_6;
    u_xlat8 = u_xlat16_4 + u_xlat16_4;
    u_xlat9 = u_xlat7 * vec4(2.0, 2.0, 2.0, 2.0) + u_xlat8;
    u_xlat13.xyz = u_xlat7.xyz + u_xlat7.xyz;
    u_xlat13.xyz = (-u_xlat16_4.xyz) * vec3(4.0, 4.0, 4.0) + u_xlat13.xyz;
    u_xlat13.xyz = abs(u_xlat13.xyz) * vec3(0.25, 0.25, 0.25);
    u_xlat13.x = dot(u_xlat13.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat13.x = u_xlat13.x * 3.0 + -0.100000001;
    u_xlat2 = u_xlat2 * vec4(2.0, 2.0, 2.0, 2.0) + u_xlat8;
    u_xlat7.xyz = u_xlat9.xyz * vec3(0.166666672, 0.166666672, 0.166666672);
    u_xlat8 = u_xlat9 * vec4(0.166666672, 0.166666672, 0.166666672, 0.166666672) + (-u_xlat16_4);
    u_xlat23 = dot(u_xlat7.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat3.x = u_xlat3.x / u_xlat23;
#ifdef UNITY_ADRENO_ES3
    u_xlat3.x = min(max(u_xlat3.x, 0.0), 1.0);
#else
    u_xlat3.x = clamp(u_xlat3.x, 0.0, 1.0);
#endif
    u_xlat7 = u_xlat3.xxxx * u_xlat8 + u_xlat16_4;
    u_xlat8 = u_xlat2 * vec4(0.166666672, 0.166666672, 0.166666672, 0.166666672) + (-u_xlat7);
    u_xlat2.xyz = u_xlat2.xyz * vec3(0.166666672, 0.166666672, 0.166666672);
    u_xlat2.x = dot(u_xlat2.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat2.x = u_xlat13.x / u_xlat2.x;
#ifdef UNITY_ADRENO_ES3
    u_xlat2.x = min(max(u_xlat2.x, 0.0), 1.0);
#else
    u_xlat2.x = clamp(u_xlat2.x, 0.0, 1.0);
#endif
    u_xlat2 = u_xlat2.xxxx * u_xlat8 + u_xlat7;
    u_xlat3 = _MainTex_TexelSize.xyxy * vec4(0.0, 3.5, 0.0, 5.5) + vs_TEXCOORD0.xyxy;
    u_xlat3 = u_xlat3 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_7 = texture(_MainTex, u_xlat3.xy);
    u_xlat16_3 = texture(_MainTex, u_xlat3.zw);
    u_xlat0 = u_xlat16_0.wxyz + u_xlat16_7.wxyz;
    u_xlat0 = u_xlat16_3.wxyz + u_xlat0;
    u_xlat3 = _MainTex_TexelSize.xyxy * vec4(0.0, 7.5, 0.0, -3.5) + vs_TEXCOORD0.xyxy;
    u_xlat3 = u_xlat3 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_7 = texture(_MainTex, u_xlat3.xy);
    u_xlat16_3 = texture(_MainTex, u_xlat3.zw);
    u_xlat0 = u_xlat0 + u_xlat16_7.wxyz;
    u_xlat0 = u_xlat16_1.wxyz + u_xlat0;
    u_xlat0 = u_xlat16_3.wxyz + u_xlat0;
    u_xlat1 = _MainTex_TexelSize.xyxy * vec4(0.0, -5.5, 0.0, -7.5) + vs_TEXCOORD0.xyxy;
    u_xlat1 = u_xlat1 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_3 = texture(_MainTex, u_xlat1.xy);
    u_xlat16_1 = texture(_MainTex, u_xlat1.zw);
    u_xlat0 = u_xlat0 + u_xlat16_3.wxyz;
    u_xlat0 = u_xlat16_1.wxyz + u_xlat0;
    u_xlat10.xyz = u_xlat0.yzw * vec3(0.125, 0.125, 0.125);
    u_xlat0.x = u_xlat0.x * 0.25 + -1.0;
#ifdef UNITY_ADRENO_ES3
    u_xlat0.x = min(max(u_xlat0.x, 0.0), 1.0);
#else
    u_xlat0.x = clamp(u_xlat0.x, 0.0, 1.0);
#endif
    u_xlat10.x = dot(u_xlat10.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat1 = _MainTex_TexelSize.xyxy * vec4(-1.0, 0.0, 1.0, 0.0) + vs_TEXCOORD0.xyxy;
    u_xlat1 = u_xlat1 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_3 = texture(_MainTex, u_xlat1.xy);
    u_xlat16_1 = texture(_MainTex, u_xlat1.zw);
    u_xlat20 = dot(u_xlat16_3.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat30 = (-u_xlat20) + u_xlat10.x;
    u_xlat7.x = dot(u_xlat16_4.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat17 = (-u_xlat20) + u_xlat7.x;
#ifdef UNITY_ADRENO_ES3
    u_xlatb20 = !!(u_xlat20==u_xlat7.x);
#else
    u_xlatb20 = u_xlat20==u_xlat7.x;
#endif
    u_xlat30 = u_xlat30 / u_xlat17;
#ifdef UNITY_ADRENO_ES3
    u_xlat30 = min(max(u_xlat30, 0.0), 1.0);
#else
    u_xlat30 = clamp(u_xlat30, 0.0, 1.0);
#endif
    u_xlat20 = (u_xlatb20) ? 0.0 : u_xlat30;
    u_xlat8 = (-u_xlat16_3) + u_xlat16_4;
    u_xlat3 = vec4(u_xlat20) * u_xlat8 + u_xlat16_3;
    u_xlat3 = (-u_xlat16_1) + u_xlat3;
    u_xlat10.x = u_xlat10.x + (-u_xlat7.x);
    u_xlat20 = dot(u_xlat16_1.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat30 = (-u_xlat20) + u_xlat7.x;
#ifdef UNITY_ADRENO_ES3
    u_xlatb20 = !!(u_xlat20==u_xlat7.x);
#else
    u_xlatb20 = u_xlat20==u_xlat7.x;
#endif
    u_xlat10.x = u_xlat10.x / u_xlat30;
    u_xlat10.x = u_xlat10.x + 1.0;
#ifdef UNITY_ADRENO_ES3
    u_xlat10.x = min(max(u_xlat10.x, 0.0), 1.0);
#else
    u_xlat10.x = clamp(u_xlat10.x, 0.0, 1.0);
#endif
    u_xlat10.x = (u_xlatb20) ? 0.0 : u_xlat10.x;
    u_xlat1 = u_xlat10.xxxx * u_xlat3 + u_xlat16_1;
    u_xlat1 = (-u_xlat2) + u_xlat1;
    u_xlat1 = u_xlat0.xxxx * u_xlat1 + u_xlat2;
#ifdef UNITY_ADRENO_ES3
    u_xlatb0 = !!(0.0<u_xlat0.x);
#else
    u_xlatb0 = 0.0<u_xlat0.x;
#endif
    u_xlat3 = _MainTex_TexelSize.xyxy * vec4(3.5, 0.0, 5.5, 0.0) + vs_TEXCOORD0.xyxy;
    u_xlat3 = u_xlat3 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_8 = texture(_MainTex, u_xlat3.xy);
    u_xlat16_3 = texture(_MainTex, u_xlat3.zw);
    u_xlat5 = u_xlat16_5.wxyz + u_xlat16_8.wxyz;
    u_xlat3 = u_xlat16_3.wxyz + u_xlat5;
    u_xlat5 = _MainTex_TexelSize.xyxy * vec4(7.5, 0.0, -3.5, 0.0) + vs_TEXCOORD0.xyxy;
    u_xlat5 = u_xlat5 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_8 = texture(_MainTex, u_xlat5.xy);
    u_xlat16_5 = texture(_MainTex, u_xlat5.zw);
    u_xlat3 = u_xlat3 + u_xlat16_8.wxyz;
    u_xlat3 = u_xlat16_6.wxyz + u_xlat3;
    u_xlat3 = u_xlat16_5.wxyz + u_xlat3;
    u_xlat5 = _MainTex_TexelSize.xyxy * vec4(-5.5, 0.0, -7.5, 0.0) + vs_TEXCOORD0.xyxy;
    u_xlat5 = u_xlat5 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_6 = texture(_MainTex, u_xlat5.xy);
    u_xlat16_5 = texture(_MainTex, u_xlat5.zw);
    u_xlat3 = u_xlat3 + u_xlat16_6.wxyz;
    u_xlat3 = u_xlat16_5.wxyz + u_xlat3;
    u_xlat10.xyz = u_xlat3.yzw * vec3(0.125, 0.125, 0.125);
    u_xlat3.x = u_xlat3.x * 0.25 + -1.0;
#ifdef UNITY_ADRENO_ES3
    u_xlat3.x = min(max(u_xlat3.x, 0.0), 1.0);
#else
    u_xlat3.x = clamp(u_xlat3.x, 0.0, 1.0);
#endif
    u_xlat10.x = dot(u_xlat10.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat5 = _MainTex_TexelSize.xyxy * vec4(0.0, -1.0, 0.0, 1.0) + vs_TEXCOORD0.xyxy;
    u_xlat5 = u_xlat5 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_6 = texture(_MainTex, u_xlat5.xy);
    u_xlat16_5 = texture(_MainTex, u_xlat5.zw);
    u_xlat20 = dot(u_xlat16_6.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat30 = (-u_xlat20) + u_xlat10.x;
    u_xlat10.x = (-u_xlat7.x) + u_xlat10.x;
    u_xlat13.x = (-u_xlat20) + u_xlat7.x;
#ifdef UNITY_ADRENO_ES3
    u_xlatb20 = !!(u_xlat20==u_xlat7.x);
#else
    u_xlatb20 = u_xlat20==u_xlat7.x;
#endif
    u_xlat30 = u_xlat30 / u_xlat13.x;
#ifdef UNITY_ADRENO_ES3
    u_xlat30 = min(max(u_xlat30, 0.0), 1.0);
#else
    u_xlat30 = clamp(u_xlat30, 0.0, 1.0);
#endif
    u_xlat20 = (u_xlatb20) ? 0.0 : u_xlat30;
    u_xlat4 = u_xlat16_4 + (-u_xlat16_6);
    u_xlat4 = vec4(u_xlat20) * u_xlat4 + u_xlat16_6;
    u_xlat4 = (-u_xlat16_5) + u_xlat4;
    u_xlat20 = dot(u_xlat16_5.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat30 = (-u_xlat20) + u_xlat7.x;
#ifdef UNITY_ADRENO_ES3
    u_xlatb20 = !!(u_xlat20==u_xlat7.x);
#else
    u_xlatb20 = u_xlat20==u_xlat7.x;
#endif
    u_xlat10.x = u_xlat10.x / u_xlat30;
    u_xlat10.x = u_xlat10.x + 1.0;
#ifdef UNITY_ADRENO_ES3
    u_xlat10.x = min(max(u_xlat10.x, 0.0), 1.0);
#else
    u_xlat10.x = clamp(u_xlat10.x, 0.0, 1.0);
#endif
    u_xlat10.x = (u_xlatb20) ? 0.0 : u_xlat10.x;
    u_xlat4 = u_xlat10.xxxx * u_xlat4 + u_xlat16_5;
    u_xlat4 = (-u_xlat1) + u_xlat4;
    u_xlat1 = u_xlat3.xxxx * u_xlat4 + u_xlat1;
#ifdef UNITY_ADRENO_ES3
    u_xlatb10 = !!(0.0<u_xlat3.x);
#else
    u_xlatb10 = 0.0<u_xlat3.x;
#endif
    u_xlatb0 = u_xlatb0 || u_xlatb10;
    u_xlat0 = (bool(u_xlatb0)) ? u_xlat1 : u_xlat2;
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
  GpuProgramID 190330
Program "vp" {
SubProgram "gles hw_tier00 " {
"#ifdef VERTEX
#version 100

uniform 	vec4 hlslcc_mtx4x4unity_ObjectToWorld[4];
uniform 	vec4 hlslcc_mtx4x4unity_MatrixVP[4];
attribute highp vec4 in_POSITION0;
attribute mediump vec2 in_TEXCOORD0;
varying highp vec2 vs_TEXCOORD0;
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
uniform lowp sampler2D _MainTex;
varying highp vec2 vs_TEXCOORD0;
#define SV_Target0 gl_FragData[0]
vec4 u_xlat0;
lowp vec4 u_xlat10_0;
bool u_xlatb0;
vec4 u_xlat1;
lowp vec4 u_xlat10_1;
vec4 u_xlat2;
lowp vec4 u_xlat10_2;
vec4 u_xlat3;
lowp vec4 u_xlat10_3;
vec4 u_xlat4;
lowp vec4 u_xlat10_4;
float u_xlat5;
vec4 u_xlat6;
lowp vec4 u_xlat10_6;
vec4 u_xlat7;
lowp vec4 u_xlat10_7;
vec4 u_xlat8;
lowp vec4 u_xlat10_8;
lowp vec4 u_xlat10_9;
lowp vec4 u_xlat10_10;
vec3 u_xlat11;
vec3 u_xlat16;
float u_xlat22;
bool u_xlatb22;
float u_xlat27;
float u_xlat33;
bool u_xlatb33;
void main()
{
    u_xlat0 = _MainTex_TexelSize.xyxy * vec4(0.0, 3.5, 0.0, 5.5) + vs_TEXCOORD0.xyxy;
    u_xlat0 = u_xlat0 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_1 = texture2D(_MainTex, u_xlat0.zw);
    u_xlat10_0 = texture2D(_MainTex, u_xlat0.xy);
    u_xlat2 = _MainTex_TexelSize.xyxy * vec4(0.0, -1.5, 0.0, 1.5) + vs_TEXCOORD0.xyxy;
    u_xlat2 = u_xlat2 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_3 = texture2D(_MainTex, u_xlat2.zw);
    u_xlat10_2 = texture2D(_MainTex, u_xlat2.xy);
    u_xlat0 = u_xlat10_0.wxyz + u_xlat10_3.wxyz;
    u_xlat3 = u_xlat10_3 + u_xlat10_2;
    u_xlat0 = u_xlat10_1.wxyz + u_xlat0;
    u_xlat1 = _MainTex_TexelSize.xyxy * vec4(0.0, 7.5, 0.0, -3.5) + vs_TEXCOORD0.xyxy;
    u_xlat1 = u_xlat1 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_4 = texture2D(_MainTex, u_xlat1.xy);
    u_xlat10_1 = texture2D(_MainTex, u_xlat1.zw);
    u_xlat0 = u_xlat0 + u_xlat10_4.wxyz;
    u_xlat0 = u_xlat10_2.wxyz + u_xlat0;
    u_xlat0 = u_xlat10_1.wxyz + u_xlat0;
    u_xlat1 = _MainTex_TexelSize.xyxy * vec4(0.0, -5.5, 0.0, -7.5) + vs_TEXCOORD0.xyxy;
    u_xlat1 = u_xlat1 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_2 = texture2D(_MainTex, u_xlat1.xy);
    u_xlat10_1 = texture2D(_MainTex, u_xlat1.zw);
    u_xlat0 = u_xlat0 + u_xlat10_2.wxyz;
    u_xlat0 = u_xlat10_1.wxyz + u_xlat0;
    u_xlat0.x = u_xlat0.x * 0.25 + -1.0;
    u_xlat0.x = clamp(u_xlat0.x, 0.0, 1.0);
    u_xlat11.xyz = u_xlat0.yzw * vec3(0.125, 0.125, 0.125);
    u_xlat11.x = dot(u_xlat11.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat1 = _MainTex_TexelSize.xyxy * vec4(-1.0, 0.0, 1.0, 0.0) + vs_TEXCOORD0.xyxy;
    u_xlat1 = u_xlat1 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_2 = texture2D(_MainTex, u_xlat1.xy);
    u_xlat10_1 = texture2D(_MainTex, u_xlat1.zw);
    u_xlat22 = dot(u_xlat10_2.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat33 = (-u_xlat22) + u_xlat11.x;
    u_xlat4.xy = vs_TEXCOORD0.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_4 = texture2D(_MainTex, u_xlat4.xy);
    u_xlat5 = dot(u_xlat10_4.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat16.x = (-u_xlat22) + u_xlat5;
    u_xlatb22 = u_xlat22==u_xlat5;
    u_xlat33 = u_xlat33 / u_xlat16.x;
    u_xlat33 = clamp(u_xlat33, 0.0, 1.0);
    u_xlat22 = (u_xlatb22) ? 0.0 : u_xlat33;
    u_xlat6 = (-u_xlat10_2) + u_xlat10_4;
    u_xlat6 = vec4(u_xlat22) * u_xlat6 + u_xlat10_2;
    u_xlat2.xyz = u_xlat10_1.xyz + u_xlat10_2.xyz;
    u_xlat2.xyz = (-u_xlat10_4.xyz) * vec3(2.0, 2.0, 2.0) + u_xlat2.xyz;
    u_xlat2.xyz = abs(u_xlat2.xyz) * vec3(0.5, 0.5, 0.5);
    u_xlat22 = dot(u_xlat2.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat22 = u_xlat22 * 3.0 + -0.100000001;
    u_xlat2 = (-u_xlat10_1) + u_xlat6;
    u_xlat11.x = u_xlat11.x + (-u_xlat5);
    u_xlat33 = dot(u_xlat10_1.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat16.x = (-u_xlat33) + u_xlat5;
    u_xlatb33 = u_xlat33==u_xlat5;
    u_xlat11.x = u_xlat11.x / u_xlat16.x;
    u_xlat11.x = u_xlat11.x + 1.0;
    u_xlat11.x = clamp(u_xlat11.x, 0.0, 1.0);
    u_xlat11.x = (u_xlatb33) ? 0.0 : u_xlat11.x;
    u_xlat1 = u_xlat11.xxxx * u_xlat2 + u_xlat10_1;
    u_xlat2 = u_xlat10_4 + u_xlat10_4;
    u_xlat3 = u_xlat3 * vec4(2.0, 2.0, 2.0, 2.0) + u_xlat2;
    u_xlat16.xyz = u_xlat3.xyz * vec3(0.166666672, 0.166666672, 0.166666672);
    u_xlat11.x = dot(u_xlat16.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat11.x = u_xlat22 / u_xlat11.x;
    u_xlat11.x = clamp(u_xlat11.x, 0.0, 1.0);
    u_xlat11.x = u_xlat11.x * 0.5;
    u_xlat6 = _MainTex_TexelSize.xyxy * vec4(-1.5, 0.0, 1.5, 0.0) + vs_TEXCOORD0.xyxy;
    u_xlat6 = u_xlat6 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_7 = texture2D(_MainTex, u_xlat6.xy);
    u_xlat10_6 = texture2D(_MainTex, u_xlat6.zw);
    u_xlat8 = u_xlat10_6 + u_xlat10_7;
    u_xlat2 = u_xlat8 * vec4(2.0, 2.0, 2.0, 2.0) + u_xlat2;
    u_xlat8 = u_xlat2 * vec4(0.166666672, 0.166666672, 0.166666672, 0.166666672) + (-u_xlat10_4);
    u_xlat2.xyz = u_xlat2.xyz * vec3(0.166666672, 0.166666672, 0.166666672);
    u_xlat22 = dot(u_xlat2.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat2 = _MainTex_TexelSize.xyxy * vec4(0.0, -1.0, 0.0, 1.0) + vs_TEXCOORD0.xyxy;
    u_xlat2 = u_xlat2 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_9 = texture2D(_MainTex, u_xlat2.xy);
    u_xlat10_2 = texture2D(_MainTex, u_xlat2.zw);
    u_xlat16.xyz = u_xlat10_2.xyz + u_xlat10_9.xyz;
    u_xlat16.xyz = (-u_xlat10_4.xyz) * vec3(2.0, 2.0, 2.0) + u_xlat16.xyz;
    u_xlat16.xyz = abs(u_xlat16.xyz) * vec3(0.5, 0.5, 0.5);
    u_xlat33 = dot(u_xlat16.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat33 = u_xlat33 * 3.0 + -0.100000001;
    u_xlat22 = u_xlat33 / u_xlat22;
    u_xlat22 = clamp(u_xlat22, 0.0, 1.0);
    u_xlat8 = vec4(u_xlat22) * u_xlat8 + u_xlat10_4;
    u_xlat4 = u_xlat10_4 + (-u_xlat10_9);
    u_xlat3 = u_xlat3 * vec4(0.166666672, 0.166666672, 0.166666672, 0.166666672) + (-u_xlat8);
    u_xlat3 = u_xlat11.xxxx * u_xlat3 + u_xlat8;
    u_xlat1 = u_xlat1 + (-u_xlat3);
    u_xlat1 = u_xlat0.xxxx * u_xlat1 + u_xlat3;
    u_xlat8 = _MainTex_TexelSize.xyxy * vec4(3.5, 0.0, 5.5, 0.0) + vs_TEXCOORD0.xyxy;
    u_xlat8 = u_xlat8 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_10 = texture2D(_MainTex, u_xlat8.xy);
    u_xlat10_8 = texture2D(_MainTex, u_xlat8.zw);
    u_xlat6 = u_xlat10_6.wxyz + u_xlat10_10.wxyz;
    u_xlat6 = u_xlat10_8.wxyz + u_xlat6;
    u_xlat8 = _MainTex_TexelSize.xyxy * vec4(7.5, 0.0, -3.5, 0.0) + vs_TEXCOORD0.xyxy;
    u_xlat8 = u_xlat8 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_10 = texture2D(_MainTex, u_xlat8.xy);
    u_xlat10_8 = texture2D(_MainTex, u_xlat8.zw);
    u_xlat6 = u_xlat6 + u_xlat10_10.wxyz;
    u_xlat6 = u_xlat10_7.wxyz + u_xlat6;
    u_xlat6 = u_xlat10_8.wxyz + u_xlat6;
    u_xlat7 = _MainTex_TexelSize.xyxy * vec4(-5.5, 0.0, -7.5, 0.0) + vs_TEXCOORD0.xyxy;
    u_xlat7 = u_xlat7 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_8 = texture2D(_MainTex, u_xlat7.xy);
    u_xlat10_7 = texture2D(_MainTex, u_xlat7.zw);
    u_xlat6 = u_xlat6 + u_xlat10_8.wxyz;
    u_xlat6 = u_xlat10_7.wxyz + u_xlat6;
    u_xlat11.xyz = u_xlat6.yzw * vec3(0.125, 0.125, 0.125);
    u_xlat16.x = u_xlat6.x * 0.25 + -1.0;
    u_xlat16.x = clamp(u_xlat16.x, 0.0, 1.0);
    u_xlat11.x = dot(u_xlat11.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat22 = dot(u_xlat10_9.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat33 = (-u_xlat22) + u_xlat11.x;
    u_xlat11.x = (-u_xlat5) + u_xlat11.x;
    u_xlat27 = (-u_xlat22) + u_xlat5;
    u_xlatb22 = u_xlat22==u_xlat5;
    u_xlat33 = u_xlat33 / u_xlat27;
    u_xlat33 = clamp(u_xlat33, 0.0, 1.0);
    u_xlat22 = (u_xlatb22) ? 0.0 : u_xlat33;
    u_xlat4 = vec4(u_xlat22) * u_xlat4 + u_xlat10_9;
    u_xlat4 = (-u_xlat10_2) + u_xlat4;
    u_xlat22 = dot(u_xlat10_2.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat33 = (-u_xlat22) + u_xlat5;
    u_xlatb22 = u_xlat22==u_xlat5;
    u_xlat11.x = u_xlat11.x / u_xlat33;
    u_xlat11.x = u_xlat11.x + 1.0;
    u_xlat11.x = clamp(u_xlat11.x, 0.0, 1.0);
    u_xlat11.x = (u_xlatb22) ? 0.0 : u_xlat11.x;
    u_xlat2 = u_xlat11.xxxx * u_xlat4 + u_xlat10_2;
    u_xlat2 = (-u_xlat1) + u_xlat2;
    u_xlat1 = u_xlat16.xxxx * u_xlat2 + u_xlat1;
    u_xlat0.x = (-u_xlat0.x) + u_xlat16.x;
    u_xlatb0 = 0.200000003<abs(u_xlat0.x);
    u_xlat0 = (bool(u_xlatb0)) ? u_xlat1 : u_xlat3;
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
attribute highp vec4 in_POSITION0;
attribute mediump vec2 in_TEXCOORD0;
varying highp vec2 vs_TEXCOORD0;
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
uniform lowp sampler2D _MainTex;
varying highp vec2 vs_TEXCOORD0;
#define SV_Target0 gl_FragData[0]
vec4 u_xlat0;
lowp vec4 u_xlat10_0;
bool u_xlatb0;
vec4 u_xlat1;
lowp vec4 u_xlat10_1;
vec4 u_xlat2;
lowp vec4 u_xlat10_2;
vec4 u_xlat3;
lowp vec4 u_xlat10_3;
vec4 u_xlat4;
lowp vec4 u_xlat10_4;
float u_xlat5;
vec4 u_xlat6;
lowp vec4 u_xlat10_6;
vec4 u_xlat7;
lowp vec4 u_xlat10_7;
vec4 u_xlat8;
lowp vec4 u_xlat10_8;
lowp vec4 u_xlat10_9;
lowp vec4 u_xlat10_10;
vec3 u_xlat11;
vec3 u_xlat16;
float u_xlat22;
bool u_xlatb22;
float u_xlat27;
float u_xlat33;
bool u_xlatb33;
void main()
{
    u_xlat0 = _MainTex_TexelSize.xyxy * vec4(0.0, 3.5, 0.0, 5.5) + vs_TEXCOORD0.xyxy;
    u_xlat0 = u_xlat0 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_1 = texture2D(_MainTex, u_xlat0.zw);
    u_xlat10_0 = texture2D(_MainTex, u_xlat0.xy);
    u_xlat2 = _MainTex_TexelSize.xyxy * vec4(0.0, -1.5, 0.0, 1.5) + vs_TEXCOORD0.xyxy;
    u_xlat2 = u_xlat2 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_3 = texture2D(_MainTex, u_xlat2.zw);
    u_xlat10_2 = texture2D(_MainTex, u_xlat2.xy);
    u_xlat0 = u_xlat10_0.wxyz + u_xlat10_3.wxyz;
    u_xlat3 = u_xlat10_3 + u_xlat10_2;
    u_xlat0 = u_xlat10_1.wxyz + u_xlat0;
    u_xlat1 = _MainTex_TexelSize.xyxy * vec4(0.0, 7.5, 0.0, -3.5) + vs_TEXCOORD0.xyxy;
    u_xlat1 = u_xlat1 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_4 = texture2D(_MainTex, u_xlat1.xy);
    u_xlat10_1 = texture2D(_MainTex, u_xlat1.zw);
    u_xlat0 = u_xlat0 + u_xlat10_4.wxyz;
    u_xlat0 = u_xlat10_2.wxyz + u_xlat0;
    u_xlat0 = u_xlat10_1.wxyz + u_xlat0;
    u_xlat1 = _MainTex_TexelSize.xyxy * vec4(0.0, -5.5, 0.0, -7.5) + vs_TEXCOORD0.xyxy;
    u_xlat1 = u_xlat1 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_2 = texture2D(_MainTex, u_xlat1.xy);
    u_xlat10_1 = texture2D(_MainTex, u_xlat1.zw);
    u_xlat0 = u_xlat0 + u_xlat10_2.wxyz;
    u_xlat0 = u_xlat10_1.wxyz + u_xlat0;
    u_xlat0.x = u_xlat0.x * 0.25 + -1.0;
    u_xlat0.x = clamp(u_xlat0.x, 0.0, 1.0);
    u_xlat11.xyz = u_xlat0.yzw * vec3(0.125, 0.125, 0.125);
    u_xlat11.x = dot(u_xlat11.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat1 = _MainTex_TexelSize.xyxy * vec4(-1.0, 0.0, 1.0, 0.0) + vs_TEXCOORD0.xyxy;
    u_xlat1 = u_xlat1 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_2 = texture2D(_MainTex, u_xlat1.xy);
    u_xlat10_1 = texture2D(_MainTex, u_xlat1.zw);
    u_xlat22 = dot(u_xlat10_2.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat33 = (-u_xlat22) + u_xlat11.x;
    u_xlat4.xy = vs_TEXCOORD0.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_4 = texture2D(_MainTex, u_xlat4.xy);
    u_xlat5 = dot(u_xlat10_4.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat16.x = (-u_xlat22) + u_xlat5;
    u_xlatb22 = u_xlat22==u_xlat5;
    u_xlat33 = u_xlat33 / u_xlat16.x;
    u_xlat33 = clamp(u_xlat33, 0.0, 1.0);
    u_xlat22 = (u_xlatb22) ? 0.0 : u_xlat33;
    u_xlat6 = (-u_xlat10_2) + u_xlat10_4;
    u_xlat6 = vec4(u_xlat22) * u_xlat6 + u_xlat10_2;
    u_xlat2.xyz = u_xlat10_1.xyz + u_xlat10_2.xyz;
    u_xlat2.xyz = (-u_xlat10_4.xyz) * vec3(2.0, 2.0, 2.0) + u_xlat2.xyz;
    u_xlat2.xyz = abs(u_xlat2.xyz) * vec3(0.5, 0.5, 0.5);
    u_xlat22 = dot(u_xlat2.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat22 = u_xlat22 * 3.0 + -0.100000001;
    u_xlat2 = (-u_xlat10_1) + u_xlat6;
    u_xlat11.x = u_xlat11.x + (-u_xlat5);
    u_xlat33 = dot(u_xlat10_1.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat16.x = (-u_xlat33) + u_xlat5;
    u_xlatb33 = u_xlat33==u_xlat5;
    u_xlat11.x = u_xlat11.x / u_xlat16.x;
    u_xlat11.x = u_xlat11.x + 1.0;
    u_xlat11.x = clamp(u_xlat11.x, 0.0, 1.0);
    u_xlat11.x = (u_xlatb33) ? 0.0 : u_xlat11.x;
    u_xlat1 = u_xlat11.xxxx * u_xlat2 + u_xlat10_1;
    u_xlat2 = u_xlat10_4 + u_xlat10_4;
    u_xlat3 = u_xlat3 * vec4(2.0, 2.0, 2.0, 2.0) + u_xlat2;
    u_xlat16.xyz = u_xlat3.xyz * vec3(0.166666672, 0.166666672, 0.166666672);
    u_xlat11.x = dot(u_xlat16.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat11.x = u_xlat22 / u_xlat11.x;
    u_xlat11.x = clamp(u_xlat11.x, 0.0, 1.0);
    u_xlat11.x = u_xlat11.x * 0.5;
    u_xlat6 = _MainTex_TexelSize.xyxy * vec4(-1.5, 0.0, 1.5, 0.0) + vs_TEXCOORD0.xyxy;
    u_xlat6 = u_xlat6 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_7 = texture2D(_MainTex, u_xlat6.xy);
    u_xlat10_6 = texture2D(_MainTex, u_xlat6.zw);
    u_xlat8 = u_xlat10_6 + u_xlat10_7;
    u_xlat2 = u_xlat8 * vec4(2.0, 2.0, 2.0, 2.0) + u_xlat2;
    u_xlat8 = u_xlat2 * vec4(0.166666672, 0.166666672, 0.166666672, 0.166666672) + (-u_xlat10_4);
    u_xlat2.xyz = u_xlat2.xyz * vec3(0.166666672, 0.166666672, 0.166666672);
    u_xlat22 = dot(u_xlat2.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat2 = _MainTex_TexelSize.xyxy * vec4(0.0, -1.0, 0.0, 1.0) + vs_TEXCOORD0.xyxy;
    u_xlat2 = u_xlat2 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_9 = texture2D(_MainTex, u_xlat2.xy);
    u_xlat10_2 = texture2D(_MainTex, u_xlat2.zw);
    u_xlat16.xyz = u_xlat10_2.xyz + u_xlat10_9.xyz;
    u_xlat16.xyz = (-u_xlat10_4.xyz) * vec3(2.0, 2.0, 2.0) + u_xlat16.xyz;
    u_xlat16.xyz = abs(u_xlat16.xyz) * vec3(0.5, 0.5, 0.5);
    u_xlat33 = dot(u_xlat16.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat33 = u_xlat33 * 3.0 + -0.100000001;
    u_xlat22 = u_xlat33 / u_xlat22;
    u_xlat22 = clamp(u_xlat22, 0.0, 1.0);
    u_xlat8 = vec4(u_xlat22) * u_xlat8 + u_xlat10_4;
    u_xlat4 = u_xlat10_4 + (-u_xlat10_9);
    u_xlat3 = u_xlat3 * vec4(0.166666672, 0.166666672, 0.166666672, 0.166666672) + (-u_xlat8);
    u_xlat3 = u_xlat11.xxxx * u_xlat3 + u_xlat8;
    u_xlat1 = u_xlat1 + (-u_xlat3);
    u_xlat1 = u_xlat0.xxxx * u_xlat1 + u_xlat3;
    u_xlat8 = _MainTex_TexelSize.xyxy * vec4(3.5, 0.0, 5.5, 0.0) + vs_TEXCOORD0.xyxy;
    u_xlat8 = u_xlat8 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_10 = texture2D(_MainTex, u_xlat8.xy);
    u_xlat10_8 = texture2D(_MainTex, u_xlat8.zw);
    u_xlat6 = u_xlat10_6.wxyz + u_xlat10_10.wxyz;
    u_xlat6 = u_xlat10_8.wxyz + u_xlat6;
    u_xlat8 = _MainTex_TexelSize.xyxy * vec4(7.5, 0.0, -3.5, 0.0) + vs_TEXCOORD0.xyxy;
    u_xlat8 = u_xlat8 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_10 = texture2D(_MainTex, u_xlat8.xy);
    u_xlat10_8 = texture2D(_MainTex, u_xlat8.zw);
    u_xlat6 = u_xlat6 + u_xlat10_10.wxyz;
    u_xlat6 = u_xlat10_7.wxyz + u_xlat6;
    u_xlat6 = u_xlat10_8.wxyz + u_xlat6;
    u_xlat7 = _MainTex_TexelSize.xyxy * vec4(-5.5, 0.0, -7.5, 0.0) + vs_TEXCOORD0.xyxy;
    u_xlat7 = u_xlat7 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_8 = texture2D(_MainTex, u_xlat7.xy);
    u_xlat10_7 = texture2D(_MainTex, u_xlat7.zw);
    u_xlat6 = u_xlat6 + u_xlat10_8.wxyz;
    u_xlat6 = u_xlat10_7.wxyz + u_xlat6;
    u_xlat11.xyz = u_xlat6.yzw * vec3(0.125, 0.125, 0.125);
    u_xlat16.x = u_xlat6.x * 0.25 + -1.0;
    u_xlat16.x = clamp(u_xlat16.x, 0.0, 1.0);
    u_xlat11.x = dot(u_xlat11.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat22 = dot(u_xlat10_9.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat33 = (-u_xlat22) + u_xlat11.x;
    u_xlat11.x = (-u_xlat5) + u_xlat11.x;
    u_xlat27 = (-u_xlat22) + u_xlat5;
    u_xlatb22 = u_xlat22==u_xlat5;
    u_xlat33 = u_xlat33 / u_xlat27;
    u_xlat33 = clamp(u_xlat33, 0.0, 1.0);
    u_xlat22 = (u_xlatb22) ? 0.0 : u_xlat33;
    u_xlat4 = vec4(u_xlat22) * u_xlat4 + u_xlat10_9;
    u_xlat4 = (-u_xlat10_2) + u_xlat4;
    u_xlat22 = dot(u_xlat10_2.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat33 = (-u_xlat22) + u_xlat5;
    u_xlatb22 = u_xlat22==u_xlat5;
    u_xlat11.x = u_xlat11.x / u_xlat33;
    u_xlat11.x = u_xlat11.x + 1.0;
    u_xlat11.x = clamp(u_xlat11.x, 0.0, 1.0);
    u_xlat11.x = (u_xlatb22) ? 0.0 : u_xlat11.x;
    u_xlat2 = u_xlat11.xxxx * u_xlat4 + u_xlat10_2;
    u_xlat2 = (-u_xlat1) + u_xlat2;
    u_xlat1 = u_xlat16.xxxx * u_xlat2 + u_xlat1;
    u_xlat0.x = (-u_xlat0.x) + u_xlat16.x;
    u_xlatb0 = 0.200000003<abs(u_xlat0.x);
    u_xlat0 = (bool(u_xlatb0)) ? u_xlat1 : u_xlat3;
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
attribute highp vec4 in_POSITION0;
attribute mediump vec2 in_TEXCOORD0;
varying highp vec2 vs_TEXCOORD0;
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
uniform lowp sampler2D _MainTex;
varying highp vec2 vs_TEXCOORD0;
#define SV_Target0 gl_FragData[0]
vec4 u_xlat0;
lowp vec4 u_xlat10_0;
bool u_xlatb0;
vec4 u_xlat1;
lowp vec4 u_xlat10_1;
vec4 u_xlat2;
lowp vec4 u_xlat10_2;
vec4 u_xlat3;
lowp vec4 u_xlat10_3;
vec4 u_xlat4;
lowp vec4 u_xlat10_4;
float u_xlat5;
vec4 u_xlat6;
lowp vec4 u_xlat10_6;
vec4 u_xlat7;
lowp vec4 u_xlat10_7;
vec4 u_xlat8;
lowp vec4 u_xlat10_8;
lowp vec4 u_xlat10_9;
lowp vec4 u_xlat10_10;
vec3 u_xlat11;
vec3 u_xlat16;
float u_xlat22;
bool u_xlatb22;
float u_xlat27;
float u_xlat33;
bool u_xlatb33;
void main()
{
    u_xlat0 = _MainTex_TexelSize.xyxy * vec4(0.0, 3.5, 0.0, 5.5) + vs_TEXCOORD0.xyxy;
    u_xlat0 = u_xlat0 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_1 = texture2D(_MainTex, u_xlat0.zw);
    u_xlat10_0 = texture2D(_MainTex, u_xlat0.xy);
    u_xlat2 = _MainTex_TexelSize.xyxy * vec4(0.0, -1.5, 0.0, 1.5) + vs_TEXCOORD0.xyxy;
    u_xlat2 = u_xlat2 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_3 = texture2D(_MainTex, u_xlat2.zw);
    u_xlat10_2 = texture2D(_MainTex, u_xlat2.xy);
    u_xlat0 = u_xlat10_0.wxyz + u_xlat10_3.wxyz;
    u_xlat3 = u_xlat10_3 + u_xlat10_2;
    u_xlat0 = u_xlat10_1.wxyz + u_xlat0;
    u_xlat1 = _MainTex_TexelSize.xyxy * vec4(0.0, 7.5, 0.0, -3.5) + vs_TEXCOORD0.xyxy;
    u_xlat1 = u_xlat1 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_4 = texture2D(_MainTex, u_xlat1.xy);
    u_xlat10_1 = texture2D(_MainTex, u_xlat1.zw);
    u_xlat0 = u_xlat0 + u_xlat10_4.wxyz;
    u_xlat0 = u_xlat10_2.wxyz + u_xlat0;
    u_xlat0 = u_xlat10_1.wxyz + u_xlat0;
    u_xlat1 = _MainTex_TexelSize.xyxy * vec4(0.0, -5.5, 0.0, -7.5) + vs_TEXCOORD0.xyxy;
    u_xlat1 = u_xlat1 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_2 = texture2D(_MainTex, u_xlat1.xy);
    u_xlat10_1 = texture2D(_MainTex, u_xlat1.zw);
    u_xlat0 = u_xlat0 + u_xlat10_2.wxyz;
    u_xlat0 = u_xlat10_1.wxyz + u_xlat0;
    u_xlat0.x = u_xlat0.x * 0.25 + -1.0;
    u_xlat0.x = clamp(u_xlat0.x, 0.0, 1.0);
    u_xlat11.xyz = u_xlat0.yzw * vec3(0.125, 0.125, 0.125);
    u_xlat11.x = dot(u_xlat11.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat1 = _MainTex_TexelSize.xyxy * vec4(-1.0, 0.0, 1.0, 0.0) + vs_TEXCOORD0.xyxy;
    u_xlat1 = u_xlat1 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_2 = texture2D(_MainTex, u_xlat1.xy);
    u_xlat10_1 = texture2D(_MainTex, u_xlat1.zw);
    u_xlat22 = dot(u_xlat10_2.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat33 = (-u_xlat22) + u_xlat11.x;
    u_xlat4.xy = vs_TEXCOORD0.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_4 = texture2D(_MainTex, u_xlat4.xy);
    u_xlat5 = dot(u_xlat10_4.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat16.x = (-u_xlat22) + u_xlat5;
    u_xlatb22 = u_xlat22==u_xlat5;
    u_xlat33 = u_xlat33 / u_xlat16.x;
    u_xlat33 = clamp(u_xlat33, 0.0, 1.0);
    u_xlat22 = (u_xlatb22) ? 0.0 : u_xlat33;
    u_xlat6 = (-u_xlat10_2) + u_xlat10_4;
    u_xlat6 = vec4(u_xlat22) * u_xlat6 + u_xlat10_2;
    u_xlat2.xyz = u_xlat10_1.xyz + u_xlat10_2.xyz;
    u_xlat2.xyz = (-u_xlat10_4.xyz) * vec3(2.0, 2.0, 2.0) + u_xlat2.xyz;
    u_xlat2.xyz = abs(u_xlat2.xyz) * vec3(0.5, 0.5, 0.5);
    u_xlat22 = dot(u_xlat2.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat22 = u_xlat22 * 3.0 + -0.100000001;
    u_xlat2 = (-u_xlat10_1) + u_xlat6;
    u_xlat11.x = u_xlat11.x + (-u_xlat5);
    u_xlat33 = dot(u_xlat10_1.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat16.x = (-u_xlat33) + u_xlat5;
    u_xlatb33 = u_xlat33==u_xlat5;
    u_xlat11.x = u_xlat11.x / u_xlat16.x;
    u_xlat11.x = u_xlat11.x + 1.0;
    u_xlat11.x = clamp(u_xlat11.x, 0.0, 1.0);
    u_xlat11.x = (u_xlatb33) ? 0.0 : u_xlat11.x;
    u_xlat1 = u_xlat11.xxxx * u_xlat2 + u_xlat10_1;
    u_xlat2 = u_xlat10_4 + u_xlat10_4;
    u_xlat3 = u_xlat3 * vec4(2.0, 2.0, 2.0, 2.0) + u_xlat2;
    u_xlat16.xyz = u_xlat3.xyz * vec3(0.166666672, 0.166666672, 0.166666672);
    u_xlat11.x = dot(u_xlat16.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat11.x = u_xlat22 / u_xlat11.x;
    u_xlat11.x = clamp(u_xlat11.x, 0.0, 1.0);
    u_xlat11.x = u_xlat11.x * 0.5;
    u_xlat6 = _MainTex_TexelSize.xyxy * vec4(-1.5, 0.0, 1.5, 0.0) + vs_TEXCOORD0.xyxy;
    u_xlat6 = u_xlat6 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_7 = texture2D(_MainTex, u_xlat6.xy);
    u_xlat10_6 = texture2D(_MainTex, u_xlat6.zw);
    u_xlat8 = u_xlat10_6 + u_xlat10_7;
    u_xlat2 = u_xlat8 * vec4(2.0, 2.0, 2.0, 2.0) + u_xlat2;
    u_xlat8 = u_xlat2 * vec4(0.166666672, 0.166666672, 0.166666672, 0.166666672) + (-u_xlat10_4);
    u_xlat2.xyz = u_xlat2.xyz * vec3(0.166666672, 0.166666672, 0.166666672);
    u_xlat22 = dot(u_xlat2.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat2 = _MainTex_TexelSize.xyxy * vec4(0.0, -1.0, 0.0, 1.0) + vs_TEXCOORD0.xyxy;
    u_xlat2 = u_xlat2 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_9 = texture2D(_MainTex, u_xlat2.xy);
    u_xlat10_2 = texture2D(_MainTex, u_xlat2.zw);
    u_xlat16.xyz = u_xlat10_2.xyz + u_xlat10_9.xyz;
    u_xlat16.xyz = (-u_xlat10_4.xyz) * vec3(2.0, 2.0, 2.0) + u_xlat16.xyz;
    u_xlat16.xyz = abs(u_xlat16.xyz) * vec3(0.5, 0.5, 0.5);
    u_xlat33 = dot(u_xlat16.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat33 = u_xlat33 * 3.0 + -0.100000001;
    u_xlat22 = u_xlat33 / u_xlat22;
    u_xlat22 = clamp(u_xlat22, 0.0, 1.0);
    u_xlat8 = vec4(u_xlat22) * u_xlat8 + u_xlat10_4;
    u_xlat4 = u_xlat10_4 + (-u_xlat10_9);
    u_xlat3 = u_xlat3 * vec4(0.166666672, 0.166666672, 0.166666672, 0.166666672) + (-u_xlat8);
    u_xlat3 = u_xlat11.xxxx * u_xlat3 + u_xlat8;
    u_xlat1 = u_xlat1 + (-u_xlat3);
    u_xlat1 = u_xlat0.xxxx * u_xlat1 + u_xlat3;
    u_xlat8 = _MainTex_TexelSize.xyxy * vec4(3.5, 0.0, 5.5, 0.0) + vs_TEXCOORD0.xyxy;
    u_xlat8 = u_xlat8 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_10 = texture2D(_MainTex, u_xlat8.xy);
    u_xlat10_8 = texture2D(_MainTex, u_xlat8.zw);
    u_xlat6 = u_xlat10_6.wxyz + u_xlat10_10.wxyz;
    u_xlat6 = u_xlat10_8.wxyz + u_xlat6;
    u_xlat8 = _MainTex_TexelSize.xyxy * vec4(7.5, 0.0, -3.5, 0.0) + vs_TEXCOORD0.xyxy;
    u_xlat8 = u_xlat8 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_10 = texture2D(_MainTex, u_xlat8.xy);
    u_xlat10_8 = texture2D(_MainTex, u_xlat8.zw);
    u_xlat6 = u_xlat6 + u_xlat10_10.wxyz;
    u_xlat6 = u_xlat10_7.wxyz + u_xlat6;
    u_xlat6 = u_xlat10_8.wxyz + u_xlat6;
    u_xlat7 = _MainTex_TexelSize.xyxy * vec4(-5.5, 0.0, -7.5, 0.0) + vs_TEXCOORD0.xyxy;
    u_xlat7 = u_xlat7 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat10_8 = texture2D(_MainTex, u_xlat7.xy);
    u_xlat10_7 = texture2D(_MainTex, u_xlat7.zw);
    u_xlat6 = u_xlat6 + u_xlat10_8.wxyz;
    u_xlat6 = u_xlat10_7.wxyz + u_xlat6;
    u_xlat11.xyz = u_xlat6.yzw * vec3(0.125, 0.125, 0.125);
    u_xlat16.x = u_xlat6.x * 0.25 + -1.0;
    u_xlat16.x = clamp(u_xlat16.x, 0.0, 1.0);
    u_xlat11.x = dot(u_xlat11.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat22 = dot(u_xlat10_9.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat33 = (-u_xlat22) + u_xlat11.x;
    u_xlat11.x = (-u_xlat5) + u_xlat11.x;
    u_xlat27 = (-u_xlat22) + u_xlat5;
    u_xlatb22 = u_xlat22==u_xlat5;
    u_xlat33 = u_xlat33 / u_xlat27;
    u_xlat33 = clamp(u_xlat33, 0.0, 1.0);
    u_xlat22 = (u_xlatb22) ? 0.0 : u_xlat33;
    u_xlat4 = vec4(u_xlat22) * u_xlat4 + u_xlat10_9;
    u_xlat4 = (-u_xlat10_2) + u_xlat4;
    u_xlat22 = dot(u_xlat10_2.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat33 = (-u_xlat22) + u_xlat5;
    u_xlatb22 = u_xlat22==u_xlat5;
    u_xlat11.x = u_xlat11.x / u_xlat33;
    u_xlat11.x = u_xlat11.x + 1.0;
    u_xlat11.x = clamp(u_xlat11.x, 0.0, 1.0);
    u_xlat11.x = (u_xlatb22) ? 0.0 : u_xlat11.x;
    u_xlat2 = u_xlat11.xxxx * u_xlat4 + u_xlat10_2;
    u_xlat2 = (-u_xlat1) + u_xlat2;
    u_xlat1 = u_xlat16.xxxx * u_xlat2 + u_xlat1;
    u_xlat0.x = (-u_xlat0.x) + u_xlat16.x;
    u_xlatb0 = 0.200000003<abs(u_xlat0.x);
    u_xlat0 = (bool(u_xlatb0)) ? u_xlat1 : u_xlat3;
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
in highp vec4 in_POSITION0;
in mediump vec2 in_TEXCOORD0;
out highp vec2 vs_TEXCOORD0;
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
    return;
}

#endif
#ifdef FRAGMENT
#version 300 es

precision highp float;
precision highp int;
uniform 	vec4 _MainTex_TexelSize;
uniform 	mediump vec4 _MainTex_ST;
uniform mediump sampler2D _MainTex;
in highp vec2 vs_TEXCOORD0;
layout(location = 0) out mediump vec4 SV_Target0;
vec4 u_xlat0;
mediump vec4 u_xlat16_0;
bool u_xlatb0;
vec4 u_xlat1;
mediump vec4 u_xlat16_1;
vec4 u_xlat2;
mediump vec4 u_xlat16_2;
vec4 u_xlat3;
mediump vec4 u_xlat16_3;
vec4 u_xlat4;
mediump vec4 u_xlat16_4;
float u_xlat5;
vec4 u_xlat6;
mediump vec4 u_xlat16_6;
vec4 u_xlat7;
mediump vec4 u_xlat16_7;
vec4 u_xlat8;
mediump vec4 u_xlat16_8;
mediump vec4 u_xlat16_9;
mediump vec4 u_xlat16_10;
vec3 u_xlat11;
vec3 u_xlat16;
float u_xlat22;
bool u_xlatb22;
float u_xlat27;
float u_xlat33;
bool u_xlatb33;
void main()
{
    u_xlat0 = _MainTex_TexelSize.xyxy * vec4(0.0, 3.5, 0.0, 5.5) + vs_TEXCOORD0.xyxy;
    u_xlat0 = u_xlat0 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_1 = texture(_MainTex, u_xlat0.zw);
    u_xlat16_0 = texture(_MainTex, u_xlat0.xy);
    u_xlat2 = _MainTex_TexelSize.xyxy * vec4(0.0, -1.5, 0.0, 1.5) + vs_TEXCOORD0.xyxy;
    u_xlat2 = u_xlat2 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_3 = texture(_MainTex, u_xlat2.zw);
    u_xlat16_2 = texture(_MainTex, u_xlat2.xy);
    u_xlat0 = u_xlat16_0.wxyz + u_xlat16_3.wxyz;
    u_xlat3 = u_xlat16_3 + u_xlat16_2;
    u_xlat0 = u_xlat16_1.wxyz + u_xlat0;
    u_xlat1 = _MainTex_TexelSize.xyxy * vec4(0.0, 7.5, 0.0, -3.5) + vs_TEXCOORD0.xyxy;
    u_xlat1 = u_xlat1 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_4 = texture(_MainTex, u_xlat1.xy);
    u_xlat16_1 = texture(_MainTex, u_xlat1.zw);
    u_xlat0 = u_xlat0 + u_xlat16_4.wxyz;
    u_xlat0 = u_xlat16_2.wxyz + u_xlat0;
    u_xlat0 = u_xlat16_1.wxyz + u_xlat0;
    u_xlat1 = _MainTex_TexelSize.xyxy * vec4(0.0, -5.5, 0.0, -7.5) + vs_TEXCOORD0.xyxy;
    u_xlat1 = u_xlat1 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_2 = texture(_MainTex, u_xlat1.xy);
    u_xlat16_1 = texture(_MainTex, u_xlat1.zw);
    u_xlat0 = u_xlat0 + u_xlat16_2.wxyz;
    u_xlat0 = u_xlat16_1.wxyz + u_xlat0;
    u_xlat0.x = u_xlat0.x * 0.25 + -1.0;
#ifdef UNITY_ADRENO_ES3
    u_xlat0.x = min(max(u_xlat0.x, 0.0), 1.0);
#else
    u_xlat0.x = clamp(u_xlat0.x, 0.0, 1.0);
#endif
    u_xlat11.xyz = u_xlat0.yzw * vec3(0.125, 0.125, 0.125);
    u_xlat11.x = dot(u_xlat11.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat1 = _MainTex_TexelSize.xyxy * vec4(-1.0, 0.0, 1.0, 0.0) + vs_TEXCOORD0.xyxy;
    u_xlat1 = u_xlat1 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_2 = texture(_MainTex, u_xlat1.xy);
    u_xlat16_1 = texture(_MainTex, u_xlat1.zw);
    u_xlat22 = dot(u_xlat16_2.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat33 = (-u_xlat22) + u_xlat11.x;
    u_xlat4.xy = vs_TEXCOORD0.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_4 = texture(_MainTex, u_xlat4.xy);
    u_xlat5 = dot(u_xlat16_4.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat16.x = (-u_xlat22) + u_xlat5;
#ifdef UNITY_ADRENO_ES3
    u_xlatb22 = !!(u_xlat22==u_xlat5);
#else
    u_xlatb22 = u_xlat22==u_xlat5;
#endif
    u_xlat33 = u_xlat33 / u_xlat16.x;
#ifdef UNITY_ADRENO_ES3
    u_xlat33 = min(max(u_xlat33, 0.0), 1.0);
#else
    u_xlat33 = clamp(u_xlat33, 0.0, 1.0);
#endif
    u_xlat22 = (u_xlatb22) ? 0.0 : u_xlat33;
    u_xlat6 = (-u_xlat16_2) + u_xlat16_4;
    u_xlat6 = vec4(u_xlat22) * u_xlat6 + u_xlat16_2;
    u_xlat2.xyz = u_xlat16_1.xyz + u_xlat16_2.xyz;
    u_xlat2.xyz = (-u_xlat16_4.xyz) * vec3(2.0, 2.0, 2.0) + u_xlat2.xyz;
    u_xlat2.xyz = abs(u_xlat2.xyz) * vec3(0.5, 0.5, 0.5);
    u_xlat22 = dot(u_xlat2.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat22 = u_xlat22 * 3.0 + -0.100000001;
    u_xlat2 = (-u_xlat16_1) + u_xlat6;
    u_xlat11.x = u_xlat11.x + (-u_xlat5);
    u_xlat33 = dot(u_xlat16_1.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat16.x = (-u_xlat33) + u_xlat5;
#ifdef UNITY_ADRENO_ES3
    u_xlatb33 = !!(u_xlat33==u_xlat5);
#else
    u_xlatb33 = u_xlat33==u_xlat5;
#endif
    u_xlat11.x = u_xlat11.x / u_xlat16.x;
    u_xlat11.x = u_xlat11.x + 1.0;
#ifdef UNITY_ADRENO_ES3
    u_xlat11.x = min(max(u_xlat11.x, 0.0), 1.0);
#else
    u_xlat11.x = clamp(u_xlat11.x, 0.0, 1.0);
#endif
    u_xlat11.x = (u_xlatb33) ? 0.0 : u_xlat11.x;
    u_xlat1 = u_xlat11.xxxx * u_xlat2 + u_xlat16_1;
    u_xlat2 = u_xlat16_4 + u_xlat16_4;
    u_xlat3 = u_xlat3 * vec4(2.0, 2.0, 2.0, 2.0) + u_xlat2;
    u_xlat16.xyz = u_xlat3.xyz * vec3(0.166666672, 0.166666672, 0.166666672);
    u_xlat11.x = dot(u_xlat16.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat11.x = u_xlat22 / u_xlat11.x;
#ifdef UNITY_ADRENO_ES3
    u_xlat11.x = min(max(u_xlat11.x, 0.0), 1.0);
#else
    u_xlat11.x = clamp(u_xlat11.x, 0.0, 1.0);
#endif
    u_xlat11.x = u_xlat11.x * 0.5;
    u_xlat6 = _MainTex_TexelSize.xyxy * vec4(-1.5, 0.0, 1.5, 0.0) + vs_TEXCOORD0.xyxy;
    u_xlat6 = u_xlat6 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_7 = texture(_MainTex, u_xlat6.xy);
    u_xlat16_6 = texture(_MainTex, u_xlat6.zw);
    u_xlat8 = u_xlat16_6 + u_xlat16_7;
    u_xlat2 = u_xlat8 * vec4(2.0, 2.0, 2.0, 2.0) + u_xlat2;
    u_xlat8 = u_xlat2 * vec4(0.166666672, 0.166666672, 0.166666672, 0.166666672) + (-u_xlat16_4);
    u_xlat2.xyz = u_xlat2.xyz * vec3(0.166666672, 0.166666672, 0.166666672);
    u_xlat22 = dot(u_xlat2.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat2 = _MainTex_TexelSize.xyxy * vec4(0.0, -1.0, 0.0, 1.0) + vs_TEXCOORD0.xyxy;
    u_xlat2 = u_xlat2 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_9 = texture(_MainTex, u_xlat2.xy);
    u_xlat16_2 = texture(_MainTex, u_xlat2.zw);
    u_xlat16.xyz = u_xlat16_2.xyz + u_xlat16_9.xyz;
    u_xlat16.xyz = (-u_xlat16_4.xyz) * vec3(2.0, 2.0, 2.0) + u_xlat16.xyz;
    u_xlat16.xyz = abs(u_xlat16.xyz) * vec3(0.5, 0.5, 0.5);
    u_xlat33 = dot(u_xlat16.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat33 = u_xlat33 * 3.0 + -0.100000001;
    u_xlat22 = u_xlat33 / u_xlat22;
#ifdef UNITY_ADRENO_ES3
    u_xlat22 = min(max(u_xlat22, 0.0), 1.0);
#else
    u_xlat22 = clamp(u_xlat22, 0.0, 1.0);
#endif
    u_xlat8 = vec4(u_xlat22) * u_xlat8 + u_xlat16_4;
    u_xlat4 = u_xlat16_4 + (-u_xlat16_9);
    u_xlat3 = u_xlat3 * vec4(0.166666672, 0.166666672, 0.166666672, 0.166666672) + (-u_xlat8);
    u_xlat3 = u_xlat11.xxxx * u_xlat3 + u_xlat8;
    u_xlat1 = u_xlat1 + (-u_xlat3);
    u_xlat1 = u_xlat0.xxxx * u_xlat1 + u_xlat3;
    u_xlat8 = _MainTex_TexelSize.xyxy * vec4(3.5, 0.0, 5.5, 0.0) + vs_TEXCOORD0.xyxy;
    u_xlat8 = u_xlat8 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_10 = texture(_MainTex, u_xlat8.xy);
    u_xlat16_8 = texture(_MainTex, u_xlat8.zw);
    u_xlat6 = u_xlat16_6.wxyz + u_xlat16_10.wxyz;
    u_xlat6 = u_xlat16_8.wxyz + u_xlat6;
    u_xlat8 = _MainTex_TexelSize.xyxy * vec4(7.5, 0.0, -3.5, 0.0) + vs_TEXCOORD0.xyxy;
    u_xlat8 = u_xlat8 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_10 = texture(_MainTex, u_xlat8.xy);
    u_xlat16_8 = texture(_MainTex, u_xlat8.zw);
    u_xlat6 = u_xlat6 + u_xlat16_10.wxyz;
    u_xlat6 = u_xlat16_7.wxyz + u_xlat6;
    u_xlat6 = u_xlat16_8.wxyz + u_xlat6;
    u_xlat7 = _MainTex_TexelSize.xyxy * vec4(-5.5, 0.0, -7.5, 0.0) + vs_TEXCOORD0.xyxy;
    u_xlat7 = u_xlat7 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_8 = texture(_MainTex, u_xlat7.xy);
    u_xlat16_7 = texture(_MainTex, u_xlat7.zw);
    u_xlat6 = u_xlat6 + u_xlat16_8.wxyz;
    u_xlat6 = u_xlat16_7.wxyz + u_xlat6;
    u_xlat11.xyz = u_xlat6.yzw * vec3(0.125, 0.125, 0.125);
    u_xlat16.x = u_xlat6.x * 0.25 + -1.0;
#ifdef UNITY_ADRENO_ES3
    u_xlat16.x = min(max(u_xlat16.x, 0.0), 1.0);
#else
    u_xlat16.x = clamp(u_xlat16.x, 0.0, 1.0);
#endif
    u_xlat11.x = dot(u_xlat11.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat22 = dot(u_xlat16_9.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat33 = (-u_xlat22) + u_xlat11.x;
    u_xlat11.x = (-u_xlat5) + u_xlat11.x;
    u_xlat27 = (-u_xlat22) + u_xlat5;
#ifdef UNITY_ADRENO_ES3
    u_xlatb22 = !!(u_xlat22==u_xlat5);
#else
    u_xlatb22 = u_xlat22==u_xlat5;
#endif
    u_xlat33 = u_xlat33 / u_xlat27;
#ifdef UNITY_ADRENO_ES3
    u_xlat33 = min(max(u_xlat33, 0.0), 1.0);
#else
    u_xlat33 = clamp(u_xlat33, 0.0, 1.0);
#endif
    u_xlat22 = (u_xlatb22) ? 0.0 : u_xlat33;
    u_xlat4 = vec4(u_xlat22) * u_xlat4 + u_xlat16_9;
    u_xlat4 = (-u_xlat16_2) + u_xlat4;
    u_xlat22 = dot(u_xlat16_2.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat33 = (-u_xlat22) + u_xlat5;
#ifdef UNITY_ADRENO_ES3
    u_xlatb22 = !!(u_xlat22==u_xlat5);
#else
    u_xlatb22 = u_xlat22==u_xlat5;
#endif
    u_xlat11.x = u_xlat11.x / u_xlat33;
    u_xlat11.x = u_xlat11.x + 1.0;
#ifdef UNITY_ADRENO_ES3
    u_xlat11.x = min(max(u_xlat11.x, 0.0), 1.0);
#else
    u_xlat11.x = clamp(u_xlat11.x, 0.0, 1.0);
#endif
    u_xlat11.x = (u_xlatb22) ? 0.0 : u_xlat11.x;
    u_xlat2 = u_xlat11.xxxx * u_xlat4 + u_xlat16_2;
    u_xlat2 = (-u_xlat1) + u_xlat2;
    u_xlat1 = u_xlat16.xxxx * u_xlat2 + u_xlat1;
    u_xlat0.x = (-u_xlat0.x) + u_xlat16.x;
#ifdef UNITY_ADRENO_ES3
    u_xlatb0 = !!(0.200000003<abs(u_xlat0.x));
#else
    u_xlatb0 = 0.200000003<abs(u_xlat0.x);
#endif
    u_xlat0 = (bool(u_xlatb0)) ? u_xlat1 : u_xlat3;
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
in highp vec4 in_POSITION0;
in mediump vec2 in_TEXCOORD0;
out highp vec2 vs_TEXCOORD0;
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
    return;
}

#endif
#ifdef FRAGMENT
#version 300 es

precision highp float;
precision highp int;
uniform 	vec4 _MainTex_TexelSize;
uniform 	mediump vec4 _MainTex_ST;
uniform mediump sampler2D _MainTex;
in highp vec2 vs_TEXCOORD0;
layout(location = 0) out mediump vec4 SV_Target0;
vec4 u_xlat0;
mediump vec4 u_xlat16_0;
bool u_xlatb0;
vec4 u_xlat1;
mediump vec4 u_xlat16_1;
vec4 u_xlat2;
mediump vec4 u_xlat16_2;
vec4 u_xlat3;
mediump vec4 u_xlat16_3;
vec4 u_xlat4;
mediump vec4 u_xlat16_4;
float u_xlat5;
vec4 u_xlat6;
mediump vec4 u_xlat16_6;
vec4 u_xlat7;
mediump vec4 u_xlat16_7;
vec4 u_xlat8;
mediump vec4 u_xlat16_8;
mediump vec4 u_xlat16_9;
mediump vec4 u_xlat16_10;
vec3 u_xlat11;
vec3 u_xlat16;
float u_xlat22;
bool u_xlatb22;
float u_xlat27;
float u_xlat33;
bool u_xlatb33;
void main()
{
    u_xlat0 = _MainTex_TexelSize.xyxy * vec4(0.0, 3.5, 0.0, 5.5) + vs_TEXCOORD0.xyxy;
    u_xlat0 = u_xlat0 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_1 = texture(_MainTex, u_xlat0.zw);
    u_xlat16_0 = texture(_MainTex, u_xlat0.xy);
    u_xlat2 = _MainTex_TexelSize.xyxy * vec4(0.0, -1.5, 0.0, 1.5) + vs_TEXCOORD0.xyxy;
    u_xlat2 = u_xlat2 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_3 = texture(_MainTex, u_xlat2.zw);
    u_xlat16_2 = texture(_MainTex, u_xlat2.xy);
    u_xlat0 = u_xlat16_0.wxyz + u_xlat16_3.wxyz;
    u_xlat3 = u_xlat16_3 + u_xlat16_2;
    u_xlat0 = u_xlat16_1.wxyz + u_xlat0;
    u_xlat1 = _MainTex_TexelSize.xyxy * vec4(0.0, 7.5, 0.0, -3.5) + vs_TEXCOORD0.xyxy;
    u_xlat1 = u_xlat1 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_4 = texture(_MainTex, u_xlat1.xy);
    u_xlat16_1 = texture(_MainTex, u_xlat1.zw);
    u_xlat0 = u_xlat0 + u_xlat16_4.wxyz;
    u_xlat0 = u_xlat16_2.wxyz + u_xlat0;
    u_xlat0 = u_xlat16_1.wxyz + u_xlat0;
    u_xlat1 = _MainTex_TexelSize.xyxy * vec4(0.0, -5.5, 0.0, -7.5) + vs_TEXCOORD0.xyxy;
    u_xlat1 = u_xlat1 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_2 = texture(_MainTex, u_xlat1.xy);
    u_xlat16_1 = texture(_MainTex, u_xlat1.zw);
    u_xlat0 = u_xlat0 + u_xlat16_2.wxyz;
    u_xlat0 = u_xlat16_1.wxyz + u_xlat0;
    u_xlat0.x = u_xlat0.x * 0.25 + -1.0;
#ifdef UNITY_ADRENO_ES3
    u_xlat0.x = min(max(u_xlat0.x, 0.0), 1.0);
#else
    u_xlat0.x = clamp(u_xlat0.x, 0.0, 1.0);
#endif
    u_xlat11.xyz = u_xlat0.yzw * vec3(0.125, 0.125, 0.125);
    u_xlat11.x = dot(u_xlat11.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat1 = _MainTex_TexelSize.xyxy * vec4(-1.0, 0.0, 1.0, 0.0) + vs_TEXCOORD0.xyxy;
    u_xlat1 = u_xlat1 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_2 = texture(_MainTex, u_xlat1.xy);
    u_xlat16_1 = texture(_MainTex, u_xlat1.zw);
    u_xlat22 = dot(u_xlat16_2.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat33 = (-u_xlat22) + u_xlat11.x;
    u_xlat4.xy = vs_TEXCOORD0.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_4 = texture(_MainTex, u_xlat4.xy);
    u_xlat5 = dot(u_xlat16_4.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat16.x = (-u_xlat22) + u_xlat5;
#ifdef UNITY_ADRENO_ES3
    u_xlatb22 = !!(u_xlat22==u_xlat5);
#else
    u_xlatb22 = u_xlat22==u_xlat5;
#endif
    u_xlat33 = u_xlat33 / u_xlat16.x;
#ifdef UNITY_ADRENO_ES3
    u_xlat33 = min(max(u_xlat33, 0.0), 1.0);
#else
    u_xlat33 = clamp(u_xlat33, 0.0, 1.0);
#endif
    u_xlat22 = (u_xlatb22) ? 0.0 : u_xlat33;
    u_xlat6 = (-u_xlat16_2) + u_xlat16_4;
    u_xlat6 = vec4(u_xlat22) * u_xlat6 + u_xlat16_2;
    u_xlat2.xyz = u_xlat16_1.xyz + u_xlat16_2.xyz;
    u_xlat2.xyz = (-u_xlat16_4.xyz) * vec3(2.0, 2.0, 2.0) + u_xlat2.xyz;
    u_xlat2.xyz = abs(u_xlat2.xyz) * vec3(0.5, 0.5, 0.5);
    u_xlat22 = dot(u_xlat2.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat22 = u_xlat22 * 3.0 + -0.100000001;
    u_xlat2 = (-u_xlat16_1) + u_xlat6;
    u_xlat11.x = u_xlat11.x + (-u_xlat5);
    u_xlat33 = dot(u_xlat16_1.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat16.x = (-u_xlat33) + u_xlat5;
#ifdef UNITY_ADRENO_ES3
    u_xlatb33 = !!(u_xlat33==u_xlat5);
#else
    u_xlatb33 = u_xlat33==u_xlat5;
#endif
    u_xlat11.x = u_xlat11.x / u_xlat16.x;
    u_xlat11.x = u_xlat11.x + 1.0;
#ifdef UNITY_ADRENO_ES3
    u_xlat11.x = min(max(u_xlat11.x, 0.0), 1.0);
#else
    u_xlat11.x = clamp(u_xlat11.x, 0.0, 1.0);
#endif
    u_xlat11.x = (u_xlatb33) ? 0.0 : u_xlat11.x;
    u_xlat1 = u_xlat11.xxxx * u_xlat2 + u_xlat16_1;
    u_xlat2 = u_xlat16_4 + u_xlat16_4;
    u_xlat3 = u_xlat3 * vec4(2.0, 2.0, 2.0, 2.0) + u_xlat2;
    u_xlat16.xyz = u_xlat3.xyz * vec3(0.166666672, 0.166666672, 0.166666672);
    u_xlat11.x = dot(u_xlat16.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat11.x = u_xlat22 / u_xlat11.x;
#ifdef UNITY_ADRENO_ES3
    u_xlat11.x = min(max(u_xlat11.x, 0.0), 1.0);
#else
    u_xlat11.x = clamp(u_xlat11.x, 0.0, 1.0);
#endif
    u_xlat11.x = u_xlat11.x * 0.5;
    u_xlat6 = _MainTex_TexelSize.xyxy * vec4(-1.5, 0.0, 1.5, 0.0) + vs_TEXCOORD0.xyxy;
    u_xlat6 = u_xlat6 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_7 = texture(_MainTex, u_xlat6.xy);
    u_xlat16_6 = texture(_MainTex, u_xlat6.zw);
    u_xlat8 = u_xlat16_6 + u_xlat16_7;
    u_xlat2 = u_xlat8 * vec4(2.0, 2.0, 2.0, 2.0) + u_xlat2;
    u_xlat8 = u_xlat2 * vec4(0.166666672, 0.166666672, 0.166666672, 0.166666672) + (-u_xlat16_4);
    u_xlat2.xyz = u_xlat2.xyz * vec3(0.166666672, 0.166666672, 0.166666672);
    u_xlat22 = dot(u_xlat2.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat2 = _MainTex_TexelSize.xyxy * vec4(0.0, -1.0, 0.0, 1.0) + vs_TEXCOORD0.xyxy;
    u_xlat2 = u_xlat2 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_9 = texture(_MainTex, u_xlat2.xy);
    u_xlat16_2 = texture(_MainTex, u_xlat2.zw);
    u_xlat16.xyz = u_xlat16_2.xyz + u_xlat16_9.xyz;
    u_xlat16.xyz = (-u_xlat16_4.xyz) * vec3(2.0, 2.0, 2.0) + u_xlat16.xyz;
    u_xlat16.xyz = abs(u_xlat16.xyz) * vec3(0.5, 0.5, 0.5);
    u_xlat33 = dot(u_xlat16.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat33 = u_xlat33 * 3.0 + -0.100000001;
    u_xlat22 = u_xlat33 / u_xlat22;
#ifdef UNITY_ADRENO_ES3
    u_xlat22 = min(max(u_xlat22, 0.0), 1.0);
#else
    u_xlat22 = clamp(u_xlat22, 0.0, 1.0);
#endif
    u_xlat8 = vec4(u_xlat22) * u_xlat8 + u_xlat16_4;
    u_xlat4 = u_xlat16_4 + (-u_xlat16_9);
    u_xlat3 = u_xlat3 * vec4(0.166666672, 0.166666672, 0.166666672, 0.166666672) + (-u_xlat8);
    u_xlat3 = u_xlat11.xxxx * u_xlat3 + u_xlat8;
    u_xlat1 = u_xlat1 + (-u_xlat3);
    u_xlat1 = u_xlat0.xxxx * u_xlat1 + u_xlat3;
    u_xlat8 = _MainTex_TexelSize.xyxy * vec4(3.5, 0.0, 5.5, 0.0) + vs_TEXCOORD0.xyxy;
    u_xlat8 = u_xlat8 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_10 = texture(_MainTex, u_xlat8.xy);
    u_xlat16_8 = texture(_MainTex, u_xlat8.zw);
    u_xlat6 = u_xlat16_6.wxyz + u_xlat16_10.wxyz;
    u_xlat6 = u_xlat16_8.wxyz + u_xlat6;
    u_xlat8 = _MainTex_TexelSize.xyxy * vec4(7.5, 0.0, -3.5, 0.0) + vs_TEXCOORD0.xyxy;
    u_xlat8 = u_xlat8 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_10 = texture(_MainTex, u_xlat8.xy);
    u_xlat16_8 = texture(_MainTex, u_xlat8.zw);
    u_xlat6 = u_xlat6 + u_xlat16_10.wxyz;
    u_xlat6 = u_xlat16_7.wxyz + u_xlat6;
    u_xlat6 = u_xlat16_8.wxyz + u_xlat6;
    u_xlat7 = _MainTex_TexelSize.xyxy * vec4(-5.5, 0.0, -7.5, 0.0) + vs_TEXCOORD0.xyxy;
    u_xlat7 = u_xlat7 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_8 = texture(_MainTex, u_xlat7.xy);
    u_xlat16_7 = texture(_MainTex, u_xlat7.zw);
    u_xlat6 = u_xlat6 + u_xlat16_8.wxyz;
    u_xlat6 = u_xlat16_7.wxyz + u_xlat6;
    u_xlat11.xyz = u_xlat6.yzw * vec3(0.125, 0.125, 0.125);
    u_xlat16.x = u_xlat6.x * 0.25 + -1.0;
#ifdef UNITY_ADRENO_ES3
    u_xlat16.x = min(max(u_xlat16.x, 0.0), 1.0);
#else
    u_xlat16.x = clamp(u_xlat16.x, 0.0, 1.0);
#endif
    u_xlat11.x = dot(u_xlat11.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat22 = dot(u_xlat16_9.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat33 = (-u_xlat22) + u_xlat11.x;
    u_xlat11.x = (-u_xlat5) + u_xlat11.x;
    u_xlat27 = (-u_xlat22) + u_xlat5;
#ifdef UNITY_ADRENO_ES3
    u_xlatb22 = !!(u_xlat22==u_xlat5);
#else
    u_xlatb22 = u_xlat22==u_xlat5;
#endif
    u_xlat33 = u_xlat33 / u_xlat27;
#ifdef UNITY_ADRENO_ES3
    u_xlat33 = min(max(u_xlat33, 0.0), 1.0);
#else
    u_xlat33 = clamp(u_xlat33, 0.0, 1.0);
#endif
    u_xlat22 = (u_xlatb22) ? 0.0 : u_xlat33;
    u_xlat4 = vec4(u_xlat22) * u_xlat4 + u_xlat16_9;
    u_xlat4 = (-u_xlat16_2) + u_xlat4;
    u_xlat22 = dot(u_xlat16_2.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat33 = (-u_xlat22) + u_xlat5;
#ifdef UNITY_ADRENO_ES3
    u_xlatb22 = !!(u_xlat22==u_xlat5);
#else
    u_xlatb22 = u_xlat22==u_xlat5;
#endif
    u_xlat11.x = u_xlat11.x / u_xlat33;
    u_xlat11.x = u_xlat11.x + 1.0;
#ifdef UNITY_ADRENO_ES3
    u_xlat11.x = min(max(u_xlat11.x, 0.0), 1.0);
#else
    u_xlat11.x = clamp(u_xlat11.x, 0.0, 1.0);
#endif
    u_xlat11.x = (u_xlatb22) ? 0.0 : u_xlat11.x;
    u_xlat2 = u_xlat11.xxxx * u_xlat4 + u_xlat16_2;
    u_xlat2 = (-u_xlat1) + u_xlat2;
    u_xlat1 = u_xlat16.xxxx * u_xlat2 + u_xlat1;
    u_xlat0.x = (-u_xlat0.x) + u_xlat16.x;
#ifdef UNITY_ADRENO_ES3
    u_xlatb0 = !!(0.200000003<abs(u_xlat0.x));
#else
    u_xlatb0 = 0.200000003<abs(u_xlat0.x);
#endif
    u_xlat0 = (bool(u_xlatb0)) ? u_xlat1 : u_xlat3;
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
in highp vec4 in_POSITION0;
in mediump vec2 in_TEXCOORD0;
out highp vec2 vs_TEXCOORD0;
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
    return;
}

#endif
#ifdef FRAGMENT
#version 300 es

precision highp float;
precision highp int;
uniform 	vec4 _MainTex_TexelSize;
uniform 	mediump vec4 _MainTex_ST;
uniform mediump sampler2D _MainTex;
in highp vec2 vs_TEXCOORD0;
layout(location = 0) out mediump vec4 SV_Target0;
vec4 u_xlat0;
mediump vec4 u_xlat16_0;
bool u_xlatb0;
vec4 u_xlat1;
mediump vec4 u_xlat16_1;
vec4 u_xlat2;
mediump vec4 u_xlat16_2;
vec4 u_xlat3;
mediump vec4 u_xlat16_3;
vec4 u_xlat4;
mediump vec4 u_xlat16_4;
float u_xlat5;
vec4 u_xlat6;
mediump vec4 u_xlat16_6;
vec4 u_xlat7;
mediump vec4 u_xlat16_7;
vec4 u_xlat8;
mediump vec4 u_xlat16_8;
mediump vec4 u_xlat16_9;
mediump vec4 u_xlat16_10;
vec3 u_xlat11;
vec3 u_xlat16;
float u_xlat22;
bool u_xlatb22;
float u_xlat27;
float u_xlat33;
bool u_xlatb33;
void main()
{
    u_xlat0 = _MainTex_TexelSize.xyxy * vec4(0.0, 3.5, 0.0, 5.5) + vs_TEXCOORD0.xyxy;
    u_xlat0 = u_xlat0 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_1 = texture(_MainTex, u_xlat0.zw);
    u_xlat16_0 = texture(_MainTex, u_xlat0.xy);
    u_xlat2 = _MainTex_TexelSize.xyxy * vec4(0.0, -1.5, 0.0, 1.5) + vs_TEXCOORD0.xyxy;
    u_xlat2 = u_xlat2 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_3 = texture(_MainTex, u_xlat2.zw);
    u_xlat16_2 = texture(_MainTex, u_xlat2.xy);
    u_xlat0 = u_xlat16_0.wxyz + u_xlat16_3.wxyz;
    u_xlat3 = u_xlat16_3 + u_xlat16_2;
    u_xlat0 = u_xlat16_1.wxyz + u_xlat0;
    u_xlat1 = _MainTex_TexelSize.xyxy * vec4(0.0, 7.5, 0.0, -3.5) + vs_TEXCOORD0.xyxy;
    u_xlat1 = u_xlat1 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_4 = texture(_MainTex, u_xlat1.xy);
    u_xlat16_1 = texture(_MainTex, u_xlat1.zw);
    u_xlat0 = u_xlat0 + u_xlat16_4.wxyz;
    u_xlat0 = u_xlat16_2.wxyz + u_xlat0;
    u_xlat0 = u_xlat16_1.wxyz + u_xlat0;
    u_xlat1 = _MainTex_TexelSize.xyxy * vec4(0.0, -5.5, 0.0, -7.5) + vs_TEXCOORD0.xyxy;
    u_xlat1 = u_xlat1 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_2 = texture(_MainTex, u_xlat1.xy);
    u_xlat16_1 = texture(_MainTex, u_xlat1.zw);
    u_xlat0 = u_xlat0 + u_xlat16_2.wxyz;
    u_xlat0 = u_xlat16_1.wxyz + u_xlat0;
    u_xlat0.x = u_xlat0.x * 0.25 + -1.0;
#ifdef UNITY_ADRENO_ES3
    u_xlat0.x = min(max(u_xlat0.x, 0.0), 1.0);
#else
    u_xlat0.x = clamp(u_xlat0.x, 0.0, 1.0);
#endif
    u_xlat11.xyz = u_xlat0.yzw * vec3(0.125, 0.125, 0.125);
    u_xlat11.x = dot(u_xlat11.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat1 = _MainTex_TexelSize.xyxy * vec4(-1.0, 0.0, 1.0, 0.0) + vs_TEXCOORD0.xyxy;
    u_xlat1 = u_xlat1 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_2 = texture(_MainTex, u_xlat1.xy);
    u_xlat16_1 = texture(_MainTex, u_xlat1.zw);
    u_xlat22 = dot(u_xlat16_2.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat33 = (-u_xlat22) + u_xlat11.x;
    u_xlat4.xy = vs_TEXCOORD0.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_4 = texture(_MainTex, u_xlat4.xy);
    u_xlat5 = dot(u_xlat16_4.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat16.x = (-u_xlat22) + u_xlat5;
#ifdef UNITY_ADRENO_ES3
    u_xlatb22 = !!(u_xlat22==u_xlat5);
#else
    u_xlatb22 = u_xlat22==u_xlat5;
#endif
    u_xlat33 = u_xlat33 / u_xlat16.x;
#ifdef UNITY_ADRENO_ES3
    u_xlat33 = min(max(u_xlat33, 0.0), 1.0);
#else
    u_xlat33 = clamp(u_xlat33, 0.0, 1.0);
#endif
    u_xlat22 = (u_xlatb22) ? 0.0 : u_xlat33;
    u_xlat6 = (-u_xlat16_2) + u_xlat16_4;
    u_xlat6 = vec4(u_xlat22) * u_xlat6 + u_xlat16_2;
    u_xlat2.xyz = u_xlat16_1.xyz + u_xlat16_2.xyz;
    u_xlat2.xyz = (-u_xlat16_4.xyz) * vec3(2.0, 2.0, 2.0) + u_xlat2.xyz;
    u_xlat2.xyz = abs(u_xlat2.xyz) * vec3(0.5, 0.5, 0.5);
    u_xlat22 = dot(u_xlat2.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat22 = u_xlat22 * 3.0 + -0.100000001;
    u_xlat2 = (-u_xlat16_1) + u_xlat6;
    u_xlat11.x = u_xlat11.x + (-u_xlat5);
    u_xlat33 = dot(u_xlat16_1.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat16.x = (-u_xlat33) + u_xlat5;
#ifdef UNITY_ADRENO_ES3
    u_xlatb33 = !!(u_xlat33==u_xlat5);
#else
    u_xlatb33 = u_xlat33==u_xlat5;
#endif
    u_xlat11.x = u_xlat11.x / u_xlat16.x;
    u_xlat11.x = u_xlat11.x + 1.0;
#ifdef UNITY_ADRENO_ES3
    u_xlat11.x = min(max(u_xlat11.x, 0.0), 1.0);
#else
    u_xlat11.x = clamp(u_xlat11.x, 0.0, 1.0);
#endif
    u_xlat11.x = (u_xlatb33) ? 0.0 : u_xlat11.x;
    u_xlat1 = u_xlat11.xxxx * u_xlat2 + u_xlat16_1;
    u_xlat2 = u_xlat16_4 + u_xlat16_4;
    u_xlat3 = u_xlat3 * vec4(2.0, 2.0, 2.0, 2.0) + u_xlat2;
    u_xlat16.xyz = u_xlat3.xyz * vec3(0.166666672, 0.166666672, 0.166666672);
    u_xlat11.x = dot(u_xlat16.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat11.x = u_xlat22 / u_xlat11.x;
#ifdef UNITY_ADRENO_ES3
    u_xlat11.x = min(max(u_xlat11.x, 0.0), 1.0);
#else
    u_xlat11.x = clamp(u_xlat11.x, 0.0, 1.0);
#endif
    u_xlat11.x = u_xlat11.x * 0.5;
    u_xlat6 = _MainTex_TexelSize.xyxy * vec4(-1.5, 0.0, 1.5, 0.0) + vs_TEXCOORD0.xyxy;
    u_xlat6 = u_xlat6 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_7 = texture(_MainTex, u_xlat6.xy);
    u_xlat16_6 = texture(_MainTex, u_xlat6.zw);
    u_xlat8 = u_xlat16_6 + u_xlat16_7;
    u_xlat2 = u_xlat8 * vec4(2.0, 2.0, 2.0, 2.0) + u_xlat2;
    u_xlat8 = u_xlat2 * vec4(0.166666672, 0.166666672, 0.166666672, 0.166666672) + (-u_xlat16_4);
    u_xlat2.xyz = u_xlat2.xyz * vec3(0.166666672, 0.166666672, 0.166666672);
    u_xlat22 = dot(u_xlat2.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat2 = _MainTex_TexelSize.xyxy * vec4(0.0, -1.0, 0.0, 1.0) + vs_TEXCOORD0.xyxy;
    u_xlat2 = u_xlat2 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_9 = texture(_MainTex, u_xlat2.xy);
    u_xlat16_2 = texture(_MainTex, u_xlat2.zw);
    u_xlat16.xyz = u_xlat16_2.xyz + u_xlat16_9.xyz;
    u_xlat16.xyz = (-u_xlat16_4.xyz) * vec3(2.0, 2.0, 2.0) + u_xlat16.xyz;
    u_xlat16.xyz = abs(u_xlat16.xyz) * vec3(0.5, 0.5, 0.5);
    u_xlat33 = dot(u_xlat16.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat33 = u_xlat33 * 3.0 + -0.100000001;
    u_xlat22 = u_xlat33 / u_xlat22;
#ifdef UNITY_ADRENO_ES3
    u_xlat22 = min(max(u_xlat22, 0.0), 1.0);
#else
    u_xlat22 = clamp(u_xlat22, 0.0, 1.0);
#endif
    u_xlat8 = vec4(u_xlat22) * u_xlat8 + u_xlat16_4;
    u_xlat4 = u_xlat16_4 + (-u_xlat16_9);
    u_xlat3 = u_xlat3 * vec4(0.166666672, 0.166666672, 0.166666672, 0.166666672) + (-u_xlat8);
    u_xlat3 = u_xlat11.xxxx * u_xlat3 + u_xlat8;
    u_xlat1 = u_xlat1 + (-u_xlat3);
    u_xlat1 = u_xlat0.xxxx * u_xlat1 + u_xlat3;
    u_xlat8 = _MainTex_TexelSize.xyxy * vec4(3.5, 0.0, 5.5, 0.0) + vs_TEXCOORD0.xyxy;
    u_xlat8 = u_xlat8 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_10 = texture(_MainTex, u_xlat8.xy);
    u_xlat16_8 = texture(_MainTex, u_xlat8.zw);
    u_xlat6 = u_xlat16_6.wxyz + u_xlat16_10.wxyz;
    u_xlat6 = u_xlat16_8.wxyz + u_xlat6;
    u_xlat8 = _MainTex_TexelSize.xyxy * vec4(7.5, 0.0, -3.5, 0.0) + vs_TEXCOORD0.xyxy;
    u_xlat8 = u_xlat8 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_10 = texture(_MainTex, u_xlat8.xy);
    u_xlat16_8 = texture(_MainTex, u_xlat8.zw);
    u_xlat6 = u_xlat6 + u_xlat16_10.wxyz;
    u_xlat6 = u_xlat16_7.wxyz + u_xlat6;
    u_xlat6 = u_xlat16_8.wxyz + u_xlat6;
    u_xlat7 = _MainTex_TexelSize.xyxy * vec4(-5.5, 0.0, -7.5, 0.0) + vs_TEXCOORD0.xyxy;
    u_xlat7 = u_xlat7 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_8 = texture(_MainTex, u_xlat7.xy);
    u_xlat16_7 = texture(_MainTex, u_xlat7.zw);
    u_xlat6 = u_xlat6 + u_xlat16_8.wxyz;
    u_xlat6 = u_xlat16_7.wxyz + u_xlat6;
    u_xlat11.xyz = u_xlat6.yzw * vec3(0.125, 0.125, 0.125);
    u_xlat16.x = u_xlat6.x * 0.25 + -1.0;
#ifdef UNITY_ADRENO_ES3
    u_xlat16.x = min(max(u_xlat16.x, 0.0), 1.0);
#else
    u_xlat16.x = clamp(u_xlat16.x, 0.0, 1.0);
#endif
    u_xlat11.x = dot(u_xlat11.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat22 = dot(u_xlat16_9.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat33 = (-u_xlat22) + u_xlat11.x;
    u_xlat11.x = (-u_xlat5) + u_xlat11.x;
    u_xlat27 = (-u_xlat22) + u_xlat5;
#ifdef UNITY_ADRENO_ES3
    u_xlatb22 = !!(u_xlat22==u_xlat5);
#else
    u_xlatb22 = u_xlat22==u_xlat5;
#endif
    u_xlat33 = u_xlat33 / u_xlat27;
#ifdef UNITY_ADRENO_ES3
    u_xlat33 = min(max(u_xlat33, 0.0), 1.0);
#else
    u_xlat33 = clamp(u_xlat33, 0.0, 1.0);
#endif
    u_xlat22 = (u_xlatb22) ? 0.0 : u_xlat33;
    u_xlat4 = vec4(u_xlat22) * u_xlat4 + u_xlat16_9;
    u_xlat4 = (-u_xlat16_2) + u_xlat4;
    u_xlat22 = dot(u_xlat16_2.xyz, vec3(0.330000013, 0.330000013, 0.330000013));
    u_xlat33 = (-u_xlat22) + u_xlat5;
#ifdef UNITY_ADRENO_ES3
    u_xlatb22 = !!(u_xlat22==u_xlat5);
#else
    u_xlatb22 = u_xlat22==u_xlat5;
#endif
    u_xlat11.x = u_xlat11.x / u_xlat33;
    u_xlat11.x = u_xlat11.x + 1.0;
#ifdef UNITY_ADRENO_ES3
    u_xlat11.x = min(max(u_xlat11.x, 0.0), 1.0);
#else
    u_xlat11.x = clamp(u_xlat11.x, 0.0, 1.0);
#endif
    u_xlat11.x = (u_xlatb22) ? 0.0 : u_xlat11.x;
    u_xlat2 = u_xlat11.xxxx * u_xlat4 + u_xlat16_2;
    u_xlat2 = (-u_xlat1) + u_xlat2;
    u_xlat1 = u_xlat16.xxxx * u_xlat2 + u_xlat1;
    u_xlat0.x = (-u_xlat0.x) + u_xlat16.x;
#ifdef UNITY_ADRENO_ES3
    u_xlatb0 = !!(0.200000003<abs(u_xlat0.x));
#else
    u_xlatb0 = 0.200000003<abs(u_xlat0.x);
#endif
    u_xlat0 = (bool(u_xlatb0)) ? u_xlat1 : u_xlat3;
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
}
}