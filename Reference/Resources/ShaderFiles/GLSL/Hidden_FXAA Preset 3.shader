//////////////////////////////////////////
//
// NOTE: This is *not* a valid shader file
//
///////////////////////////////////////////
Shader "Hidden/FXAA Preset 3" {
Properties {
_MainTex ("Base (RGB)", 2D) = "white" { }
}
SubShader {
 Pass {
  ZTest Always
  ZWrite Off
  Cull Off
  GpuProgramID 58047
Program "vp" {
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
#ifdef GL_EXT_shader_texture_lod
#extension GL_EXT_shader_texture_lod : enable
#endif

precision highp float;
precision highp int;
uniform 	mediump vec4 _MainTex_ST;
uniform 	vec4 _MainTex_TexelSize;
uniform mediump sampler2D _MainTex;
in highp vec2 vs_TEXCOORD0;
layout(location = 0) out highp vec4 SV_Target0;
vec4 u_xlat0;
mediump vec3 u_xlat16_0;
float u_xlat1;
mediump vec3 u_xlat16_1;
vec3 u_xlat2;
vec4 u_xlat3;
mediump vec3 u_xlat16_3;
vec2 u_xlat4;
mediump vec3 u_xlat16_4;
vec4 u_xlat5;
mediump vec3 u_xlat16_5;
mediump vec3 u_xlat16_6;
int u_xlati6;
vec3 u_xlat7;
mediump vec2 u_xlat16_7;
vec2 u_xlat9;
mediump vec3 u_xlat16_9;
bool u_xlatb9;
float u_xlat11;
bool u_xlatb11;
float u_xlat12;
float u_xlat13;
bool u_xlatb13;
int u_xlati14;
vec2 u_xlat15;
int u_xlati15;
bool u_xlatb15;
float u_xlat17;
vec2 u_xlat19;
bool u_xlatb19;
vec2 u_xlat20;
vec2 u_xlat21;
int u_xlati22;
float u_xlat24;
int u_xlati24;
bool u_xlatb24;
float u_xlat25;
float u_xlat26;
int u_xlati26;
float u_xlat27;
float u_xlat28;
float u_xlat30;
bool u_xlatb30;
void main()
{
    u_xlat0 = _MainTex_TexelSize.xyxy * vec4(0.0, -1.0, -1.0, 0.0) + vs_TEXCOORD0.xyxy;
    u_xlat0 = u_xlat0 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_1.xyz = textureLod(_MainTex, u_xlat0.xy, 0.0).xyz;
    u_xlat16_0.xyz = textureLod(_MainTex, u_xlat0.zw, 0.0).xyz;
    u_xlat2.xy = vs_TEXCOORD0.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat2.xyz = textureLod(_MainTex, u_xlat2.xy, 0.0).xyz;
    u_xlat3 = _MainTex_TexelSize.xyxy * vec4(1.0, 0.0, 0.0, 1.0) + vs_TEXCOORD0.xyxy;
    u_xlat3 = u_xlat3 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_4.xyz = textureLod(_MainTex, u_xlat3.xy, 0.0).xyz;
    u_xlat16_3.xyz = textureLod(_MainTex, u_xlat3.zw, 0.0).xyz;
    u_xlat24 = u_xlat16_1.y * 1.9632107 + u_xlat16_1.x;
    u_xlat25 = u_xlat16_0.y * 1.9632107 + u_xlat16_0.x;
    u_xlat26 = u_xlat2.y * 1.9632107 + u_xlat2.x;
    u_xlat27 = u_xlat16_4.y * 1.9632107 + u_xlat16_4.x;
    u_xlat28 = u_xlat16_3.y * 1.9632107 + u_xlat16_3.x;
    u_xlat5.x = min(u_xlat24, u_xlat25);
    u_xlat13 = min(u_xlat27, u_xlat28);
    u_xlat5.x = min(u_xlat13, u_xlat5.x);
    u_xlat5.x = min(u_xlat26, u_xlat5.x);
    u_xlat13 = max(u_xlat24, u_xlat25);
    u_xlat21.x = max(u_xlat27, u_xlat28);
    u_xlat13 = max(u_xlat21.x, u_xlat13);
    u_xlat13 = max(u_xlat26, u_xlat13);
    u_xlat5.x = (-u_xlat5.x) + u_xlat13;
    u_xlat13 = u_xlat13 * 0.125;
    u_xlat13 = max(u_xlat13, 0.0416666679);
#ifdef UNITY_ADRENO_ES3
    u_xlatb13 = !!(u_xlat5.x>=u_xlat13);
#else
    u_xlatb13 = u_xlat5.x>=u_xlat13;
#endif
    if(u_xlatb13){
        u_xlat0.xyz = u_xlat16_0.xyz + u_xlat16_1.xyz;
        u_xlat0.xyz = u_xlat2.xyz + u_xlat0.xyz;
        u_xlat0.xyz = u_xlat16_4.xyz + u_xlat0.xyz;
        u_xlat0.xyz = u_xlat16_3.xyz + u_xlat0.xyz;
        u_xlat1 = u_xlat24 + u_xlat25;
        u_xlat1 = u_xlat27 + u_xlat1;
        u_xlat1 = u_xlat28 + u_xlat1;
        u_xlat1 = u_xlat1 * 0.25 + (-u_xlat26);
        u_xlat1 = abs(u_xlat1) / u_xlat5.x;
        u_xlat1 = u_xlat1 + -0.25;
        u_xlat1 = max(u_xlat1, 0.0);
        u_xlat1 = u_xlat1 * 1.33333337;
        u_xlat1 = min(u_xlat1, 0.75);
        u_xlat9.xy = vs_TEXCOORD0.xy + (-_MainTex_TexelSize.xy);
        u_xlat9.xy = u_xlat9.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat16_3.xyz = textureLod(_MainTex, u_xlat9.xy, 0.0).xyz;
        u_xlat5 = _MainTex_TexelSize.xyxy * vec4(1.0, -1.0, -1.0, 1.0) + vs_TEXCOORD0.xyxy;
        u_xlat5 = u_xlat5 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
        u_xlat16_4.xyz = textureLod(_MainTex, u_xlat5.xy, 0.0).xyz;
        u_xlat16_5.xyz = textureLod(_MainTex, u_xlat5.zw, 0.0).xyz;
        u_xlat9.xy = vs_TEXCOORD0.xy + _MainTex_TexelSize.xy;
        u_xlat9.xy = u_xlat9.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat16_6.xyz = textureLod(_MainTex, u_xlat9.xy, 0.0).xyz;
        u_xlat7.xyz = u_xlat16_3.xyz + u_xlat16_4.xyz;
        u_xlat7.xyz = u_xlat16_5.xyz + u_xlat7.xyz;
        u_xlat7.xyz = u_xlat16_6.xyz + u_xlat7.xyz;
        u_xlat0.xyz = u_xlat0.xyz + u_xlat7.xyz;
        u_xlat0.xyz = vec3(u_xlat1) * u_xlat0.xyz;
        u_xlat9.x = u_xlat16_3.y * 1.9632107 + u_xlat16_3.x;
        u_xlat17 = u_xlat16_4.y * 1.9632107 + u_xlat16_4.x;
        u_xlat3.x = u_xlat16_5.y * 1.9632107 + u_xlat16_5.x;
        u_xlat11 = u_xlat16_6.y * 1.9632107 + u_xlat16_6.x;
        u_xlat19.x = u_xlat24 * -0.5;
        u_xlat19.x = u_xlat9.x * 0.25 + u_xlat19.x;
        u_xlat19.x = u_xlat17 * 0.25 + u_xlat19.x;
        u_xlat4.x = u_xlat25 * -0.5;
        u_xlat12 = u_xlat25 * 0.5 + (-u_xlat26);
        u_xlat20.x = u_xlat27 * -0.5;
        u_xlat12 = u_xlat27 * 0.5 + u_xlat12;
        u_xlat19.x = abs(u_xlat19.x) + abs(u_xlat12);
        u_xlat12 = u_xlat28 * -0.5;
        u_xlat12 = u_xlat3.x * 0.25 + u_xlat12;
        u_xlat12 = u_xlat11 * 0.25 + u_xlat12;
        u_xlat19.x = u_xlat19.x + abs(u_xlat12);
        u_xlat9.x = u_xlat9.x * 0.25 + u_xlat4.x;
        u_xlat9.x = u_xlat3.x * 0.25 + u_xlat9.x;
        u_xlat3.x = u_xlat24 * 0.5 + (-u_xlat26);
        u_xlat3.x = u_xlat28 * 0.5 + u_xlat3.x;
        u_xlat9.x = abs(u_xlat9.x) + abs(u_xlat3.x);
        u_xlat17 = u_xlat17 * 0.25 + u_xlat20.x;
        u_xlat17 = u_xlat11 * 0.25 + u_xlat17;
        u_xlat9.x = abs(u_xlat17) + u_xlat9.x;
#ifdef UNITY_ADRENO_ES3
        u_xlatb9 = !!(u_xlat9.x>=u_xlat19.x);
#else
        u_xlatb9 = u_xlat9.x>=u_xlat19.x;
#endif
        u_xlat17 = (u_xlatb9) ? (-_MainTex_TexelSize.y) : (-_MainTex_TexelSize.x);
        u_xlat24 = (u_xlatb9) ? u_xlat24 : u_xlat25;
        u_xlat25 = (u_xlatb9) ? u_xlat28 : u_xlat27;
        u_xlat3.x = (-u_xlat26) + u_xlat24;
        u_xlat11 = (-u_xlat26) + u_xlat25;
        u_xlat24 = u_xlat26 + u_xlat24;
        u_xlat24 = u_xlat24 * 0.5;
        u_xlat25 = u_xlat26 + u_xlat25;
        u_xlat25 = u_xlat25 * 0.5;
#ifdef UNITY_ADRENO_ES3
        u_xlatb19 = !!(abs(u_xlat3.x)>=abs(u_xlat11));
#else
        u_xlatb19 = abs(u_xlat3.x)>=abs(u_xlat11);
#endif
        u_xlat24 = (u_xlatb19) ? u_xlat24 : u_xlat25;
        u_xlat25 = (u_xlatb19) ? abs(u_xlat3.x) : abs(u_xlat11);
        u_xlat17 = (u_xlatb19) ? u_xlat17 : (-u_xlat17);
        u_xlat3.x = u_xlat17 * 0.5;
        u_xlat3.y = (u_xlatb9) ? 0.0 : u_xlat3.x;
        u_xlat3.x = u_xlatb9 ? u_xlat3.x : float(0.0);
        u_xlat4.xy = u_xlat3.yx + vs_TEXCOORD0.xy;
        u_xlat25 = u_xlat25 * 0.25;
        u_xlat3.y = float(0.0);
        u_xlat3.z = float(0.0);
        u_xlat3.xw = _MainTex_TexelSize.xy;
        u_xlat3.xy = (bool(u_xlatb9)) ? u_xlat3.xy : u_xlat3.zw;
        u_xlat19.xy = (-u_xlat3.xy) + u_xlat4.xy;
        u_xlat4.xy = u_xlat3.xy + u_xlat4.xy;
        u_xlat20.xy = u_xlat19.xy;
        u_xlat5.xy = u_xlat4.xy;
        u_xlat21.xy = vec2(u_xlat24);
        u_xlati6 = int(0);
        u_xlati14 = int(0);
        u_xlati22 = int(0);
        while(true){
#ifdef UNITY_ADRENO_ES3
            u_xlatb30 = !!(u_xlati22>=16);
#else
            u_xlatb30 = u_xlati22>=16;
#endif
            if(u_xlatb30){break;}
            if(u_xlati6 == 0) {
                u_xlat7.xy = u_xlat20.xy * _MainTex_ST.xy + _MainTex_ST.zw;
                u_xlat16_7.xy = textureLod(_MainTex, u_xlat7.xy, 0.0).xy;
                u_xlat30 = u_xlat16_7.y * 1.9632107 + u_xlat16_7.x;
            } else {
                u_xlat30 = u_xlat21.x;
            //ENDIF
            }
            if(u_xlati14 == 0) {
                u_xlat7.xy = u_xlat5.xy * _MainTex_ST.xy + _MainTex_ST.zw;
                u_xlat16_7.xy = textureLod(_MainTex, u_xlat7.xy, 0.0).xy;
                u_xlat7.x = u_xlat16_7.y * 1.9632107 + u_xlat16_7.x;
            } else {
                u_xlat7.x = u_xlat21.y;
            //ENDIF
            }
            u_xlat15.x = (-u_xlat24) + u_xlat30;
#ifdef UNITY_ADRENO_ES3
            u_xlatb15 = !!(abs(u_xlat15.x)>=u_xlat25);
#else
            u_xlatb15 = abs(u_xlat15.x)>=u_xlat25;
#endif
            u_xlati6 = int(uint(uint(u_xlati6) | (uint(u_xlatb15) * 0xffffffffu)));
            u_xlat15.x = (-u_xlat24) + u_xlat7.x;
#ifdef UNITY_ADRENO_ES3
            u_xlatb15 = !!(abs(u_xlat15.x)>=u_xlat25);
#else
            u_xlatb15 = abs(u_xlat15.x)>=u_xlat25;
#endif
            u_xlati14 = int(uint(uint(u_xlati14) | (uint(u_xlatb15) * 0xffffffffu)));
            u_xlati15 = int(uint(uint(u_xlati14) & uint(u_xlati6)));
            if(u_xlati15 != 0) {
                u_xlat21.x = u_xlat30;
                u_xlat21.y = u_xlat7.x;
                break;
            //ENDIF
            }
            u_xlat15.xy = (-u_xlat3.xy) + u_xlat20.xy;
            u_xlat20.xy = (int(u_xlati6) != 0) ? u_xlat20.xy : u_xlat15.xy;
            u_xlat15.xy = u_xlat3.xy + u_xlat5.xy;
            u_xlat5.xy = (int(u_xlati14) != 0) ? u_xlat5.xy : u_xlat15.xy;
            u_xlati22 = u_xlati22 + 1;
            u_xlat21.x = u_xlat30;
            u_xlat21.y = u_xlat7.x;
        }
        u_xlat3.xy = (-u_xlat20.xy) + vs_TEXCOORD0.xy;
        u_xlat25 = (u_xlatb9) ? u_xlat3.x : u_xlat3.y;
        u_xlat3.xy = u_xlat5.xy + (-vs_TEXCOORD0.xy);
        u_xlat3.x = (u_xlatb9) ? u_xlat3.x : u_xlat3.y;
#ifdef UNITY_ADRENO_ES3
        u_xlatb11 = !!(u_xlat25<u_xlat3.x);
#else
        u_xlatb11 = u_xlat25<u_xlat3.x;
#endif
        u_xlat19.x = (u_xlatb11) ? u_xlat21.x : u_xlat21.y;
        u_xlat26 = (-u_xlat24) + u_xlat26;
#ifdef UNITY_ADRENO_ES3
        { bool cond = u_xlat26<0.0; u_xlati26 = int(!!cond ? 0xFFFFFFFFu : uint(0)); }
#else
        u_xlati26 = int((u_xlat26<0.0) ? 0xFFFFFFFFu : uint(0));
#endif
        u_xlat24 = (-u_xlat24) + u_xlat19.x;
#ifdef UNITY_ADRENO_ES3
        { bool cond = u_xlat24<0.0; u_xlati24 = int(!!cond ? 0xFFFFFFFFu : uint(0)); }
#else
        u_xlati24 = int((u_xlat24<0.0) ? 0xFFFFFFFFu : uint(0));
#endif
#ifdef UNITY_ADRENO_ES3
        u_xlatb24 = !!(u_xlati24==u_xlati26);
#else
        u_xlatb24 = u_xlati24==u_xlati26;
#endif
        u_xlat24 = (u_xlatb24) ? 0.0 : u_xlat17;
        u_xlat17 = u_xlat25 + u_xlat3.x;
        u_xlat25 = (u_xlatb11) ? u_xlat25 : u_xlat3.x;
        u_xlat17 = -1.0 / u_xlat17;
        u_xlat17 = u_xlat25 * u_xlat17 + 0.5;
        u_xlat24 = u_xlat24 * u_xlat17;
        u_xlat17 = (u_xlatb9) ? 0.0 : u_xlat24;
        u_xlat3.x = u_xlat17 + vs_TEXCOORD0.x;
        u_xlat24 = u_xlatb9 ? u_xlat24 : float(0.0);
        u_xlat3.y = u_xlat24 + vs_TEXCOORD0.y;
        u_xlat9.xy = u_xlat3.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat16_9.xyz = textureLod(_MainTex, u_xlat9.xy, 0.0).xyz;
        u_xlat0.xyz = u_xlat0.xyz * vec3(0.111111112, 0.111111112, 0.111111112) + u_xlat16_9.xyz;
        u_xlat2.xyz = (-vec3(u_xlat1)) * u_xlat16_9.xyz + u_xlat0.xyz;
    //ENDIF
    }
    SV_Target0.xyz = u_xlat2.xyz;
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
#ifdef GL_EXT_shader_texture_lod
#extension GL_EXT_shader_texture_lod : enable
#endif

precision highp float;
precision highp int;
uniform 	mediump vec4 _MainTex_ST;
uniform 	vec4 _MainTex_TexelSize;
uniform mediump sampler2D _MainTex;
in highp vec2 vs_TEXCOORD0;
layout(location = 0) out highp vec4 SV_Target0;
vec4 u_xlat0;
mediump vec3 u_xlat16_0;
float u_xlat1;
mediump vec3 u_xlat16_1;
vec3 u_xlat2;
vec4 u_xlat3;
mediump vec3 u_xlat16_3;
vec2 u_xlat4;
mediump vec3 u_xlat16_4;
vec4 u_xlat5;
mediump vec3 u_xlat16_5;
mediump vec3 u_xlat16_6;
int u_xlati6;
vec3 u_xlat7;
mediump vec2 u_xlat16_7;
vec2 u_xlat9;
mediump vec3 u_xlat16_9;
bool u_xlatb9;
float u_xlat11;
bool u_xlatb11;
float u_xlat12;
float u_xlat13;
bool u_xlatb13;
int u_xlati14;
vec2 u_xlat15;
int u_xlati15;
bool u_xlatb15;
float u_xlat17;
vec2 u_xlat19;
bool u_xlatb19;
vec2 u_xlat20;
vec2 u_xlat21;
int u_xlati22;
float u_xlat24;
int u_xlati24;
bool u_xlatb24;
float u_xlat25;
float u_xlat26;
int u_xlati26;
float u_xlat27;
float u_xlat28;
float u_xlat30;
bool u_xlatb30;
void main()
{
    u_xlat0 = _MainTex_TexelSize.xyxy * vec4(0.0, -1.0, -1.0, 0.0) + vs_TEXCOORD0.xyxy;
    u_xlat0 = u_xlat0 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_1.xyz = textureLod(_MainTex, u_xlat0.xy, 0.0).xyz;
    u_xlat16_0.xyz = textureLod(_MainTex, u_xlat0.zw, 0.0).xyz;
    u_xlat2.xy = vs_TEXCOORD0.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat2.xyz = textureLod(_MainTex, u_xlat2.xy, 0.0).xyz;
    u_xlat3 = _MainTex_TexelSize.xyxy * vec4(1.0, 0.0, 0.0, 1.0) + vs_TEXCOORD0.xyxy;
    u_xlat3 = u_xlat3 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_4.xyz = textureLod(_MainTex, u_xlat3.xy, 0.0).xyz;
    u_xlat16_3.xyz = textureLod(_MainTex, u_xlat3.zw, 0.0).xyz;
    u_xlat24 = u_xlat16_1.y * 1.9632107 + u_xlat16_1.x;
    u_xlat25 = u_xlat16_0.y * 1.9632107 + u_xlat16_0.x;
    u_xlat26 = u_xlat2.y * 1.9632107 + u_xlat2.x;
    u_xlat27 = u_xlat16_4.y * 1.9632107 + u_xlat16_4.x;
    u_xlat28 = u_xlat16_3.y * 1.9632107 + u_xlat16_3.x;
    u_xlat5.x = min(u_xlat24, u_xlat25);
    u_xlat13 = min(u_xlat27, u_xlat28);
    u_xlat5.x = min(u_xlat13, u_xlat5.x);
    u_xlat5.x = min(u_xlat26, u_xlat5.x);
    u_xlat13 = max(u_xlat24, u_xlat25);
    u_xlat21.x = max(u_xlat27, u_xlat28);
    u_xlat13 = max(u_xlat21.x, u_xlat13);
    u_xlat13 = max(u_xlat26, u_xlat13);
    u_xlat5.x = (-u_xlat5.x) + u_xlat13;
    u_xlat13 = u_xlat13 * 0.125;
    u_xlat13 = max(u_xlat13, 0.0416666679);
#ifdef UNITY_ADRENO_ES3
    u_xlatb13 = !!(u_xlat5.x>=u_xlat13);
#else
    u_xlatb13 = u_xlat5.x>=u_xlat13;
#endif
    if(u_xlatb13){
        u_xlat0.xyz = u_xlat16_0.xyz + u_xlat16_1.xyz;
        u_xlat0.xyz = u_xlat2.xyz + u_xlat0.xyz;
        u_xlat0.xyz = u_xlat16_4.xyz + u_xlat0.xyz;
        u_xlat0.xyz = u_xlat16_3.xyz + u_xlat0.xyz;
        u_xlat1 = u_xlat24 + u_xlat25;
        u_xlat1 = u_xlat27 + u_xlat1;
        u_xlat1 = u_xlat28 + u_xlat1;
        u_xlat1 = u_xlat1 * 0.25 + (-u_xlat26);
        u_xlat1 = abs(u_xlat1) / u_xlat5.x;
        u_xlat1 = u_xlat1 + -0.25;
        u_xlat1 = max(u_xlat1, 0.0);
        u_xlat1 = u_xlat1 * 1.33333337;
        u_xlat1 = min(u_xlat1, 0.75);
        u_xlat9.xy = vs_TEXCOORD0.xy + (-_MainTex_TexelSize.xy);
        u_xlat9.xy = u_xlat9.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat16_3.xyz = textureLod(_MainTex, u_xlat9.xy, 0.0).xyz;
        u_xlat5 = _MainTex_TexelSize.xyxy * vec4(1.0, -1.0, -1.0, 1.0) + vs_TEXCOORD0.xyxy;
        u_xlat5 = u_xlat5 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
        u_xlat16_4.xyz = textureLod(_MainTex, u_xlat5.xy, 0.0).xyz;
        u_xlat16_5.xyz = textureLod(_MainTex, u_xlat5.zw, 0.0).xyz;
        u_xlat9.xy = vs_TEXCOORD0.xy + _MainTex_TexelSize.xy;
        u_xlat9.xy = u_xlat9.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat16_6.xyz = textureLod(_MainTex, u_xlat9.xy, 0.0).xyz;
        u_xlat7.xyz = u_xlat16_3.xyz + u_xlat16_4.xyz;
        u_xlat7.xyz = u_xlat16_5.xyz + u_xlat7.xyz;
        u_xlat7.xyz = u_xlat16_6.xyz + u_xlat7.xyz;
        u_xlat0.xyz = u_xlat0.xyz + u_xlat7.xyz;
        u_xlat0.xyz = vec3(u_xlat1) * u_xlat0.xyz;
        u_xlat9.x = u_xlat16_3.y * 1.9632107 + u_xlat16_3.x;
        u_xlat17 = u_xlat16_4.y * 1.9632107 + u_xlat16_4.x;
        u_xlat3.x = u_xlat16_5.y * 1.9632107 + u_xlat16_5.x;
        u_xlat11 = u_xlat16_6.y * 1.9632107 + u_xlat16_6.x;
        u_xlat19.x = u_xlat24 * -0.5;
        u_xlat19.x = u_xlat9.x * 0.25 + u_xlat19.x;
        u_xlat19.x = u_xlat17 * 0.25 + u_xlat19.x;
        u_xlat4.x = u_xlat25 * -0.5;
        u_xlat12 = u_xlat25 * 0.5 + (-u_xlat26);
        u_xlat20.x = u_xlat27 * -0.5;
        u_xlat12 = u_xlat27 * 0.5 + u_xlat12;
        u_xlat19.x = abs(u_xlat19.x) + abs(u_xlat12);
        u_xlat12 = u_xlat28 * -0.5;
        u_xlat12 = u_xlat3.x * 0.25 + u_xlat12;
        u_xlat12 = u_xlat11 * 0.25 + u_xlat12;
        u_xlat19.x = u_xlat19.x + abs(u_xlat12);
        u_xlat9.x = u_xlat9.x * 0.25 + u_xlat4.x;
        u_xlat9.x = u_xlat3.x * 0.25 + u_xlat9.x;
        u_xlat3.x = u_xlat24 * 0.5 + (-u_xlat26);
        u_xlat3.x = u_xlat28 * 0.5 + u_xlat3.x;
        u_xlat9.x = abs(u_xlat9.x) + abs(u_xlat3.x);
        u_xlat17 = u_xlat17 * 0.25 + u_xlat20.x;
        u_xlat17 = u_xlat11 * 0.25 + u_xlat17;
        u_xlat9.x = abs(u_xlat17) + u_xlat9.x;
#ifdef UNITY_ADRENO_ES3
        u_xlatb9 = !!(u_xlat9.x>=u_xlat19.x);
#else
        u_xlatb9 = u_xlat9.x>=u_xlat19.x;
#endif
        u_xlat17 = (u_xlatb9) ? (-_MainTex_TexelSize.y) : (-_MainTex_TexelSize.x);
        u_xlat24 = (u_xlatb9) ? u_xlat24 : u_xlat25;
        u_xlat25 = (u_xlatb9) ? u_xlat28 : u_xlat27;
        u_xlat3.x = (-u_xlat26) + u_xlat24;
        u_xlat11 = (-u_xlat26) + u_xlat25;
        u_xlat24 = u_xlat26 + u_xlat24;
        u_xlat24 = u_xlat24 * 0.5;
        u_xlat25 = u_xlat26 + u_xlat25;
        u_xlat25 = u_xlat25 * 0.5;
#ifdef UNITY_ADRENO_ES3
        u_xlatb19 = !!(abs(u_xlat3.x)>=abs(u_xlat11));
#else
        u_xlatb19 = abs(u_xlat3.x)>=abs(u_xlat11);
#endif
        u_xlat24 = (u_xlatb19) ? u_xlat24 : u_xlat25;
        u_xlat25 = (u_xlatb19) ? abs(u_xlat3.x) : abs(u_xlat11);
        u_xlat17 = (u_xlatb19) ? u_xlat17 : (-u_xlat17);
        u_xlat3.x = u_xlat17 * 0.5;
        u_xlat3.y = (u_xlatb9) ? 0.0 : u_xlat3.x;
        u_xlat3.x = u_xlatb9 ? u_xlat3.x : float(0.0);
        u_xlat4.xy = u_xlat3.yx + vs_TEXCOORD0.xy;
        u_xlat25 = u_xlat25 * 0.25;
        u_xlat3.y = float(0.0);
        u_xlat3.z = float(0.0);
        u_xlat3.xw = _MainTex_TexelSize.xy;
        u_xlat3.xy = (bool(u_xlatb9)) ? u_xlat3.xy : u_xlat3.zw;
        u_xlat19.xy = (-u_xlat3.xy) + u_xlat4.xy;
        u_xlat4.xy = u_xlat3.xy + u_xlat4.xy;
        u_xlat20.xy = u_xlat19.xy;
        u_xlat5.xy = u_xlat4.xy;
        u_xlat21.xy = vec2(u_xlat24);
        u_xlati6 = int(0);
        u_xlati14 = int(0);
        u_xlati22 = int(0);
        while(true){
#ifdef UNITY_ADRENO_ES3
            u_xlatb30 = !!(u_xlati22>=16);
#else
            u_xlatb30 = u_xlati22>=16;
#endif
            if(u_xlatb30){break;}
            if(u_xlati6 == 0) {
                u_xlat7.xy = u_xlat20.xy * _MainTex_ST.xy + _MainTex_ST.zw;
                u_xlat16_7.xy = textureLod(_MainTex, u_xlat7.xy, 0.0).xy;
                u_xlat30 = u_xlat16_7.y * 1.9632107 + u_xlat16_7.x;
            } else {
                u_xlat30 = u_xlat21.x;
            //ENDIF
            }
            if(u_xlati14 == 0) {
                u_xlat7.xy = u_xlat5.xy * _MainTex_ST.xy + _MainTex_ST.zw;
                u_xlat16_7.xy = textureLod(_MainTex, u_xlat7.xy, 0.0).xy;
                u_xlat7.x = u_xlat16_7.y * 1.9632107 + u_xlat16_7.x;
            } else {
                u_xlat7.x = u_xlat21.y;
            //ENDIF
            }
            u_xlat15.x = (-u_xlat24) + u_xlat30;
#ifdef UNITY_ADRENO_ES3
            u_xlatb15 = !!(abs(u_xlat15.x)>=u_xlat25);
#else
            u_xlatb15 = abs(u_xlat15.x)>=u_xlat25;
#endif
            u_xlati6 = int(uint(uint(u_xlati6) | (uint(u_xlatb15) * 0xffffffffu)));
            u_xlat15.x = (-u_xlat24) + u_xlat7.x;
#ifdef UNITY_ADRENO_ES3
            u_xlatb15 = !!(abs(u_xlat15.x)>=u_xlat25);
#else
            u_xlatb15 = abs(u_xlat15.x)>=u_xlat25;
#endif
            u_xlati14 = int(uint(uint(u_xlati14) | (uint(u_xlatb15) * 0xffffffffu)));
            u_xlati15 = int(uint(uint(u_xlati14) & uint(u_xlati6)));
            if(u_xlati15 != 0) {
                u_xlat21.x = u_xlat30;
                u_xlat21.y = u_xlat7.x;
                break;
            //ENDIF
            }
            u_xlat15.xy = (-u_xlat3.xy) + u_xlat20.xy;
            u_xlat20.xy = (int(u_xlati6) != 0) ? u_xlat20.xy : u_xlat15.xy;
            u_xlat15.xy = u_xlat3.xy + u_xlat5.xy;
            u_xlat5.xy = (int(u_xlati14) != 0) ? u_xlat5.xy : u_xlat15.xy;
            u_xlati22 = u_xlati22 + 1;
            u_xlat21.x = u_xlat30;
            u_xlat21.y = u_xlat7.x;
        }
        u_xlat3.xy = (-u_xlat20.xy) + vs_TEXCOORD0.xy;
        u_xlat25 = (u_xlatb9) ? u_xlat3.x : u_xlat3.y;
        u_xlat3.xy = u_xlat5.xy + (-vs_TEXCOORD0.xy);
        u_xlat3.x = (u_xlatb9) ? u_xlat3.x : u_xlat3.y;
#ifdef UNITY_ADRENO_ES3
        u_xlatb11 = !!(u_xlat25<u_xlat3.x);
#else
        u_xlatb11 = u_xlat25<u_xlat3.x;
#endif
        u_xlat19.x = (u_xlatb11) ? u_xlat21.x : u_xlat21.y;
        u_xlat26 = (-u_xlat24) + u_xlat26;
#ifdef UNITY_ADRENO_ES3
        { bool cond = u_xlat26<0.0; u_xlati26 = int(!!cond ? 0xFFFFFFFFu : uint(0)); }
#else
        u_xlati26 = int((u_xlat26<0.0) ? 0xFFFFFFFFu : uint(0));
#endif
        u_xlat24 = (-u_xlat24) + u_xlat19.x;
#ifdef UNITY_ADRENO_ES3
        { bool cond = u_xlat24<0.0; u_xlati24 = int(!!cond ? 0xFFFFFFFFu : uint(0)); }
#else
        u_xlati24 = int((u_xlat24<0.0) ? 0xFFFFFFFFu : uint(0));
#endif
#ifdef UNITY_ADRENO_ES3
        u_xlatb24 = !!(u_xlati24==u_xlati26);
#else
        u_xlatb24 = u_xlati24==u_xlati26;
#endif
        u_xlat24 = (u_xlatb24) ? 0.0 : u_xlat17;
        u_xlat17 = u_xlat25 + u_xlat3.x;
        u_xlat25 = (u_xlatb11) ? u_xlat25 : u_xlat3.x;
        u_xlat17 = -1.0 / u_xlat17;
        u_xlat17 = u_xlat25 * u_xlat17 + 0.5;
        u_xlat24 = u_xlat24 * u_xlat17;
        u_xlat17 = (u_xlatb9) ? 0.0 : u_xlat24;
        u_xlat3.x = u_xlat17 + vs_TEXCOORD0.x;
        u_xlat24 = u_xlatb9 ? u_xlat24 : float(0.0);
        u_xlat3.y = u_xlat24 + vs_TEXCOORD0.y;
        u_xlat9.xy = u_xlat3.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat16_9.xyz = textureLod(_MainTex, u_xlat9.xy, 0.0).xyz;
        u_xlat0.xyz = u_xlat0.xyz * vec3(0.111111112, 0.111111112, 0.111111112) + u_xlat16_9.xyz;
        u_xlat2.xyz = (-vec3(u_xlat1)) * u_xlat16_9.xyz + u_xlat0.xyz;
    //ENDIF
    }
    SV_Target0.xyz = u_xlat2.xyz;
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
#ifdef GL_EXT_shader_texture_lod
#extension GL_EXT_shader_texture_lod : enable
#endif

precision highp float;
precision highp int;
uniform 	mediump vec4 _MainTex_ST;
uniform 	vec4 _MainTex_TexelSize;
uniform mediump sampler2D _MainTex;
in highp vec2 vs_TEXCOORD0;
layout(location = 0) out highp vec4 SV_Target0;
vec4 u_xlat0;
mediump vec3 u_xlat16_0;
float u_xlat1;
mediump vec3 u_xlat16_1;
vec3 u_xlat2;
vec4 u_xlat3;
mediump vec3 u_xlat16_3;
vec2 u_xlat4;
mediump vec3 u_xlat16_4;
vec4 u_xlat5;
mediump vec3 u_xlat16_5;
mediump vec3 u_xlat16_6;
int u_xlati6;
vec3 u_xlat7;
mediump vec2 u_xlat16_7;
vec2 u_xlat9;
mediump vec3 u_xlat16_9;
bool u_xlatb9;
float u_xlat11;
bool u_xlatb11;
float u_xlat12;
float u_xlat13;
bool u_xlatb13;
int u_xlati14;
vec2 u_xlat15;
int u_xlati15;
bool u_xlatb15;
float u_xlat17;
vec2 u_xlat19;
bool u_xlatb19;
vec2 u_xlat20;
vec2 u_xlat21;
int u_xlati22;
float u_xlat24;
int u_xlati24;
bool u_xlatb24;
float u_xlat25;
float u_xlat26;
int u_xlati26;
float u_xlat27;
float u_xlat28;
float u_xlat30;
bool u_xlatb30;
void main()
{
    u_xlat0 = _MainTex_TexelSize.xyxy * vec4(0.0, -1.0, -1.0, 0.0) + vs_TEXCOORD0.xyxy;
    u_xlat0 = u_xlat0 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_1.xyz = textureLod(_MainTex, u_xlat0.xy, 0.0).xyz;
    u_xlat16_0.xyz = textureLod(_MainTex, u_xlat0.zw, 0.0).xyz;
    u_xlat2.xy = vs_TEXCOORD0.xy * _MainTex_ST.xy + _MainTex_ST.zw;
    u_xlat2.xyz = textureLod(_MainTex, u_xlat2.xy, 0.0).xyz;
    u_xlat3 = _MainTex_TexelSize.xyxy * vec4(1.0, 0.0, 0.0, 1.0) + vs_TEXCOORD0.xyxy;
    u_xlat3 = u_xlat3 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
    u_xlat16_4.xyz = textureLod(_MainTex, u_xlat3.xy, 0.0).xyz;
    u_xlat16_3.xyz = textureLod(_MainTex, u_xlat3.zw, 0.0).xyz;
    u_xlat24 = u_xlat16_1.y * 1.9632107 + u_xlat16_1.x;
    u_xlat25 = u_xlat16_0.y * 1.9632107 + u_xlat16_0.x;
    u_xlat26 = u_xlat2.y * 1.9632107 + u_xlat2.x;
    u_xlat27 = u_xlat16_4.y * 1.9632107 + u_xlat16_4.x;
    u_xlat28 = u_xlat16_3.y * 1.9632107 + u_xlat16_3.x;
    u_xlat5.x = min(u_xlat24, u_xlat25);
    u_xlat13 = min(u_xlat27, u_xlat28);
    u_xlat5.x = min(u_xlat13, u_xlat5.x);
    u_xlat5.x = min(u_xlat26, u_xlat5.x);
    u_xlat13 = max(u_xlat24, u_xlat25);
    u_xlat21.x = max(u_xlat27, u_xlat28);
    u_xlat13 = max(u_xlat21.x, u_xlat13);
    u_xlat13 = max(u_xlat26, u_xlat13);
    u_xlat5.x = (-u_xlat5.x) + u_xlat13;
    u_xlat13 = u_xlat13 * 0.125;
    u_xlat13 = max(u_xlat13, 0.0416666679);
#ifdef UNITY_ADRENO_ES3
    u_xlatb13 = !!(u_xlat5.x>=u_xlat13);
#else
    u_xlatb13 = u_xlat5.x>=u_xlat13;
#endif
    if(u_xlatb13){
        u_xlat0.xyz = u_xlat16_0.xyz + u_xlat16_1.xyz;
        u_xlat0.xyz = u_xlat2.xyz + u_xlat0.xyz;
        u_xlat0.xyz = u_xlat16_4.xyz + u_xlat0.xyz;
        u_xlat0.xyz = u_xlat16_3.xyz + u_xlat0.xyz;
        u_xlat1 = u_xlat24 + u_xlat25;
        u_xlat1 = u_xlat27 + u_xlat1;
        u_xlat1 = u_xlat28 + u_xlat1;
        u_xlat1 = u_xlat1 * 0.25 + (-u_xlat26);
        u_xlat1 = abs(u_xlat1) / u_xlat5.x;
        u_xlat1 = u_xlat1 + -0.25;
        u_xlat1 = max(u_xlat1, 0.0);
        u_xlat1 = u_xlat1 * 1.33333337;
        u_xlat1 = min(u_xlat1, 0.75);
        u_xlat9.xy = vs_TEXCOORD0.xy + (-_MainTex_TexelSize.xy);
        u_xlat9.xy = u_xlat9.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat16_3.xyz = textureLod(_MainTex, u_xlat9.xy, 0.0).xyz;
        u_xlat5 = _MainTex_TexelSize.xyxy * vec4(1.0, -1.0, -1.0, 1.0) + vs_TEXCOORD0.xyxy;
        u_xlat5 = u_xlat5 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
        u_xlat16_4.xyz = textureLod(_MainTex, u_xlat5.xy, 0.0).xyz;
        u_xlat16_5.xyz = textureLod(_MainTex, u_xlat5.zw, 0.0).xyz;
        u_xlat9.xy = vs_TEXCOORD0.xy + _MainTex_TexelSize.xy;
        u_xlat9.xy = u_xlat9.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat16_6.xyz = textureLod(_MainTex, u_xlat9.xy, 0.0).xyz;
        u_xlat7.xyz = u_xlat16_3.xyz + u_xlat16_4.xyz;
        u_xlat7.xyz = u_xlat16_5.xyz + u_xlat7.xyz;
        u_xlat7.xyz = u_xlat16_6.xyz + u_xlat7.xyz;
        u_xlat0.xyz = u_xlat0.xyz + u_xlat7.xyz;
        u_xlat0.xyz = vec3(u_xlat1) * u_xlat0.xyz;
        u_xlat9.x = u_xlat16_3.y * 1.9632107 + u_xlat16_3.x;
        u_xlat17 = u_xlat16_4.y * 1.9632107 + u_xlat16_4.x;
        u_xlat3.x = u_xlat16_5.y * 1.9632107 + u_xlat16_5.x;
        u_xlat11 = u_xlat16_6.y * 1.9632107 + u_xlat16_6.x;
        u_xlat19.x = u_xlat24 * -0.5;
        u_xlat19.x = u_xlat9.x * 0.25 + u_xlat19.x;
        u_xlat19.x = u_xlat17 * 0.25 + u_xlat19.x;
        u_xlat4.x = u_xlat25 * -0.5;
        u_xlat12 = u_xlat25 * 0.5 + (-u_xlat26);
        u_xlat20.x = u_xlat27 * -0.5;
        u_xlat12 = u_xlat27 * 0.5 + u_xlat12;
        u_xlat19.x = abs(u_xlat19.x) + abs(u_xlat12);
        u_xlat12 = u_xlat28 * -0.5;
        u_xlat12 = u_xlat3.x * 0.25 + u_xlat12;
        u_xlat12 = u_xlat11 * 0.25 + u_xlat12;
        u_xlat19.x = u_xlat19.x + abs(u_xlat12);
        u_xlat9.x = u_xlat9.x * 0.25 + u_xlat4.x;
        u_xlat9.x = u_xlat3.x * 0.25 + u_xlat9.x;
        u_xlat3.x = u_xlat24 * 0.5 + (-u_xlat26);
        u_xlat3.x = u_xlat28 * 0.5 + u_xlat3.x;
        u_xlat9.x = abs(u_xlat9.x) + abs(u_xlat3.x);
        u_xlat17 = u_xlat17 * 0.25 + u_xlat20.x;
        u_xlat17 = u_xlat11 * 0.25 + u_xlat17;
        u_xlat9.x = abs(u_xlat17) + u_xlat9.x;
#ifdef UNITY_ADRENO_ES3
        u_xlatb9 = !!(u_xlat9.x>=u_xlat19.x);
#else
        u_xlatb9 = u_xlat9.x>=u_xlat19.x;
#endif
        u_xlat17 = (u_xlatb9) ? (-_MainTex_TexelSize.y) : (-_MainTex_TexelSize.x);
        u_xlat24 = (u_xlatb9) ? u_xlat24 : u_xlat25;
        u_xlat25 = (u_xlatb9) ? u_xlat28 : u_xlat27;
        u_xlat3.x = (-u_xlat26) + u_xlat24;
        u_xlat11 = (-u_xlat26) + u_xlat25;
        u_xlat24 = u_xlat26 + u_xlat24;
        u_xlat24 = u_xlat24 * 0.5;
        u_xlat25 = u_xlat26 + u_xlat25;
        u_xlat25 = u_xlat25 * 0.5;
#ifdef UNITY_ADRENO_ES3
        u_xlatb19 = !!(abs(u_xlat3.x)>=abs(u_xlat11));
#else
        u_xlatb19 = abs(u_xlat3.x)>=abs(u_xlat11);
#endif
        u_xlat24 = (u_xlatb19) ? u_xlat24 : u_xlat25;
        u_xlat25 = (u_xlatb19) ? abs(u_xlat3.x) : abs(u_xlat11);
        u_xlat17 = (u_xlatb19) ? u_xlat17 : (-u_xlat17);
        u_xlat3.x = u_xlat17 * 0.5;
        u_xlat3.y = (u_xlatb9) ? 0.0 : u_xlat3.x;
        u_xlat3.x = u_xlatb9 ? u_xlat3.x : float(0.0);
        u_xlat4.xy = u_xlat3.yx + vs_TEXCOORD0.xy;
        u_xlat25 = u_xlat25 * 0.25;
        u_xlat3.y = float(0.0);
        u_xlat3.z = float(0.0);
        u_xlat3.xw = _MainTex_TexelSize.xy;
        u_xlat3.xy = (bool(u_xlatb9)) ? u_xlat3.xy : u_xlat3.zw;
        u_xlat19.xy = (-u_xlat3.xy) + u_xlat4.xy;
        u_xlat4.xy = u_xlat3.xy + u_xlat4.xy;
        u_xlat20.xy = u_xlat19.xy;
        u_xlat5.xy = u_xlat4.xy;
        u_xlat21.xy = vec2(u_xlat24);
        u_xlati6 = int(0);
        u_xlati14 = int(0);
        u_xlati22 = int(0);
        while(true){
#ifdef UNITY_ADRENO_ES3
            u_xlatb30 = !!(u_xlati22>=16);
#else
            u_xlatb30 = u_xlati22>=16;
#endif
            if(u_xlatb30){break;}
            if(u_xlati6 == 0) {
                u_xlat7.xy = u_xlat20.xy * _MainTex_ST.xy + _MainTex_ST.zw;
                u_xlat16_7.xy = textureLod(_MainTex, u_xlat7.xy, 0.0).xy;
                u_xlat30 = u_xlat16_7.y * 1.9632107 + u_xlat16_7.x;
            } else {
                u_xlat30 = u_xlat21.x;
            //ENDIF
            }
            if(u_xlati14 == 0) {
                u_xlat7.xy = u_xlat5.xy * _MainTex_ST.xy + _MainTex_ST.zw;
                u_xlat16_7.xy = textureLod(_MainTex, u_xlat7.xy, 0.0).xy;
                u_xlat7.x = u_xlat16_7.y * 1.9632107 + u_xlat16_7.x;
            } else {
                u_xlat7.x = u_xlat21.y;
            //ENDIF
            }
            u_xlat15.x = (-u_xlat24) + u_xlat30;
#ifdef UNITY_ADRENO_ES3
            u_xlatb15 = !!(abs(u_xlat15.x)>=u_xlat25);
#else
            u_xlatb15 = abs(u_xlat15.x)>=u_xlat25;
#endif
            u_xlati6 = int(uint(uint(u_xlati6) | (uint(u_xlatb15) * 0xffffffffu)));
            u_xlat15.x = (-u_xlat24) + u_xlat7.x;
#ifdef UNITY_ADRENO_ES3
            u_xlatb15 = !!(abs(u_xlat15.x)>=u_xlat25);
#else
            u_xlatb15 = abs(u_xlat15.x)>=u_xlat25;
#endif
            u_xlati14 = int(uint(uint(u_xlati14) | (uint(u_xlatb15) * 0xffffffffu)));
            u_xlati15 = int(uint(uint(u_xlati14) & uint(u_xlati6)));
            if(u_xlati15 != 0) {
                u_xlat21.x = u_xlat30;
                u_xlat21.y = u_xlat7.x;
                break;
            //ENDIF
            }
            u_xlat15.xy = (-u_xlat3.xy) + u_xlat20.xy;
            u_xlat20.xy = (int(u_xlati6) != 0) ? u_xlat20.xy : u_xlat15.xy;
            u_xlat15.xy = u_xlat3.xy + u_xlat5.xy;
            u_xlat5.xy = (int(u_xlati14) != 0) ? u_xlat5.xy : u_xlat15.xy;
            u_xlati22 = u_xlati22 + 1;
            u_xlat21.x = u_xlat30;
            u_xlat21.y = u_xlat7.x;
        }
        u_xlat3.xy = (-u_xlat20.xy) + vs_TEXCOORD0.xy;
        u_xlat25 = (u_xlatb9) ? u_xlat3.x : u_xlat3.y;
        u_xlat3.xy = u_xlat5.xy + (-vs_TEXCOORD0.xy);
        u_xlat3.x = (u_xlatb9) ? u_xlat3.x : u_xlat3.y;
#ifdef UNITY_ADRENO_ES3
        u_xlatb11 = !!(u_xlat25<u_xlat3.x);
#else
        u_xlatb11 = u_xlat25<u_xlat3.x;
#endif
        u_xlat19.x = (u_xlatb11) ? u_xlat21.x : u_xlat21.y;
        u_xlat26 = (-u_xlat24) + u_xlat26;
#ifdef UNITY_ADRENO_ES3
        { bool cond = u_xlat26<0.0; u_xlati26 = int(!!cond ? 0xFFFFFFFFu : uint(0)); }
#else
        u_xlati26 = int((u_xlat26<0.0) ? 0xFFFFFFFFu : uint(0));
#endif
        u_xlat24 = (-u_xlat24) + u_xlat19.x;
#ifdef UNITY_ADRENO_ES3
        { bool cond = u_xlat24<0.0; u_xlati24 = int(!!cond ? 0xFFFFFFFFu : uint(0)); }
#else
        u_xlati24 = int((u_xlat24<0.0) ? 0xFFFFFFFFu : uint(0));
#endif
#ifdef UNITY_ADRENO_ES3
        u_xlatb24 = !!(u_xlati24==u_xlati26);
#else
        u_xlatb24 = u_xlati24==u_xlati26;
#endif
        u_xlat24 = (u_xlatb24) ? 0.0 : u_xlat17;
        u_xlat17 = u_xlat25 + u_xlat3.x;
        u_xlat25 = (u_xlatb11) ? u_xlat25 : u_xlat3.x;
        u_xlat17 = -1.0 / u_xlat17;
        u_xlat17 = u_xlat25 * u_xlat17 + 0.5;
        u_xlat24 = u_xlat24 * u_xlat17;
        u_xlat17 = (u_xlatb9) ? 0.0 : u_xlat24;
        u_xlat3.x = u_xlat17 + vs_TEXCOORD0.x;
        u_xlat24 = u_xlatb9 ? u_xlat24 : float(0.0);
        u_xlat3.y = u_xlat24 + vs_TEXCOORD0.y;
        u_xlat9.xy = u_xlat3.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat16_9.xyz = textureLod(_MainTex, u_xlat9.xy, 0.0).xyz;
        u_xlat0.xyz = u_xlat0.xyz * vec3(0.111111112, 0.111111112, 0.111111112) + u_xlat16_9.xyz;
        u_xlat2.xyz = (-vec3(u_xlat1)) * u_xlat16_9.xyz + u_xlat0.xyz;
    //ENDIF
    }
    SV_Target0.xyz = u_xlat2.xyz;
    SV_Target0.w = 0.0;
    return;
}

#endif
"
}
}
Program "fp" {
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
Fallback "Hidden/FXAA II"
}