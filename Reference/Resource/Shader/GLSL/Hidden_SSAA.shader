//////////////////////////////////////////
//
// NOTE: This is *not* a valid shader file
//
///////////////////////////////////////////
Shader "Hidden/SSAA" {
Properties {
_MainTex ("Base (RGB)", 2D) = "white" { }
}
SubShader {
 Pass {
  ZTest Always
  ZWrite Off
  Cull Off
  GpuProgramID 24552
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
varying highp vec2 vs_TEXCOORD1;
varying highp vec2 vs_TEXCOORD2;
varying highp vec2 vs_TEXCOORD3;
varying highp vec2 vs_TEXCOORD4;
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
    u_xlat0.yz = _MainTex_TexelSize.yx * vec2(1.75, 1.75);
    u_xlat0.x = float(0.0);
    u_xlat0.w = float(0.0);
    vs_TEXCOORD0.xy = (-u_xlat0.xy) + in_TEXCOORD0.xy;
    vs_TEXCOORD1.xy = (-u_xlat0.zw) + in_TEXCOORD0.xy;
    vs_TEXCOORD2.xy = u_xlat0.zw + in_TEXCOORD0.xy;
    vs_TEXCOORD3.xy = u_xlat0.xy + in_TEXCOORD0.xy;
    vs_TEXCOORD4.xy = in_TEXCOORD0.xy;
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
varying highp vec2 vs_TEXCOORD1;
varying highp vec2 vs_TEXCOORD2;
varying highp vec2 vs_TEXCOORD3;
varying highp vec2 vs_TEXCOORD4;
#define SV_Target0 gl_FragData[0]
vec2 u_xlat0;
mediump vec4 u_xlat16_0;
lowp vec4 u_xlat10_0;
mediump vec4 u_xlat16_1;
lowp vec4 u_xlat10_1;
vec2 u_xlat2;
lowp vec4 u_xlat10_2;
lowp vec4 u_xlat10_3;
vec2 u_xlat4;
lowp vec4 u_xlat10_4;
mediump vec3 u_xlat16_6;
vec2 u_xlat10;
bool u_xlatb10;
vec2 u_xlat12;
void main()
{
    u_xlat0.xy = vs_TEXCOORD0.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_0.xyz = texture2D(_MainTex, u_xlat0.xy).xyz;
    u_xlat16_1.x = dot(u_xlat10_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat0.xy = vs_TEXCOORD1.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_0.xyz = texture2D(_MainTex, u_xlat0.xy).xyz;
    u_xlat16_6.x = dot(u_xlat10_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat0.xy = vs_TEXCOORD2.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_0.xyz = texture2D(_MainTex, u_xlat0.xy).xyz;
    u_xlat16_1.z = dot(u_xlat10_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat0.xy = vs_TEXCOORD3.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_0.xyz = texture2D(_MainTex, u_xlat0.xy).xyz;
    u_xlat16_6.z = dot(u_xlat10_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat0.xy = (-u_xlat16_6.zx) + u_xlat16_1.xz;
    u_xlat0.x = (-u_xlat0.x);
    u_xlat16_1.x = dot(u_xlat0.xy, u_xlat0.xy);
    u_xlat16_1.x = sqrt(u_xlat16_1.x);
    u_xlatb10 = u_xlat16_1.x<0.0625;
    if(u_xlatb10){
        u_xlat10.xy = vs_TEXCOORD4.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat10_2 = texture2D(_MainTex, u_xlat10.xy);
        SV_Target0 = u_xlat10_2;
    } else {
        u_xlat10.xy = _MainTex_TexelSize.xy / u_xlat16_1.xx;
        u_xlat2.xy = u_xlat10.xy * u_xlat0.xy;
        u_xlat12.xy = vs_TEXCOORD4.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat10_1 = texture2D(_MainTex, u_xlat12.xy);
        u_xlat12.xy = u_xlat2.xy * vec2(0.5, 0.5) + vs_TEXCOORD4.xy;
        u_xlat12.xy = u_xlat12.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat10_3 = texture2D(_MainTex, u_xlat12.xy);
        u_xlat2.xy = (-u_xlat2.xy) * vec2(0.5, 0.5) + vs_TEXCOORD4.xy;
        u_xlat2.xy = u_xlat2.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat10_2 = texture2D(_MainTex, u_xlat2.xy);
        u_xlat4.xy = u_xlat0.xy * u_xlat10.xy + vs_TEXCOORD4.xy;
        u_xlat4.xy = u_xlat4.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat10_4 = texture2D(_MainTex, u_xlat4.xy);
        u_xlat0.xy = (-u_xlat0.xy) * u_xlat10.xy + vs_TEXCOORD4.xy;
        u_xlat0.xy = u_xlat0.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat10_0 = texture2D(_MainTex, u_xlat0.xy);
        u_xlat16_1 = u_xlat10_3 * vec4(0.899999976, 0.899999976, 0.899999976, 0.899999976) + u_xlat10_1;
        u_xlat16_1 = u_xlat10_2 * vec4(0.899999976, 0.899999976, 0.899999976, 0.899999976) + u_xlat16_1;
        u_xlat16_1 = u_xlat10_4 * vec4(0.75, 0.75, 0.75, 0.75) + u_xlat16_1;
        u_xlat16_0 = u_xlat10_0 * vec4(0.75, 0.75, 0.75, 0.75) + u_xlat16_1;
        SV_Target0 = u_xlat16_0 * vec4(0.232558131, 0.232558131, 0.232558131, 0.232558131);
    //ENDIF
    }
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
varying highp vec2 vs_TEXCOORD1;
varying highp vec2 vs_TEXCOORD2;
varying highp vec2 vs_TEXCOORD3;
varying highp vec2 vs_TEXCOORD4;
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
    u_xlat0.yz = _MainTex_TexelSize.yx * vec2(1.75, 1.75);
    u_xlat0.x = float(0.0);
    u_xlat0.w = float(0.0);
    vs_TEXCOORD0.xy = (-u_xlat0.xy) + in_TEXCOORD0.xy;
    vs_TEXCOORD1.xy = (-u_xlat0.zw) + in_TEXCOORD0.xy;
    vs_TEXCOORD2.xy = u_xlat0.zw + in_TEXCOORD0.xy;
    vs_TEXCOORD3.xy = u_xlat0.xy + in_TEXCOORD0.xy;
    vs_TEXCOORD4.xy = in_TEXCOORD0.xy;
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
varying highp vec2 vs_TEXCOORD1;
varying highp vec2 vs_TEXCOORD2;
varying highp vec2 vs_TEXCOORD3;
varying highp vec2 vs_TEXCOORD4;
#define SV_Target0 gl_FragData[0]
vec2 u_xlat0;
mediump vec4 u_xlat16_0;
lowp vec4 u_xlat10_0;
mediump vec4 u_xlat16_1;
lowp vec4 u_xlat10_1;
vec2 u_xlat2;
lowp vec4 u_xlat10_2;
lowp vec4 u_xlat10_3;
vec2 u_xlat4;
lowp vec4 u_xlat10_4;
mediump vec3 u_xlat16_6;
vec2 u_xlat10;
bool u_xlatb10;
vec2 u_xlat12;
void main()
{
    u_xlat0.xy = vs_TEXCOORD0.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_0.xyz = texture2D(_MainTex, u_xlat0.xy).xyz;
    u_xlat16_1.x = dot(u_xlat10_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat0.xy = vs_TEXCOORD1.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_0.xyz = texture2D(_MainTex, u_xlat0.xy).xyz;
    u_xlat16_6.x = dot(u_xlat10_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat0.xy = vs_TEXCOORD2.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_0.xyz = texture2D(_MainTex, u_xlat0.xy).xyz;
    u_xlat16_1.z = dot(u_xlat10_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat0.xy = vs_TEXCOORD3.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_0.xyz = texture2D(_MainTex, u_xlat0.xy).xyz;
    u_xlat16_6.z = dot(u_xlat10_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat0.xy = (-u_xlat16_6.zx) + u_xlat16_1.xz;
    u_xlat0.x = (-u_xlat0.x);
    u_xlat16_1.x = dot(u_xlat0.xy, u_xlat0.xy);
    u_xlat16_1.x = sqrt(u_xlat16_1.x);
    u_xlatb10 = u_xlat16_1.x<0.0625;
    if(u_xlatb10){
        u_xlat10.xy = vs_TEXCOORD4.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat10_2 = texture2D(_MainTex, u_xlat10.xy);
        SV_Target0 = u_xlat10_2;
    } else {
        u_xlat10.xy = _MainTex_TexelSize.xy / u_xlat16_1.xx;
        u_xlat2.xy = u_xlat10.xy * u_xlat0.xy;
        u_xlat12.xy = vs_TEXCOORD4.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat10_1 = texture2D(_MainTex, u_xlat12.xy);
        u_xlat12.xy = u_xlat2.xy * vec2(0.5, 0.5) + vs_TEXCOORD4.xy;
        u_xlat12.xy = u_xlat12.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat10_3 = texture2D(_MainTex, u_xlat12.xy);
        u_xlat2.xy = (-u_xlat2.xy) * vec2(0.5, 0.5) + vs_TEXCOORD4.xy;
        u_xlat2.xy = u_xlat2.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat10_2 = texture2D(_MainTex, u_xlat2.xy);
        u_xlat4.xy = u_xlat0.xy * u_xlat10.xy + vs_TEXCOORD4.xy;
        u_xlat4.xy = u_xlat4.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat10_4 = texture2D(_MainTex, u_xlat4.xy);
        u_xlat0.xy = (-u_xlat0.xy) * u_xlat10.xy + vs_TEXCOORD4.xy;
        u_xlat0.xy = u_xlat0.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat10_0 = texture2D(_MainTex, u_xlat0.xy);
        u_xlat16_1 = u_xlat10_3 * vec4(0.899999976, 0.899999976, 0.899999976, 0.899999976) + u_xlat10_1;
        u_xlat16_1 = u_xlat10_2 * vec4(0.899999976, 0.899999976, 0.899999976, 0.899999976) + u_xlat16_1;
        u_xlat16_1 = u_xlat10_4 * vec4(0.75, 0.75, 0.75, 0.75) + u_xlat16_1;
        u_xlat16_0 = u_xlat10_0 * vec4(0.75, 0.75, 0.75, 0.75) + u_xlat16_1;
        SV_Target0 = u_xlat16_0 * vec4(0.232558131, 0.232558131, 0.232558131, 0.232558131);
    //ENDIF
    }
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
varying highp vec2 vs_TEXCOORD1;
varying highp vec2 vs_TEXCOORD2;
varying highp vec2 vs_TEXCOORD3;
varying highp vec2 vs_TEXCOORD4;
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
    u_xlat0.yz = _MainTex_TexelSize.yx * vec2(1.75, 1.75);
    u_xlat0.x = float(0.0);
    u_xlat0.w = float(0.0);
    vs_TEXCOORD0.xy = (-u_xlat0.xy) + in_TEXCOORD0.xy;
    vs_TEXCOORD1.xy = (-u_xlat0.zw) + in_TEXCOORD0.xy;
    vs_TEXCOORD2.xy = u_xlat0.zw + in_TEXCOORD0.xy;
    vs_TEXCOORD3.xy = u_xlat0.xy + in_TEXCOORD0.xy;
    vs_TEXCOORD4.xy = in_TEXCOORD0.xy;
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
varying highp vec2 vs_TEXCOORD1;
varying highp vec2 vs_TEXCOORD2;
varying highp vec2 vs_TEXCOORD3;
varying highp vec2 vs_TEXCOORD4;
#define SV_Target0 gl_FragData[0]
vec2 u_xlat0;
mediump vec4 u_xlat16_0;
lowp vec4 u_xlat10_0;
mediump vec4 u_xlat16_1;
lowp vec4 u_xlat10_1;
vec2 u_xlat2;
lowp vec4 u_xlat10_2;
lowp vec4 u_xlat10_3;
vec2 u_xlat4;
lowp vec4 u_xlat10_4;
mediump vec3 u_xlat16_6;
vec2 u_xlat10;
bool u_xlatb10;
vec2 u_xlat12;
void main()
{
    u_xlat0.xy = vs_TEXCOORD0.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_0.xyz = texture2D(_MainTex, u_xlat0.xy).xyz;
    u_xlat16_1.x = dot(u_xlat10_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat0.xy = vs_TEXCOORD1.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_0.xyz = texture2D(_MainTex, u_xlat0.xy).xyz;
    u_xlat16_6.x = dot(u_xlat10_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat0.xy = vs_TEXCOORD2.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_0.xyz = texture2D(_MainTex, u_xlat0.xy).xyz;
    u_xlat16_1.z = dot(u_xlat10_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat0.xy = vs_TEXCOORD3.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat10_0.xyz = texture2D(_MainTex, u_xlat0.xy).xyz;
    u_xlat16_6.z = dot(u_xlat10_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat0.xy = (-u_xlat16_6.zx) + u_xlat16_1.xz;
    u_xlat0.x = (-u_xlat0.x);
    u_xlat16_1.x = dot(u_xlat0.xy, u_xlat0.xy);
    u_xlat16_1.x = sqrt(u_xlat16_1.x);
    u_xlatb10 = u_xlat16_1.x<0.0625;
    if(u_xlatb10){
        u_xlat10.xy = vs_TEXCOORD4.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat10_2 = texture2D(_MainTex, u_xlat10.xy);
        SV_Target0 = u_xlat10_2;
    } else {
        u_xlat10.xy = _MainTex_TexelSize.xy / u_xlat16_1.xx;
        u_xlat2.xy = u_xlat10.xy * u_xlat0.xy;
        u_xlat12.xy = vs_TEXCOORD4.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat10_1 = texture2D(_MainTex, u_xlat12.xy);
        u_xlat12.xy = u_xlat2.xy * vec2(0.5, 0.5) + vs_TEXCOORD4.xy;
        u_xlat12.xy = u_xlat12.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat10_3 = texture2D(_MainTex, u_xlat12.xy);
        u_xlat2.xy = (-u_xlat2.xy) * vec2(0.5, 0.5) + vs_TEXCOORD4.xy;
        u_xlat2.xy = u_xlat2.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat10_2 = texture2D(_MainTex, u_xlat2.xy);
        u_xlat4.xy = u_xlat0.xy * u_xlat10.xy + vs_TEXCOORD4.xy;
        u_xlat4.xy = u_xlat4.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat10_4 = texture2D(_MainTex, u_xlat4.xy);
        u_xlat0.xy = (-u_xlat0.xy) * u_xlat10.xy + vs_TEXCOORD4.xy;
        u_xlat0.xy = u_xlat0.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat10_0 = texture2D(_MainTex, u_xlat0.xy);
        u_xlat16_1 = u_xlat10_3 * vec4(0.899999976, 0.899999976, 0.899999976, 0.899999976) + u_xlat10_1;
        u_xlat16_1 = u_xlat10_2 * vec4(0.899999976, 0.899999976, 0.899999976, 0.899999976) + u_xlat16_1;
        u_xlat16_1 = u_xlat10_4 * vec4(0.75, 0.75, 0.75, 0.75) + u_xlat16_1;
        u_xlat16_0 = u_xlat10_0 * vec4(0.75, 0.75, 0.75, 0.75) + u_xlat16_1;
        SV_Target0 = u_xlat16_0 * vec4(0.232558131, 0.232558131, 0.232558131, 0.232558131);
    //ENDIF
    }
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
out highp vec2 vs_TEXCOORD1;
out highp vec2 vs_TEXCOORD2;
out highp vec2 vs_TEXCOORD3;
out highp vec2 vs_TEXCOORD4;
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
    u_xlat0.yz = _MainTex_TexelSize.yx * vec2(1.75, 1.75);
    u_xlat0.x = float(0.0);
    u_xlat0.w = float(0.0);
    vs_TEXCOORD0.xy = (-u_xlat0.xy) + in_TEXCOORD0.xy;
    vs_TEXCOORD1.xy = (-u_xlat0.zw) + in_TEXCOORD0.xy;
    vs_TEXCOORD2.xy = u_xlat0.zw + in_TEXCOORD0.xy;
    vs_TEXCOORD3.xy = u_xlat0.xy + in_TEXCOORD0.xy;
    vs_TEXCOORD4.xy = in_TEXCOORD0.xy;
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
in highp vec2 vs_TEXCOORD1;
in highp vec2 vs_TEXCOORD2;
in highp vec2 vs_TEXCOORD3;
in highp vec2 vs_TEXCOORD4;
layout(location = 0) out mediump vec4 SV_Target0;
vec2 u_xlat0;
mediump vec4 u_xlat16_0;
mediump vec4 u_xlat16_1;
vec2 u_xlat2;
mediump vec4 u_xlat16_2;
mediump vec4 u_xlat16_3;
vec2 u_xlat4;
mediump vec4 u_xlat16_4;
mediump vec3 u_xlat16_6;
vec2 u_xlat10;
bool u_xlatb10;
vec2 u_xlat12;
void main()
{
    u_xlat0.xy = vs_TEXCOORD0.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_0.xyz = texture(_MainTex, u_xlat0.xy).xyz;
    u_xlat16_1.x = dot(u_xlat16_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat0.xy = vs_TEXCOORD1.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_0.xyz = texture(_MainTex, u_xlat0.xy).xyz;
    u_xlat16_6.x = dot(u_xlat16_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat0.xy = vs_TEXCOORD2.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_0.xyz = texture(_MainTex, u_xlat0.xy).xyz;
    u_xlat16_1.z = dot(u_xlat16_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat0.xy = vs_TEXCOORD3.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_0.xyz = texture(_MainTex, u_xlat0.xy).xyz;
    u_xlat16_6.z = dot(u_xlat16_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat0.xy = (-u_xlat16_6.zx) + u_xlat16_1.xz;
    u_xlat0.x = (-u_xlat0.x);
    u_xlat16_1.x = dot(u_xlat0.xy, u_xlat0.xy);
    u_xlat16_1.x = sqrt(u_xlat16_1.x);
#ifdef UNITY_ADRENO_ES3
    u_xlatb10 = !!(u_xlat16_1.x<0.0625);
#else
    u_xlatb10 = u_xlat16_1.x<0.0625;
#endif
    if(u_xlatb10){
        u_xlat10.xy = vs_TEXCOORD4.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat16_2 = texture(_MainTex, u_xlat10.xy);
        SV_Target0 = u_xlat16_2;
    } else {
        u_xlat10.xy = _MainTex_TexelSize.xy / u_xlat16_1.xx;
        u_xlat2.xy = u_xlat10.xy * u_xlat0.xy;
        u_xlat12.xy = vs_TEXCOORD4.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat16_1 = texture(_MainTex, u_xlat12.xy);
        u_xlat12.xy = u_xlat2.xy * vec2(0.5, 0.5) + vs_TEXCOORD4.xy;
        u_xlat12.xy = u_xlat12.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat16_3 = texture(_MainTex, u_xlat12.xy);
        u_xlat2.xy = (-u_xlat2.xy) * vec2(0.5, 0.5) + vs_TEXCOORD4.xy;
        u_xlat2.xy = u_xlat2.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat16_2 = texture(_MainTex, u_xlat2.xy);
        u_xlat4.xy = u_xlat0.xy * u_xlat10.xy + vs_TEXCOORD4.xy;
        u_xlat4.xy = u_xlat4.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat16_4 = texture(_MainTex, u_xlat4.xy);
        u_xlat0.xy = (-u_xlat0.xy) * u_xlat10.xy + vs_TEXCOORD4.xy;
        u_xlat0.xy = u_xlat0.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat16_0 = texture(_MainTex, u_xlat0.xy);
        u_xlat16_1 = u_xlat16_3 * vec4(0.899999976, 0.899999976, 0.899999976, 0.899999976) + u_xlat16_1;
        u_xlat16_1 = u_xlat16_2 * vec4(0.899999976, 0.899999976, 0.899999976, 0.899999976) + u_xlat16_1;
        u_xlat16_1 = u_xlat16_4 * vec4(0.75, 0.75, 0.75, 0.75) + u_xlat16_1;
        u_xlat16_0 = u_xlat16_0 * vec4(0.75, 0.75, 0.75, 0.75) + u_xlat16_1;
        SV_Target0 = u_xlat16_0 * vec4(0.232558131, 0.232558131, 0.232558131, 0.232558131);
    //ENDIF
    }
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
out highp vec2 vs_TEXCOORD1;
out highp vec2 vs_TEXCOORD2;
out highp vec2 vs_TEXCOORD3;
out highp vec2 vs_TEXCOORD4;
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
    u_xlat0.yz = _MainTex_TexelSize.yx * vec2(1.75, 1.75);
    u_xlat0.x = float(0.0);
    u_xlat0.w = float(0.0);
    vs_TEXCOORD0.xy = (-u_xlat0.xy) + in_TEXCOORD0.xy;
    vs_TEXCOORD1.xy = (-u_xlat0.zw) + in_TEXCOORD0.xy;
    vs_TEXCOORD2.xy = u_xlat0.zw + in_TEXCOORD0.xy;
    vs_TEXCOORD3.xy = u_xlat0.xy + in_TEXCOORD0.xy;
    vs_TEXCOORD4.xy = in_TEXCOORD0.xy;
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
in highp vec2 vs_TEXCOORD1;
in highp vec2 vs_TEXCOORD2;
in highp vec2 vs_TEXCOORD3;
in highp vec2 vs_TEXCOORD4;
layout(location = 0) out mediump vec4 SV_Target0;
vec2 u_xlat0;
mediump vec4 u_xlat16_0;
mediump vec4 u_xlat16_1;
vec2 u_xlat2;
mediump vec4 u_xlat16_2;
mediump vec4 u_xlat16_3;
vec2 u_xlat4;
mediump vec4 u_xlat16_4;
mediump vec3 u_xlat16_6;
vec2 u_xlat10;
bool u_xlatb10;
vec2 u_xlat12;
void main()
{
    u_xlat0.xy = vs_TEXCOORD0.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_0.xyz = texture(_MainTex, u_xlat0.xy).xyz;
    u_xlat16_1.x = dot(u_xlat16_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat0.xy = vs_TEXCOORD1.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_0.xyz = texture(_MainTex, u_xlat0.xy).xyz;
    u_xlat16_6.x = dot(u_xlat16_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat0.xy = vs_TEXCOORD2.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_0.xyz = texture(_MainTex, u_xlat0.xy).xyz;
    u_xlat16_1.z = dot(u_xlat16_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat0.xy = vs_TEXCOORD3.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_0.xyz = texture(_MainTex, u_xlat0.xy).xyz;
    u_xlat16_6.z = dot(u_xlat16_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat0.xy = (-u_xlat16_6.zx) + u_xlat16_1.xz;
    u_xlat0.x = (-u_xlat0.x);
    u_xlat16_1.x = dot(u_xlat0.xy, u_xlat0.xy);
    u_xlat16_1.x = sqrt(u_xlat16_1.x);
#ifdef UNITY_ADRENO_ES3
    u_xlatb10 = !!(u_xlat16_1.x<0.0625);
#else
    u_xlatb10 = u_xlat16_1.x<0.0625;
#endif
    if(u_xlatb10){
        u_xlat10.xy = vs_TEXCOORD4.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat16_2 = texture(_MainTex, u_xlat10.xy);
        SV_Target0 = u_xlat16_2;
    } else {
        u_xlat10.xy = _MainTex_TexelSize.xy / u_xlat16_1.xx;
        u_xlat2.xy = u_xlat10.xy * u_xlat0.xy;
        u_xlat12.xy = vs_TEXCOORD4.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat16_1 = texture(_MainTex, u_xlat12.xy);
        u_xlat12.xy = u_xlat2.xy * vec2(0.5, 0.5) + vs_TEXCOORD4.xy;
        u_xlat12.xy = u_xlat12.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat16_3 = texture(_MainTex, u_xlat12.xy);
        u_xlat2.xy = (-u_xlat2.xy) * vec2(0.5, 0.5) + vs_TEXCOORD4.xy;
        u_xlat2.xy = u_xlat2.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat16_2 = texture(_MainTex, u_xlat2.xy);
        u_xlat4.xy = u_xlat0.xy * u_xlat10.xy + vs_TEXCOORD4.xy;
        u_xlat4.xy = u_xlat4.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat16_4 = texture(_MainTex, u_xlat4.xy);
        u_xlat0.xy = (-u_xlat0.xy) * u_xlat10.xy + vs_TEXCOORD4.xy;
        u_xlat0.xy = u_xlat0.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat16_0 = texture(_MainTex, u_xlat0.xy);
        u_xlat16_1 = u_xlat16_3 * vec4(0.899999976, 0.899999976, 0.899999976, 0.899999976) + u_xlat16_1;
        u_xlat16_1 = u_xlat16_2 * vec4(0.899999976, 0.899999976, 0.899999976, 0.899999976) + u_xlat16_1;
        u_xlat16_1 = u_xlat16_4 * vec4(0.75, 0.75, 0.75, 0.75) + u_xlat16_1;
        u_xlat16_0 = u_xlat16_0 * vec4(0.75, 0.75, 0.75, 0.75) + u_xlat16_1;
        SV_Target0 = u_xlat16_0 * vec4(0.232558131, 0.232558131, 0.232558131, 0.232558131);
    //ENDIF
    }
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
out highp vec2 vs_TEXCOORD1;
out highp vec2 vs_TEXCOORD2;
out highp vec2 vs_TEXCOORD3;
out highp vec2 vs_TEXCOORD4;
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
    u_xlat0.yz = _MainTex_TexelSize.yx * vec2(1.75, 1.75);
    u_xlat0.x = float(0.0);
    u_xlat0.w = float(0.0);
    vs_TEXCOORD0.xy = (-u_xlat0.xy) + in_TEXCOORD0.xy;
    vs_TEXCOORD1.xy = (-u_xlat0.zw) + in_TEXCOORD0.xy;
    vs_TEXCOORD2.xy = u_xlat0.zw + in_TEXCOORD0.xy;
    vs_TEXCOORD3.xy = u_xlat0.xy + in_TEXCOORD0.xy;
    vs_TEXCOORD4.xy = in_TEXCOORD0.xy;
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
in highp vec2 vs_TEXCOORD1;
in highp vec2 vs_TEXCOORD2;
in highp vec2 vs_TEXCOORD3;
in highp vec2 vs_TEXCOORD4;
layout(location = 0) out mediump vec4 SV_Target0;
vec2 u_xlat0;
mediump vec4 u_xlat16_0;
mediump vec4 u_xlat16_1;
vec2 u_xlat2;
mediump vec4 u_xlat16_2;
mediump vec4 u_xlat16_3;
vec2 u_xlat4;
mediump vec4 u_xlat16_4;
mediump vec3 u_xlat16_6;
vec2 u_xlat10;
bool u_xlatb10;
vec2 u_xlat12;
void main()
{
    u_xlat0.xy = vs_TEXCOORD0.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_0.xyz = texture(_MainTex, u_xlat0.xy).xyz;
    u_xlat16_1.x = dot(u_xlat16_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat0.xy = vs_TEXCOORD1.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_0.xyz = texture(_MainTex, u_xlat0.xy).xyz;
    u_xlat16_6.x = dot(u_xlat16_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat0.xy = vs_TEXCOORD2.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_0.xyz = texture(_MainTex, u_xlat0.xy).xyz;
    u_xlat16_1.z = dot(u_xlat16_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat0.xy = vs_TEXCOORD3.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat16_0.xyz = texture(_MainTex, u_xlat0.xy).xyz;
    u_xlat16_6.z = dot(u_xlat16_0.xyz, vec3(0.219999999, 0.707000017, 0.0710000023));
    u_xlat0.xy = (-u_xlat16_6.zx) + u_xlat16_1.xz;
    u_xlat0.x = (-u_xlat0.x);
    u_xlat16_1.x = dot(u_xlat0.xy, u_xlat0.xy);
    u_xlat16_1.x = sqrt(u_xlat16_1.x);
#ifdef UNITY_ADRENO_ES3
    u_xlatb10 = !!(u_xlat16_1.x<0.0625);
#else
    u_xlatb10 = u_xlat16_1.x<0.0625;
#endif
    if(u_xlatb10){
        u_xlat10.xy = vs_TEXCOORD4.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat16_2 = texture(_MainTex, u_xlat10.xy);
        SV_Target0 = u_xlat16_2;
    } else {
        u_xlat10.xy = _MainTex_TexelSize.xy / u_xlat16_1.xx;
        u_xlat2.xy = u_xlat10.xy * u_xlat0.xy;
        u_xlat12.xy = vs_TEXCOORD4.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat16_1 = texture(_MainTex, u_xlat12.xy);
        u_xlat12.xy = u_xlat2.xy * vec2(0.5, 0.5) + vs_TEXCOORD4.xy;
        u_xlat12.xy = u_xlat12.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat16_3 = texture(_MainTex, u_xlat12.xy);
        u_xlat2.xy = (-u_xlat2.xy) * vec2(0.5, 0.5) + vs_TEXCOORD4.xy;
        u_xlat2.xy = u_xlat2.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat16_2 = texture(_MainTex, u_xlat2.xy);
        u_xlat4.xy = u_xlat0.xy * u_xlat10.xy + vs_TEXCOORD4.xy;
        u_xlat4.xy = u_xlat4.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat16_4 = texture(_MainTex, u_xlat4.xy);
        u_xlat0.xy = (-u_xlat0.xy) * u_xlat10.xy + vs_TEXCOORD4.xy;
        u_xlat0.xy = u_xlat0.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat16_0 = texture(_MainTex, u_xlat0.xy);
        u_xlat16_1 = u_xlat16_3 * vec4(0.899999976, 0.899999976, 0.899999976, 0.899999976) + u_xlat16_1;
        u_xlat16_1 = u_xlat16_2 * vec4(0.899999976, 0.899999976, 0.899999976, 0.899999976) + u_xlat16_1;
        u_xlat16_1 = u_xlat16_4 * vec4(0.75, 0.75, 0.75, 0.75) + u_xlat16_1;
        u_xlat16_0 = u_xlat16_0 * vec4(0.75, 0.75, 0.75, 0.75) + u_xlat16_1;
        SV_Target0 = u_xlat16_0 * vec4(0.232558131, 0.232558131, 0.232558131, 0.232558131);
    //ENDIF
    }
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