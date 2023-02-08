//////////////////////////////////////////
//
// NOTE: This is *not* a valid shader file
//
///////////////////////////////////////////
Shader "Hidden/FXAA Preset 2" {
Properties {
_MainTex ("Base (RGB)", 2D) = "white" { }
}
SubShader {
 Pass {
  ZTest Always
  ZWrite Off
  Cull Off
  GpuProgramID 31217
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
vec4 u_xlat4;
mediump vec3 u_xlat16_4;
vec4 u_xlat5;
mediump vec3 u_xlat16_5;
vec2 u_xlat6;
mediump vec3 u_xlat16_6;
vec3 u_xlat7;
int u_xlati7;
vec2 u_xlat8;
vec2 u_xlat10;
mediump vec3 u_xlat16_10;
bool u_xlatb10;
float u_xlat12;
bool u_xlatb12;
float u_xlat13;
float u_xlat14;
bool u_xlatb14;
vec2 u_xlat16;
mediump vec2 u_xlat16_16;
bool u_xlatb16;
float u_xlat19;
vec2 u_xlat21;
bool u_xlatb21;
float u_xlat22;
float u_xlat23;
int u_xlati24;
vec2 u_xlat25;
mediump vec2 u_xlat16_25;
float u_xlat27;
int u_xlati27;
bool u_xlatb27;
float u_xlat28;
float u_xlat29;
int u_xlati29;
float u_xlat30;
float u_xlat31;
int u_xlati33;
float u_xlat34;
int u_xlati34;
bool u_xlatb34;
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
    u_xlat27 = u_xlat16_1.y * 1.9632107 + u_xlat16_1.x;
    u_xlat28 = u_xlat16_0.y * 1.9632107 + u_xlat16_0.x;
    u_xlat29 = u_xlat2.y * 1.9632107 + u_xlat2.x;
    u_xlat30 = u_xlat16_4.y * 1.9632107 + u_xlat16_4.x;
    u_xlat31 = u_xlat16_3.y * 1.9632107 + u_xlat16_3.x;
    u_xlat5.x = min(u_xlat27, u_xlat28);
    u_xlat14 = min(u_xlat30, u_xlat31);
    u_xlat5.x = min(u_xlat14, u_xlat5.x);
    u_xlat5.x = min(u_xlat29, u_xlat5.x);
    u_xlat14 = max(u_xlat27, u_xlat28);
    u_xlat23 = max(u_xlat30, u_xlat31);
    u_xlat14 = max(u_xlat23, u_xlat14);
    u_xlat14 = max(u_xlat29, u_xlat14);
    u_xlat5.x = (-u_xlat5.x) + u_xlat14;
    u_xlat14 = u_xlat14 * 0.125;
    u_xlat14 = max(u_xlat14, 0.0416666679);
#ifdef UNITY_ADRENO_ES3
    u_xlatb14 = !!(u_xlat5.x>=u_xlat14);
#else
    u_xlatb14 = u_xlat5.x>=u_xlat14;
#endif
    if(u_xlatb14){
        u_xlat0.xyz = u_xlat16_0.xyz + u_xlat16_1.xyz;
        u_xlat0.xyz = u_xlat2.xyz + u_xlat0.xyz;
        u_xlat0.xyz = u_xlat16_4.xyz + u_xlat0.xyz;
        u_xlat0.xyz = u_xlat16_3.xyz + u_xlat0.xyz;
        u_xlat1 = u_xlat27 + u_xlat28;
        u_xlat1 = u_xlat30 + u_xlat1;
        u_xlat1 = u_xlat31 + u_xlat1;
        u_xlat1 = u_xlat1 * 0.25 + (-u_xlat29);
        u_xlat1 = abs(u_xlat1) / u_xlat5.x;
        u_xlat1 = u_xlat1 + -0.25;
        u_xlat1 = max(u_xlat1, 0.0);
        u_xlat1 = u_xlat1 * 1.33333337;
        u_xlat1 = min(u_xlat1, 0.75);
        u_xlat10.xy = vs_TEXCOORD0.xy + (-_MainTex_TexelSize.xy);
        u_xlat10.xy = u_xlat10.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat16_3.xyz = textureLod(_MainTex, u_xlat10.xy, 0.0).xyz;
        u_xlat5 = _MainTex_TexelSize.xyxy * vec4(1.0, -1.0, -1.0, 1.0) + vs_TEXCOORD0.xyxy;
        u_xlat5 = u_xlat5 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
        u_xlat16_4.xyz = textureLod(_MainTex, u_xlat5.xy, 0.0).xyz;
        u_xlat16_5.xyz = textureLod(_MainTex, u_xlat5.zw, 0.0).xyz;
        u_xlat10.xy = vs_TEXCOORD0.xy + _MainTex_TexelSize.xy;
        u_xlat10.xy = u_xlat10.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat16_6.xyz = textureLod(_MainTex, u_xlat10.xy, 0.0).xyz;
        u_xlat7.xyz = u_xlat16_3.xyz + u_xlat16_4.xyz;
        u_xlat7.xyz = u_xlat16_5.xyz + u_xlat7.xyz;
        u_xlat7.xyz = u_xlat16_6.xyz + u_xlat7.xyz;
        u_xlat0.xyz = u_xlat0.xyz + u_xlat7.xyz;
        u_xlat0.xyz = vec3(u_xlat1) * u_xlat0.xyz;
        u_xlat10.x = u_xlat16_3.y * 1.9632107 + u_xlat16_3.x;
        u_xlat19 = u_xlat16_4.y * 1.9632107 + u_xlat16_4.x;
        u_xlat3.x = u_xlat16_5.y * 1.9632107 + u_xlat16_5.x;
        u_xlat12 = u_xlat16_6.y * 1.9632107 + u_xlat16_6.x;
        u_xlat21.x = u_xlat27 * -0.5;
        u_xlat21.x = u_xlat10.x * 0.25 + u_xlat21.x;
        u_xlat21.x = u_xlat19 * 0.25 + u_xlat21.x;
        u_xlat4.x = u_xlat28 * -0.5;
        u_xlat13 = u_xlat28 * 0.5 + (-u_xlat29);
        u_xlat22 = u_xlat30 * -0.5;
        u_xlat13 = u_xlat30 * 0.5 + u_xlat13;
        u_xlat21.x = abs(u_xlat21.x) + abs(u_xlat13);
        u_xlat13 = u_xlat31 * -0.5;
        u_xlat13 = u_xlat3.x * 0.25 + u_xlat13;
        u_xlat13 = u_xlat12 * 0.25 + u_xlat13;
        u_xlat21.x = u_xlat21.x + abs(u_xlat13);
        u_xlat10.x = u_xlat10.x * 0.25 + u_xlat4.x;
        u_xlat10.x = u_xlat3.x * 0.25 + u_xlat10.x;
        u_xlat3.x = u_xlat27 * 0.5 + (-u_xlat29);
        u_xlat3.x = u_xlat31 * 0.5 + u_xlat3.x;
        u_xlat10.x = abs(u_xlat10.x) + abs(u_xlat3.x);
        u_xlat19 = u_xlat19 * 0.25 + u_xlat22;
        u_xlat19 = u_xlat12 * 0.25 + u_xlat19;
        u_xlat10.x = abs(u_xlat19) + u_xlat10.x;
#ifdef UNITY_ADRENO_ES3
        u_xlatb10 = !!(u_xlat10.x>=u_xlat21.x);
#else
        u_xlatb10 = u_xlat10.x>=u_xlat21.x;
#endif
        u_xlat19 = (u_xlatb10) ? (-_MainTex_TexelSize.y) : (-_MainTex_TexelSize.x);
        u_xlat27 = (u_xlatb10) ? u_xlat27 : u_xlat28;
        u_xlat28 = (u_xlatb10) ? u_xlat31 : u_xlat30;
        u_xlat3.x = (-u_xlat29) + u_xlat27;
        u_xlat12 = (-u_xlat29) + u_xlat28;
        u_xlat27 = u_xlat29 + u_xlat27;
        u_xlat27 = u_xlat27 * 0.5;
        u_xlat28 = u_xlat29 + u_xlat28;
        u_xlat28 = u_xlat28 * 0.5;
#ifdef UNITY_ADRENO_ES3
        u_xlatb21 = !!(abs(u_xlat3.x)>=abs(u_xlat12));
#else
        u_xlatb21 = abs(u_xlat3.x)>=abs(u_xlat12);
#endif
        u_xlat27 = (u_xlatb21) ? u_xlat27 : u_xlat28;
        u_xlat28 = (u_xlatb21) ? abs(u_xlat3.x) : abs(u_xlat12);
        u_xlat19 = (u_xlatb21) ? u_xlat19 : (-u_xlat19);
        u_xlat3.x = u_xlat19 * 0.5;
        u_xlat3.y = (u_xlatb10) ? 0.0 : u_xlat3.x;
        u_xlat3.x = u_xlatb10 ? u_xlat3.x : float(0.0);
        u_xlat4.xy = u_xlat3.yx + vs_TEXCOORD0.xy;
        u_xlat28 = u_xlat28 * 0.25;
        u_xlat3.y = float(0.0);
        u_xlat3.z = float(0.0);
        u_xlat3.xw = _MainTex_TexelSize.xy;
        u_xlat3.xy = (bool(u_xlatb10)) ? u_xlat3.xy : u_xlat3.zw;
        u_xlat4 = u_xlat3.xyxy * vec4(-1.5, -1.5, 1.5, 1.5) + u_xlat4.xyxy;
        u_xlat21.xy = u_xlat3.xy + u_xlat3.xy;
        u_xlat5 = u_xlat4;
        u_xlat6.xy = vec2(u_xlat27);
        u_xlati24 = int(0);
        u_xlati33 = int(0);
        u_xlati7 = 0;
        while(true){
#ifdef UNITY_ADRENO_ES3
            u_xlatb16 = !!(u_xlati7>=8);
#else
            u_xlatb16 = u_xlati7>=8;
#endif
            if(u_xlatb16){break;}
            if(u_xlati24 == 0) {
                u_xlat16.xy = u_xlat5.xy * _MainTex_ST.xy + _MainTex_ST.zw;
                u_xlat16_16.xy = textureGrad(_MainTex, u_xlat16.xy, vec4(u_xlat21.xyxx).xy, vec4(u_xlat21.xyxx).xy).xy;
                u_xlat16.x = u_xlat16_16.y * 1.9632107 + u_xlat16_16.x;
            } else {
                u_xlat16.x = u_xlat6.x;
            //ENDIF
            }
            if(u_xlati33 == 0) {
                u_xlat25.xy = u_xlat5.zw * _MainTex_ST.xy + _MainTex_ST.zw;
                u_xlat16_25.xy = textureGrad(_MainTex, u_xlat25.xy, vec4(u_xlat21.xyxx).xy, vec4(u_xlat21.xyxx).xy).xy;
                u_xlat16.y = u_xlat16_25.y * 1.9632107 + u_xlat16_25.x;
            } else {
                u_xlat16.y = u_xlat6.y;
            //ENDIF
            }
            u_xlat34 = (-u_xlat27) + u_xlat16.x;
#ifdef UNITY_ADRENO_ES3
            u_xlatb34 = !!(abs(u_xlat34)>=u_xlat28);
#else
            u_xlatb34 = abs(u_xlat34)>=u_xlat28;
#endif
            u_xlati24 = int(uint(uint(u_xlati24) | (uint(u_xlatb34) * 0xffffffffu)));
            u_xlat34 = (-u_xlat27) + u_xlat16.y;
#ifdef UNITY_ADRENO_ES3
            u_xlatb34 = !!(abs(u_xlat34)>=u_xlat28);
#else
            u_xlatb34 = abs(u_xlat34)>=u_xlat28;
#endif
            u_xlati33 = int(uint(uint(u_xlati33) | (uint(u_xlatb34) * 0xffffffffu)));
            u_xlati34 = int(uint(uint(u_xlati33) & uint(u_xlati24)));
            if(u_xlati34 != 0) {
                u_xlat6.xy = u_xlat16.xy;
                break;
            //ENDIF
            }
            u_xlat8.xy = (-u_xlat3.xy) * vec2(2.0, 2.0) + u_xlat5.xy;
            u_xlat5.xy = (int(u_xlati24) != 0) ? u_xlat5.xy : u_xlat8.xy;
            u_xlat8.xy = u_xlat3.xy * vec2(2.0, 2.0) + u_xlat5.zw;
            u_xlat5.zw = (int(u_xlati33) != 0) ? u_xlat5.zw : u_xlat8.xy;
            u_xlati7 = u_xlati7 + 1;
            u_xlat6.xy = u_xlat16.xy;
        }
        u_xlat3.xy = (-u_xlat5.xy) + vs_TEXCOORD0.xy;
        u_xlat28 = (u_xlatb10) ? u_xlat3.x : u_xlat3.y;
        u_xlat3.xy = u_xlat5.zw + (-vs_TEXCOORD0.xy);
        u_xlat3.x = (u_xlatb10) ? u_xlat3.x : u_xlat3.y;
#ifdef UNITY_ADRENO_ES3
        u_xlatb12 = !!(u_xlat28<u_xlat3.x);
#else
        u_xlatb12 = u_xlat28<u_xlat3.x;
#endif
        u_xlat21.x = (u_xlatb12) ? u_xlat6.x : u_xlat6.y;
        u_xlat29 = (-u_xlat27) + u_xlat29;
#ifdef UNITY_ADRENO_ES3
        { bool cond = u_xlat29<0.0; u_xlati29 = int(!!cond ? 0xFFFFFFFFu : uint(0)); }
#else
        u_xlati29 = int((u_xlat29<0.0) ? 0xFFFFFFFFu : uint(0));
#endif
        u_xlat27 = (-u_xlat27) + u_xlat21.x;
#ifdef UNITY_ADRENO_ES3
        { bool cond = u_xlat27<0.0; u_xlati27 = int(!!cond ? 0xFFFFFFFFu : uint(0)); }
#else
        u_xlati27 = int((u_xlat27<0.0) ? 0xFFFFFFFFu : uint(0));
#endif
#ifdef UNITY_ADRENO_ES3
        u_xlatb27 = !!(u_xlati27==u_xlati29);
#else
        u_xlatb27 = u_xlati27==u_xlati29;
#endif
        u_xlat27 = (u_xlatb27) ? 0.0 : u_xlat19;
        u_xlat19 = u_xlat28 + u_xlat3.x;
        u_xlat28 = (u_xlatb12) ? u_xlat28 : u_xlat3.x;
        u_xlat19 = -1.0 / u_xlat19;
        u_xlat19 = u_xlat28 * u_xlat19 + 0.5;
        u_xlat27 = u_xlat27 * u_xlat19;
        u_xlat19 = (u_xlatb10) ? 0.0 : u_xlat27;
        u_xlat3.x = u_xlat19 + vs_TEXCOORD0.x;
        u_xlat27 = u_xlatb10 ? u_xlat27 : float(0.0);
        u_xlat3.y = u_xlat27 + vs_TEXCOORD0.y;
        u_xlat10.xy = u_xlat3.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat16_10.xyz = textureLod(_MainTex, u_xlat10.xy, 0.0).xyz;
        u_xlat0.xyz = u_xlat0.xyz * vec3(0.111111112, 0.111111112, 0.111111112) + u_xlat16_10.xyz;
        u_xlat2.xyz = (-vec3(u_xlat1)) * u_xlat16_10.xyz + u_xlat0.xyz;
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
vec4 u_xlat4;
mediump vec3 u_xlat16_4;
vec4 u_xlat5;
mediump vec3 u_xlat16_5;
vec2 u_xlat6;
mediump vec3 u_xlat16_6;
vec3 u_xlat7;
int u_xlati7;
vec2 u_xlat8;
vec2 u_xlat10;
mediump vec3 u_xlat16_10;
bool u_xlatb10;
float u_xlat12;
bool u_xlatb12;
float u_xlat13;
float u_xlat14;
bool u_xlatb14;
vec2 u_xlat16;
mediump vec2 u_xlat16_16;
bool u_xlatb16;
float u_xlat19;
vec2 u_xlat21;
bool u_xlatb21;
float u_xlat22;
float u_xlat23;
int u_xlati24;
vec2 u_xlat25;
mediump vec2 u_xlat16_25;
float u_xlat27;
int u_xlati27;
bool u_xlatb27;
float u_xlat28;
float u_xlat29;
int u_xlati29;
float u_xlat30;
float u_xlat31;
int u_xlati33;
float u_xlat34;
int u_xlati34;
bool u_xlatb34;
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
    u_xlat27 = u_xlat16_1.y * 1.9632107 + u_xlat16_1.x;
    u_xlat28 = u_xlat16_0.y * 1.9632107 + u_xlat16_0.x;
    u_xlat29 = u_xlat2.y * 1.9632107 + u_xlat2.x;
    u_xlat30 = u_xlat16_4.y * 1.9632107 + u_xlat16_4.x;
    u_xlat31 = u_xlat16_3.y * 1.9632107 + u_xlat16_3.x;
    u_xlat5.x = min(u_xlat27, u_xlat28);
    u_xlat14 = min(u_xlat30, u_xlat31);
    u_xlat5.x = min(u_xlat14, u_xlat5.x);
    u_xlat5.x = min(u_xlat29, u_xlat5.x);
    u_xlat14 = max(u_xlat27, u_xlat28);
    u_xlat23 = max(u_xlat30, u_xlat31);
    u_xlat14 = max(u_xlat23, u_xlat14);
    u_xlat14 = max(u_xlat29, u_xlat14);
    u_xlat5.x = (-u_xlat5.x) + u_xlat14;
    u_xlat14 = u_xlat14 * 0.125;
    u_xlat14 = max(u_xlat14, 0.0416666679);
#ifdef UNITY_ADRENO_ES3
    u_xlatb14 = !!(u_xlat5.x>=u_xlat14);
#else
    u_xlatb14 = u_xlat5.x>=u_xlat14;
#endif
    if(u_xlatb14){
        u_xlat0.xyz = u_xlat16_0.xyz + u_xlat16_1.xyz;
        u_xlat0.xyz = u_xlat2.xyz + u_xlat0.xyz;
        u_xlat0.xyz = u_xlat16_4.xyz + u_xlat0.xyz;
        u_xlat0.xyz = u_xlat16_3.xyz + u_xlat0.xyz;
        u_xlat1 = u_xlat27 + u_xlat28;
        u_xlat1 = u_xlat30 + u_xlat1;
        u_xlat1 = u_xlat31 + u_xlat1;
        u_xlat1 = u_xlat1 * 0.25 + (-u_xlat29);
        u_xlat1 = abs(u_xlat1) / u_xlat5.x;
        u_xlat1 = u_xlat1 + -0.25;
        u_xlat1 = max(u_xlat1, 0.0);
        u_xlat1 = u_xlat1 * 1.33333337;
        u_xlat1 = min(u_xlat1, 0.75);
        u_xlat10.xy = vs_TEXCOORD0.xy + (-_MainTex_TexelSize.xy);
        u_xlat10.xy = u_xlat10.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat16_3.xyz = textureLod(_MainTex, u_xlat10.xy, 0.0).xyz;
        u_xlat5 = _MainTex_TexelSize.xyxy * vec4(1.0, -1.0, -1.0, 1.0) + vs_TEXCOORD0.xyxy;
        u_xlat5 = u_xlat5 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
        u_xlat16_4.xyz = textureLod(_MainTex, u_xlat5.xy, 0.0).xyz;
        u_xlat16_5.xyz = textureLod(_MainTex, u_xlat5.zw, 0.0).xyz;
        u_xlat10.xy = vs_TEXCOORD0.xy + _MainTex_TexelSize.xy;
        u_xlat10.xy = u_xlat10.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat16_6.xyz = textureLod(_MainTex, u_xlat10.xy, 0.0).xyz;
        u_xlat7.xyz = u_xlat16_3.xyz + u_xlat16_4.xyz;
        u_xlat7.xyz = u_xlat16_5.xyz + u_xlat7.xyz;
        u_xlat7.xyz = u_xlat16_6.xyz + u_xlat7.xyz;
        u_xlat0.xyz = u_xlat0.xyz + u_xlat7.xyz;
        u_xlat0.xyz = vec3(u_xlat1) * u_xlat0.xyz;
        u_xlat10.x = u_xlat16_3.y * 1.9632107 + u_xlat16_3.x;
        u_xlat19 = u_xlat16_4.y * 1.9632107 + u_xlat16_4.x;
        u_xlat3.x = u_xlat16_5.y * 1.9632107 + u_xlat16_5.x;
        u_xlat12 = u_xlat16_6.y * 1.9632107 + u_xlat16_6.x;
        u_xlat21.x = u_xlat27 * -0.5;
        u_xlat21.x = u_xlat10.x * 0.25 + u_xlat21.x;
        u_xlat21.x = u_xlat19 * 0.25 + u_xlat21.x;
        u_xlat4.x = u_xlat28 * -0.5;
        u_xlat13 = u_xlat28 * 0.5 + (-u_xlat29);
        u_xlat22 = u_xlat30 * -0.5;
        u_xlat13 = u_xlat30 * 0.5 + u_xlat13;
        u_xlat21.x = abs(u_xlat21.x) + abs(u_xlat13);
        u_xlat13 = u_xlat31 * -0.5;
        u_xlat13 = u_xlat3.x * 0.25 + u_xlat13;
        u_xlat13 = u_xlat12 * 0.25 + u_xlat13;
        u_xlat21.x = u_xlat21.x + abs(u_xlat13);
        u_xlat10.x = u_xlat10.x * 0.25 + u_xlat4.x;
        u_xlat10.x = u_xlat3.x * 0.25 + u_xlat10.x;
        u_xlat3.x = u_xlat27 * 0.5 + (-u_xlat29);
        u_xlat3.x = u_xlat31 * 0.5 + u_xlat3.x;
        u_xlat10.x = abs(u_xlat10.x) + abs(u_xlat3.x);
        u_xlat19 = u_xlat19 * 0.25 + u_xlat22;
        u_xlat19 = u_xlat12 * 0.25 + u_xlat19;
        u_xlat10.x = abs(u_xlat19) + u_xlat10.x;
#ifdef UNITY_ADRENO_ES3
        u_xlatb10 = !!(u_xlat10.x>=u_xlat21.x);
#else
        u_xlatb10 = u_xlat10.x>=u_xlat21.x;
#endif
        u_xlat19 = (u_xlatb10) ? (-_MainTex_TexelSize.y) : (-_MainTex_TexelSize.x);
        u_xlat27 = (u_xlatb10) ? u_xlat27 : u_xlat28;
        u_xlat28 = (u_xlatb10) ? u_xlat31 : u_xlat30;
        u_xlat3.x = (-u_xlat29) + u_xlat27;
        u_xlat12 = (-u_xlat29) + u_xlat28;
        u_xlat27 = u_xlat29 + u_xlat27;
        u_xlat27 = u_xlat27 * 0.5;
        u_xlat28 = u_xlat29 + u_xlat28;
        u_xlat28 = u_xlat28 * 0.5;
#ifdef UNITY_ADRENO_ES3
        u_xlatb21 = !!(abs(u_xlat3.x)>=abs(u_xlat12));
#else
        u_xlatb21 = abs(u_xlat3.x)>=abs(u_xlat12);
#endif
        u_xlat27 = (u_xlatb21) ? u_xlat27 : u_xlat28;
        u_xlat28 = (u_xlatb21) ? abs(u_xlat3.x) : abs(u_xlat12);
        u_xlat19 = (u_xlatb21) ? u_xlat19 : (-u_xlat19);
        u_xlat3.x = u_xlat19 * 0.5;
        u_xlat3.y = (u_xlatb10) ? 0.0 : u_xlat3.x;
        u_xlat3.x = u_xlatb10 ? u_xlat3.x : float(0.0);
        u_xlat4.xy = u_xlat3.yx + vs_TEXCOORD0.xy;
        u_xlat28 = u_xlat28 * 0.25;
        u_xlat3.y = float(0.0);
        u_xlat3.z = float(0.0);
        u_xlat3.xw = _MainTex_TexelSize.xy;
        u_xlat3.xy = (bool(u_xlatb10)) ? u_xlat3.xy : u_xlat3.zw;
        u_xlat4 = u_xlat3.xyxy * vec4(-1.5, -1.5, 1.5, 1.5) + u_xlat4.xyxy;
        u_xlat21.xy = u_xlat3.xy + u_xlat3.xy;
        u_xlat5 = u_xlat4;
        u_xlat6.xy = vec2(u_xlat27);
        u_xlati24 = int(0);
        u_xlati33 = int(0);
        u_xlati7 = 0;
        while(true){
#ifdef UNITY_ADRENO_ES3
            u_xlatb16 = !!(u_xlati7>=8);
#else
            u_xlatb16 = u_xlati7>=8;
#endif
            if(u_xlatb16){break;}
            if(u_xlati24 == 0) {
                u_xlat16.xy = u_xlat5.xy * _MainTex_ST.xy + _MainTex_ST.zw;
                u_xlat16_16.xy = textureGrad(_MainTex, u_xlat16.xy, vec4(u_xlat21.xyxx).xy, vec4(u_xlat21.xyxx).xy).xy;
                u_xlat16.x = u_xlat16_16.y * 1.9632107 + u_xlat16_16.x;
            } else {
                u_xlat16.x = u_xlat6.x;
            //ENDIF
            }
            if(u_xlati33 == 0) {
                u_xlat25.xy = u_xlat5.zw * _MainTex_ST.xy + _MainTex_ST.zw;
                u_xlat16_25.xy = textureGrad(_MainTex, u_xlat25.xy, vec4(u_xlat21.xyxx).xy, vec4(u_xlat21.xyxx).xy).xy;
                u_xlat16.y = u_xlat16_25.y * 1.9632107 + u_xlat16_25.x;
            } else {
                u_xlat16.y = u_xlat6.y;
            //ENDIF
            }
            u_xlat34 = (-u_xlat27) + u_xlat16.x;
#ifdef UNITY_ADRENO_ES3
            u_xlatb34 = !!(abs(u_xlat34)>=u_xlat28);
#else
            u_xlatb34 = abs(u_xlat34)>=u_xlat28;
#endif
            u_xlati24 = int(uint(uint(u_xlati24) | (uint(u_xlatb34) * 0xffffffffu)));
            u_xlat34 = (-u_xlat27) + u_xlat16.y;
#ifdef UNITY_ADRENO_ES3
            u_xlatb34 = !!(abs(u_xlat34)>=u_xlat28);
#else
            u_xlatb34 = abs(u_xlat34)>=u_xlat28;
#endif
            u_xlati33 = int(uint(uint(u_xlati33) | (uint(u_xlatb34) * 0xffffffffu)));
            u_xlati34 = int(uint(uint(u_xlati33) & uint(u_xlati24)));
            if(u_xlati34 != 0) {
                u_xlat6.xy = u_xlat16.xy;
                break;
            //ENDIF
            }
            u_xlat8.xy = (-u_xlat3.xy) * vec2(2.0, 2.0) + u_xlat5.xy;
            u_xlat5.xy = (int(u_xlati24) != 0) ? u_xlat5.xy : u_xlat8.xy;
            u_xlat8.xy = u_xlat3.xy * vec2(2.0, 2.0) + u_xlat5.zw;
            u_xlat5.zw = (int(u_xlati33) != 0) ? u_xlat5.zw : u_xlat8.xy;
            u_xlati7 = u_xlati7 + 1;
            u_xlat6.xy = u_xlat16.xy;
        }
        u_xlat3.xy = (-u_xlat5.xy) + vs_TEXCOORD0.xy;
        u_xlat28 = (u_xlatb10) ? u_xlat3.x : u_xlat3.y;
        u_xlat3.xy = u_xlat5.zw + (-vs_TEXCOORD0.xy);
        u_xlat3.x = (u_xlatb10) ? u_xlat3.x : u_xlat3.y;
#ifdef UNITY_ADRENO_ES3
        u_xlatb12 = !!(u_xlat28<u_xlat3.x);
#else
        u_xlatb12 = u_xlat28<u_xlat3.x;
#endif
        u_xlat21.x = (u_xlatb12) ? u_xlat6.x : u_xlat6.y;
        u_xlat29 = (-u_xlat27) + u_xlat29;
#ifdef UNITY_ADRENO_ES3
        { bool cond = u_xlat29<0.0; u_xlati29 = int(!!cond ? 0xFFFFFFFFu : uint(0)); }
#else
        u_xlati29 = int((u_xlat29<0.0) ? 0xFFFFFFFFu : uint(0));
#endif
        u_xlat27 = (-u_xlat27) + u_xlat21.x;
#ifdef UNITY_ADRENO_ES3
        { bool cond = u_xlat27<0.0; u_xlati27 = int(!!cond ? 0xFFFFFFFFu : uint(0)); }
#else
        u_xlati27 = int((u_xlat27<0.0) ? 0xFFFFFFFFu : uint(0));
#endif
#ifdef UNITY_ADRENO_ES3
        u_xlatb27 = !!(u_xlati27==u_xlati29);
#else
        u_xlatb27 = u_xlati27==u_xlati29;
#endif
        u_xlat27 = (u_xlatb27) ? 0.0 : u_xlat19;
        u_xlat19 = u_xlat28 + u_xlat3.x;
        u_xlat28 = (u_xlatb12) ? u_xlat28 : u_xlat3.x;
        u_xlat19 = -1.0 / u_xlat19;
        u_xlat19 = u_xlat28 * u_xlat19 + 0.5;
        u_xlat27 = u_xlat27 * u_xlat19;
        u_xlat19 = (u_xlatb10) ? 0.0 : u_xlat27;
        u_xlat3.x = u_xlat19 + vs_TEXCOORD0.x;
        u_xlat27 = u_xlatb10 ? u_xlat27 : float(0.0);
        u_xlat3.y = u_xlat27 + vs_TEXCOORD0.y;
        u_xlat10.xy = u_xlat3.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat16_10.xyz = textureLod(_MainTex, u_xlat10.xy, 0.0).xyz;
        u_xlat0.xyz = u_xlat0.xyz * vec3(0.111111112, 0.111111112, 0.111111112) + u_xlat16_10.xyz;
        u_xlat2.xyz = (-vec3(u_xlat1)) * u_xlat16_10.xyz + u_xlat0.xyz;
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
vec4 u_xlat4;
mediump vec3 u_xlat16_4;
vec4 u_xlat5;
mediump vec3 u_xlat16_5;
vec2 u_xlat6;
mediump vec3 u_xlat16_6;
vec3 u_xlat7;
int u_xlati7;
vec2 u_xlat8;
vec2 u_xlat10;
mediump vec3 u_xlat16_10;
bool u_xlatb10;
float u_xlat12;
bool u_xlatb12;
float u_xlat13;
float u_xlat14;
bool u_xlatb14;
vec2 u_xlat16;
mediump vec2 u_xlat16_16;
bool u_xlatb16;
float u_xlat19;
vec2 u_xlat21;
bool u_xlatb21;
float u_xlat22;
float u_xlat23;
int u_xlati24;
vec2 u_xlat25;
mediump vec2 u_xlat16_25;
float u_xlat27;
int u_xlati27;
bool u_xlatb27;
float u_xlat28;
float u_xlat29;
int u_xlati29;
float u_xlat30;
float u_xlat31;
int u_xlati33;
float u_xlat34;
int u_xlati34;
bool u_xlatb34;
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
    u_xlat27 = u_xlat16_1.y * 1.9632107 + u_xlat16_1.x;
    u_xlat28 = u_xlat16_0.y * 1.9632107 + u_xlat16_0.x;
    u_xlat29 = u_xlat2.y * 1.9632107 + u_xlat2.x;
    u_xlat30 = u_xlat16_4.y * 1.9632107 + u_xlat16_4.x;
    u_xlat31 = u_xlat16_3.y * 1.9632107 + u_xlat16_3.x;
    u_xlat5.x = min(u_xlat27, u_xlat28);
    u_xlat14 = min(u_xlat30, u_xlat31);
    u_xlat5.x = min(u_xlat14, u_xlat5.x);
    u_xlat5.x = min(u_xlat29, u_xlat5.x);
    u_xlat14 = max(u_xlat27, u_xlat28);
    u_xlat23 = max(u_xlat30, u_xlat31);
    u_xlat14 = max(u_xlat23, u_xlat14);
    u_xlat14 = max(u_xlat29, u_xlat14);
    u_xlat5.x = (-u_xlat5.x) + u_xlat14;
    u_xlat14 = u_xlat14 * 0.125;
    u_xlat14 = max(u_xlat14, 0.0416666679);
#ifdef UNITY_ADRENO_ES3
    u_xlatb14 = !!(u_xlat5.x>=u_xlat14);
#else
    u_xlatb14 = u_xlat5.x>=u_xlat14;
#endif
    if(u_xlatb14){
        u_xlat0.xyz = u_xlat16_0.xyz + u_xlat16_1.xyz;
        u_xlat0.xyz = u_xlat2.xyz + u_xlat0.xyz;
        u_xlat0.xyz = u_xlat16_4.xyz + u_xlat0.xyz;
        u_xlat0.xyz = u_xlat16_3.xyz + u_xlat0.xyz;
        u_xlat1 = u_xlat27 + u_xlat28;
        u_xlat1 = u_xlat30 + u_xlat1;
        u_xlat1 = u_xlat31 + u_xlat1;
        u_xlat1 = u_xlat1 * 0.25 + (-u_xlat29);
        u_xlat1 = abs(u_xlat1) / u_xlat5.x;
        u_xlat1 = u_xlat1 + -0.25;
        u_xlat1 = max(u_xlat1, 0.0);
        u_xlat1 = u_xlat1 * 1.33333337;
        u_xlat1 = min(u_xlat1, 0.75);
        u_xlat10.xy = vs_TEXCOORD0.xy + (-_MainTex_TexelSize.xy);
        u_xlat10.xy = u_xlat10.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat16_3.xyz = textureLod(_MainTex, u_xlat10.xy, 0.0).xyz;
        u_xlat5 = _MainTex_TexelSize.xyxy * vec4(1.0, -1.0, -1.0, 1.0) + vs_TEXCOORD0.xyxy;
        u_xlat5 = u_xlat5 * _MainTex_ST.xyxy + _MainTex_ST.zwzw;
        u_xlat16_4.xyz = textureLod(_MainTex, u_xlat5.xy, 0.0).xyz;
        u_xlat16_5.xyz = textureLod(_MainTex, u_xlat5.zw, 0.0).xyz;
        u_xlat10.xy = vs_TEXCOORD0.xy + _MainTex_TexelSize.xy;
        u_xlat10.xy = u_xlat10.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat16_6.xyz = textureLod(_MainTex, u_xlat10.xy, 0.0).xyz;
        u_xlat7.xyz = u_xlat16_3.xyz + u_xlat16_4.xyz;
        u_xlat7.xyz = u_xlat16_5.xyz + u_xlat7.xyz;
        u_xlat7.xyz = u_xlat16_6.xyz + u_xlat7.xyz;
        u_xlat0.xyz = u_xlat0.xyz + u_xlat7.xyz;
        u_xlat0.xyz = vec3(u_xlat1) * u_xlat0.xyz;
        u_xlat10.x = u_xlat16_3.y * 1.9632107 + u_xlat16_3.x;
        u_xlat19 = u_xlat16_4.y * 1.9632107 + u_xlat16_4.x;
        u_xlat3.x = u_xlat16_5.y * 1.9632107 + u_xlat16_5.x;
        u_xlat12 = u_xlat16_6.y * 1.9632107 + u_xlat16_6.x;
        u_xlat21.x = u_xlat27 * -0.5;
        u_xlat21.x = u_xlat10.x * 0.25 + u_xlat21.x;
        u_xlat21.x = u_xlat19 * 0.25 + u_xlat21.x;
        u_xlat4.x = u_xlat28 * -0.5;
        u_xlat13 = u_xlat28 * 0.5 + (-u_xlat29);
        u_xlat22 = u_xlat30 * -0.5;
        u_xlat13 = u_xlat30 * 0.5 + u_xlat13;
        u_xlat21.x = abs(u_xlat21.x) + abs(u_xlat13);
        u_xlat13 = u_xlat31 * -0.5;
        u_xlat13 = u_xlat3.x * 0.25 + u_xlat13;
        u_xlat13 = u_xlat12 * 0.25 + u_xlat13;
        u_xlat21.x = u_xlat21.x + abs(u_xlat13);
        u_xlat10.x = u_xlat10.x * 0.25 + u_xlat4.x;
        u_xlat10.x = u_xlat3.x * 0.25 + u_xlat10.x;
        u_xlat3.x = u_xlat27 * 0.5 + (-u_xlat29);
        u_xlat3.x = u_xlat31 * 0.5 + u_xlat3.x;
        u_xlat10.x = abs(u_xlat10.x) + abs(u_xlat3.x);
        u_xlat19 = u_xlat19 * 0.25 + u_xlat22;
        u_xlat19 = u_xlat12 * 0.25 + u_xlat19;
        u_xlat10.x = abs(u_xlat19) + u_xlat10.x;
#ifdef UNITY_ADRENO_ES3
        u_xlatb10 = !!(u_xlat10.x>=u_xlat21.x);
#else
        u_xlatb10 = u_xlat10.x>=u_xlat21.x;
#endif
        u_xlat19 = (u_xlatb10) ? (-_MainTex_TexelSize.y) : (-_MainTex_TexelSize.x);
        u_xlat27 = (u_xlatb10) ? u_xlat27 : u_xlat28;
        u_xlat28 = (u_xlatb10) ? u_xlat31 : u_xlat30;
        u_xlat3.x = (-u_xlat29) + u_xlat27;
        u_xlat12 = (-u_xlat29) + u_xlat28;
        u_xlat27 = u_xlat29 + u_xlat27;
        u_xlat27 = u_xlat27 * 0.5;
        u_xlat28 = u_xlat29 + u_xlat28;
        u_xlat28 = u_xlat28 * 0.5;
#ifdef UNITY_ADRENO_ES3
        u_xlatb21 = !!(abs(u_xlat3.x)>=abs(u_xlat12));
#else
        u_xlatb21 = abs(u_xlat3.x)>=abs(u_xlat12);
#endif
        u_xlat27 = (u_xlatb21) ? u_xlat27 : u_xlat28;
        u_xlat28 = (u_xlatb21) ? abs(u_xlat3.x) : abs(u_xlat12);
        u_xlat19 = (u_xlatb21) ? u_xlat19 : (-u_xlat19);
        u_xlat3.x = u_xlat19 * 0.5;
        u_xlat3.y = (u_xlatb10) ? 0.0 : u_xlat3.x;
        u_xlat3.x = u_xlatb10 ? u_xlat3.x : float(0.0);
        u_xlat4.xy = u_xlat3.yx + vs_TEXCOORD0.xy;
        u_xlat28 = u_xlat28 * 0.25;
        u_xlat3.y = float(0.0);
        u_xlat3.z = float(0.0);
        u_xlat3.xw = _MainTex_TexelSize.xy;
        u_xlat3.xy = (bool(u_xlatb10)) ? u_xlat3.xy : u_xlat3.zw;
        u_xlat4 = u_xlat3.xyxy * vec4(-1.5, -1.5, 1.5, 1.5) + u_xlat4.xyxy;
        u_xlat21.xy = u_xlat3.xy + u_xlat3.xy;
        u_xlat5 = u_xlat4;
        u_xlat6.xy = vec2(u_xlat27);
        u_xlati24 = int(0);
        u_xlati33 = int(0);
        u_xlati7 = 0;
        while(true){
#ifdef UNITY_ADRENO_ES3
            u_xlatb16 = !!(u_xlati7>=8);
#else
            u_xlatb16 = u_xlati7>=8;
#endif
            if(u_xlatb16){break;}
            if(u_xlati24 == 0) {
                u_xlat16.xy = u_xlat5.xy * _MainTex_ST.xy + _MainTex_ST.zw;
                u_xlat16_16.xy = textureGrad(_MainTex, u_xlat16.xy, vec4(u_xlat21.xyxx).xy, vec4(u_xlat21.xyxx).xy).xy;
                u_xlat16.x = u_xlat16_16.y * 1.9632107 + u_xlat16_16.x;
            } else {
                u_xlat16.x = u_xlat6.x;
            //ENDIF
            }
            if(u_xlati33 == 0) {
                u_xlat25.xy = u_xlat5.zw * _MainTex_ST.xy + _MainTex_ST.zw;
                u_xlat16_25.xy = textureGrad(_MainTex, u_xlat25.xy, vec4(u_xlat21.xyxx).xy, vec4(u_xlat21.xyxx).xy).xy;
                u_xlat16.y = u_xlat16_25.y * 1.9632107 + u_xlat16_25.x;
            } else {
                u_xlat16.y = u_xlat6.y;
            //ENDIF
            }
            u_xlat34 = (-u_xlat27) + u_xlat16.x;
#ifdef UNITY_ADRENO_ES3
            u_xlatb34 = !!(abs(u_xlat34)>=u_xlat28);
#else
            u_xlatb34 = abs(u_xlat34)>=u_xlat28;
#endif
            u_xlati24 = int(uint(uint(u_xlati24) | (uint(u_xlatb34) * 0xffffffffu)));
            u_xlat34 = (-u_xlat27) + u_xlat16.y;
#ifdef UNITY_ADRENO_ES3
            u_xlatb34 = !!(abs(u_xlat34)>=u_xlat28);
#else
            u_xlatb34 = abs(u_xlat34)>=u_xlat28;
#endif
            u_xlati33 = int(uint(uint(u_xlati33) | (uint(u_xlatb34) * 0xffffffffu)));
            u_xlati34 = int(uint(uint(u_xlati33) & uint(u_xlati24)));
            if(u_xlati34 != 0) {
                u_xlat6.xy = u_xlat16.xy;
                break;
            //ENDIF
            }
            u_xlat8.xy = (-u_xlat3.xy) * vec2(2.0, 2.0) + u_xlat5.xy;
            u_xlat5.xy = (int(u_xlati24) != 0) ? u_xlat5.xy : u_xlat8.xy;
            u_xlat8.xy = u_xlat3.xy * vec2(2.0, 2.0) + u_xlat5.zw;
            u_xlat5.zw = (int(u_xlati33) != 0) ? u_xlat5.zw : u_xlat8.xy;
            u_xlati7 = u_xlati7 + 1;
            u_xlat6.xy = u_xlat16.xy;
        }
        u_xlat3.xy = (-u_xlat5.xy) + vs_TEXCOORD0.xy;
        u_xlat28 = (u_xlatb10) ? u_xlat3.x : u_xlat3.y;
        u_xlat3.xy = u_xlat5.zw + (-vs_TEXCOORD0.xy);
        u_xlat3.x = (u_xlatb10) ? u_xlat3.x : u_xlat3.y;
#ifdef UNITY_ADRENO_ES3
        u_xlatb12 = !!(u_xlat28<u_xlat3.x);
#else
        u_xlatb12 = u_xlat28<u_xlat3.x;
#endif
        u_xlat21.x = (u_xlatb12) ? u_xlat6.x : u_xlat6.y;
        u_xlat29 = (-u_xlat27) + u_xlat29;
#ifdef UNITY_ADRENO_ES3
        { bool cond = u_xlat29<0.0; u_xlati29 = int(!!cond ? 0xFFFFFFFFu : uint(0)); }
#else
        u_xlati29 = int((u_xlat29<0.0) ? 0xFFFFFFFFu : uint(0));
#endif
        u_xlat27 = (-u_xlat27) + u_xlat21.x;
#ifdef UNITY_ADRENO_ES3
        { bool cond = u_xlat27<0.0; u_xlati27 = int(!!cond ? 0xFFFFFFFFu : uint(0)); }
#else
        u_xlati27 = int((u_xlat27<0.0) ? 0xFFFFFFFFu : uint(0));
#endif
#ifdef UNITY_ADRENO_ES3
        u_xlatb27 = !!(u_xlati27==u_xlati29);
#else
        u_xlatb27 = u_xlati27==u_xlati29;
#endif
        u_xlat27 = (u_xlatb27) ? 0.0 : u_xlat19;
        u_xlat19 = u_xlat28 + u_xlat3.x;
        u_xlat28 = (u_xlatb12) ? u_xlat28 : u_xlat3.x;
        u_xlat19 = -1.0 / u_xlat19;
        u_xlat19 = u_xlat28 * u_xlat19 + 0.5;
        u_xlat27 = u_xlat27 * u_xlat19;
        u_xlat19 = (u_xlatb10) ? 0.0 : u_xlat27;
        u_xlat3.x = u_xlat19 + vs_TEXCOORD0.x;
        u_xlat27 = u_xlatb10 ? u_xlat27 : float(0.0);
        u_xlat3.y = u_xlat27 + vs_TEXCOORD0.y;
        u_xlat10.xy = u_xlat3.xy * _MainTex_ST.xy + _MainTex_ST.zw;
        u_xlat16_10.xyz = textureLod(_MainTex, u_xlat10.xy, 0.0).xyz;
        u_xlat0.xyz = u_xlat0.xyz * vec3(0.111111112, 0.111111112, 0.111111112) + u_xlat16_10.xyz;
        u_xlat2.xyz = (-vec3(u_xlat1)) * u_xlat16_10.xyz + u_xlat0.xyz;
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