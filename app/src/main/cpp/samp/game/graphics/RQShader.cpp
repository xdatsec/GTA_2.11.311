//
// Created on 11.04.2024.
//

#include "RQShader.h"
#include "vendor/armhook/patch.h"
#include "game/Streaming.h"

#include <iostream>
#include <sstream>
#include <string>

ShaderCreator vertexSrc;
ShaderCreator pixelSrc;

bool RQShader::BuildSource(uint32 flags, const char **pixelSource, const char **vertexSource)
{
    vertexSrc.shaderSrc[0] = 0;
    pixelSrc.shaderSrc[0] = 0;
    BuildPixelSource(flags);
    BuildVertexSource(flags);

    *pixelSource = (const char*)&pixelSrc;
    *vertexSource = (const char*)&vertexSrc;
    return true;
}

void RQShader::InjectHooks() {
    //co we need this?
    //CHook::Write(g_libGTASA + (VER_x32 ? 0x679320 : 0x850668), &curShaderStateFlags);

   // CHook::Redirect("_ZN8RQShader11BuildSourceEjPPKcS2_", &BuildSource);
}

int OS_SystemChip(){
    return CHook::CallFunction<int>("_Z13OS_SystemChipv");
}

void BuildPixelSource(uint32 flags) {
    RQCapabilities* RQCaps = (RQCapabilities*)(g_libGTASA + (VER_x32 ? 0x6B8B9C : 0x896130));

    int ped_spec = 0;
    std::ostringstream buff;

    buff << "precision mediump float;";

    if ((flags & FLAG_TEX0))
    {
        buff    << "uniform sampler2D Diffuse;"
                << "varying mediump vec2 Out_Tex0;";
    }
    if (flags & (FLAG_ENVMAP | FLAG_SPHERE_ENVMAP))
    {
        if (flags & FLAG_ENVMAP)
            buff << "varying mediump vec2 Out_Tex1;";
        else
            buff << "varying mediump vec3 Out_Refl;";
    }

    if (flags & (FLAG_ENVMAP | FLAG_SPHERE_ENVMAP)) {
        buff    << "uniform sampler2D EnvMap;"
                <<  "uniform lowp float EnvMapCoefficient;";
    }
    else if (flags & FLAG_DETAILMAP) {
        buff    << "uniform sampler2D EnvMap;"
                << "uniform float DetailTiling;";
    }

    if (flags & FLAG_FOG)
    {
        buff    << "varying mediump float Out_FogAmt;"
                << "uniform lowp vec3 FogColor;";
    }
    if (flags & (FLAG_COLOR | FLAG_LIGHTING))
        buff << "varying lowp vec4 Out_Color;";

    if ((flags & FLAG_LIGHT1) && (flags & (FLAG_ENVMAP | FLAG_SPHERE_ENVMAP | ped_spec)))
        buff << "varying lowp vec3 Out_Spec;";

    if (flags & FLAG_ALPHA_MODULATE)
        buff << "uniform lowp float AlphaModulate;";

    if ( (flags & 0x80000) != 0 )
    {
        buff    << "varying mediump vec2 Out_WaterDetail;"
                << "varying mediump vec2 Out_WaterDetail2;"
                << "varying mediump float Out_WaterAlphaBlend;";
    }

    buff    << "void main()"
            << "{"
            << "lowp vec4 fcolor;";

    if ( (flags & 0x20) == 0 )
    {
        if ( (flags & 0x12) != 0 )
            buff << "fcolor = Out_Color;";
        else
            buff << "fcolor = vec4(0.0);";

        goto LABEL_40;
    }
    if (flags & FLAG_TEXBIAS)
        buff << "lowp vec4 diffuseColor = texture2D(Diffuse, Out_Tex0, -1.5);";
//    else if ( RQCaps->isSlowGPU )
//      buff << "lowp vec4 diffuseColor = texture2D(Diffuse, Out_Tex0);";
    else
        buff << "lowp vec4 diffuseColor = texture2D(Diffuse, Out_Tex0, -0.5);";

    buff << "fcolor = diffuseColor;";

    if ( (flags & 0x12) != 0 )
    {
        if ( (flags & 0x10000) == 0 )
        {
            buff << "fcolor *= Out_Color;";
            goto LABEL_33;
        }
        if ( (flags & 0x80000) != 0 )
        {
            buff    << "float waterDetail = texture2D(EnvMap, Out_WaterDetail, -1.0).x + texture2D(EnvMap, Out_WaterDetail2, -1.0).x;"
                    << "fcolor *= vec4(Out_Color.xyz * waterDetail * 1.1, Out_Color.w);";

            goto LABEL_38;
        }
        buff << "fcolor *= vec4(Out_Color.xyz * texture2D(EnvMap, Out_Tex0.xy * DetailTiling, -0.5).xyz * 2.0, Out_Color.w);";

        LABEL_40:
        if ( (flags & 0x40) == 0 )
            goto LABEL_42;
        goto LABEL_41;
    }
    LABEL_33:
    if ( (flags & 0x80000) != 0 )
    {
        LABEL_38:
        buff << "fcolor.a += Out_WaterAlphaBlend;";
        goto LABEL_40;
    }
    if ( (flags & 0x40) == 0 )
        goto LABEL_42;
    LABEL_41:
    buff << "fcolor.xyz = mix(fcolor.xyz, texture2D(EnvMap, Out_Tex1).xyz, EnvMapCoefficient);";
    LABEL_42:
    if (flags & FLAG_SPHERE_ENVMAP)
    {
        buff    << "vec2 ReflPos = normalize(Out_Refl.xy) * (Out_Refl.z * 0.5 + 0.5);"
                << "ReflPos = (ReflPos * vec2(0.5,0.5)) + vec2(0.5,0.5);"
                << "lowp vec4 ReflTexture =  texture2D(EnvMap, ReflPos);"
                << "fcolor.xyz = mix(fcolor.xyz,ReflTexture.xyz, EnvMapCoefficient);"
                << "fcolor.w += ReflTexture.b * 0.125;";
    }
    if (!RQCaps->unk_08) {
        if ((flags & FLAG_LIGHT1) && (flags & (FLAG_ENVMAP | FLAG_SPHERE_ENVMAP | ped_spec)))
            buff << "fcolor.xyz += Out_Spec;";

        if (flags & FLAG_FOG)
            buff << "fcolor.xyz = mix(fcolor.xyz, FogColor, Out_FogAmt);";
    }
    if (flags & FLAG_GAMMA)
        buff << "fcolor.xyz += fcolor.xyz * 0.5;";

    buff << "gl_FragColor = fcolor;";

    if (flags & FLAG_ALPHA_TEST)
    {
        buff << "/*ATBEGIN*/";
//        if ( v2 == OSDC_Tegra3 && (flags & 0x20) != 0 )
//        {
//            if ( (flags & 0x800) != 0 )
//            {
//                snprintf(s, 0x200uLL, "if (diffuseColor.a < 0.8) { discard; }");
//            }
//            else if ( (flags & 0x200) != 0 )
//            {
//                snprintf(s, 0x200uLL, "gl_FragColor.a = Out_Color.a;");
//                strcat((char *)&pixelSrc, s);
//                snprintf(s, 0x200uLL, "if (diffuseColor.a < 0.5) { discard; }");
//            }
//            else
//            {
//                snprintf(s, 0x200uLL, "if (diffuseColor.a < 0.2) { discard; }");
//            }
//        }
        /*else */
        if (flags & FLAG_TEXBIAS)
            buff << "if (gl_FragColor.a < 0.8) { discard; }";

        else if (flags & FLAG_CAMERA_BASED_NORMALS)
        {
            buff    << "if (gl_FragColor.a < 0.5) { discard; }"
                    << "gl_FragColor.a = Out_Color.a;";
        }
        else
            buff << "if (gl_FragColor.a < 0.2) { discard; }";

        buff << "/*ATEND*/";
    }

    if (flags & FLAG_ALPHA_MODULATE)
        buff << "gl_FragColor.a *= AlphaModulate;";

    buff << "}";

    strncpy(pixelSrc.shaderSrc, buff.str().c_str(), sizeof(pixelSrc.shaderSrc) - 1);
    pixelSrc.shaderSrc[sizeof(pixelSrc.shaderSrc) - 1] = '\0';
}

int GetMobileEffectSetting() {
    return CHook::CallFunction<int>("_Z22GetMobileEffectSettingv");
}

void BuildVertexSource(uint32 flags) {
    int& RQMaxBones = *(int(*))(g_libGTASA + (VER_x32 ? 0x6B8BAC : 0x896140));
    RQCapabilities* RQCaps = (RQCapabilities*)(g_libGTASA + (VER_x32 ? 0x6B8B9C : 0x896130));

    char* vertexColor;
    //int ped_spec = (FLAG_BONE3 | FLAG_BONE4); //  (FLAG_BONE3 | FLAG_BONE4)

    int ped_spec = 0;
    std::ostringstream buff;

    buff    << "#version 100\n"
            << "precision highp float;\n"
            << "uniform mat4 ProjMatrix;\n"
            << "uniform mat4 ViewMatrix;\n"
            << "uniform mat4 ObjMatrix;\n";

    if (flags & FLAG_LIGHTING)
    {
        buff    << "uniform lowp vec3 AmbientLightColor;\n"
                << "uniform lowp vec4 MaterialEmissive;\n"
                << "uniform lowp vec4 MaterialAmbient;\n"
                << "uniform lowp vec4 MaterialDiffuse;\n";

        if (flags & FLAG_LIGHT1)
        {
            buff    << "uniform lowp vec3 DirLightDiffuseColor;\n"
                    << "uniform vec3 DirLightDirection;\n";

            if (GetMobileEffectSetting() == 3) {
                if (flags & (FLAG_BACKLIGHT | FLAG_BONE3 | FLAG_BONE4))
                    buff << "uniform vec3 DirBackLightDirection;";
            }
        }
        if (flags & FLAG_LIGHT2)
        {
            buff    << "uniform lowp vec3 DirLight2DiffuseColor;\n"
                    << "uniform vec3 DirLight2Direction;\n";
        }
        if (flags & FLAG_LIGHT3)
        {
            buff    << "uniform lowp vec3 DirLight3DiffuseColor;\n"
                    << "uniform vec3 DirLight3Direction;\n";
        }
    }

    buff    << "attribute vec3 Position;\n"
            << "attribute vec3 Normal;\n";

    if (flags & FLAG_TEX0) {
        if (flags & FLAG_PROJECT_TEXCOORD)
            buff << "attribute vec4 TexCoord0;\n";
        else
            buff << "attribute vec2 TexCoord0;\n";
    }

    buff << "attribute vec4 GlobalColor;\n";

    if (flags & (FLAG_BONE3 | FLAG_BONE4)) {
        buff    << "attribute vec4 BoneWeight;\n"
                << "attribute vec4 BoneIndices;\n"
                << "uniform highp vec4 Bones[" << 3 * RQMaxBones << "];\n";
    }

    if (flags & FLAG_TEX0)
        buff << "varying mediump vec2 Out_Tex0;\n";

    if (flags & FLAG_TEXMATRIX)
        buff << "uniform mat3 NormalMatrix;\n";

    if (flags & (FLAG_ENVMAP | FLAG_SPHERE_ENVMAP))
        buff << "uniform lowp float EnvMapCoefficient;\n";

    if (flags & FLAG_ENVMAP)
        buff << "varying mediump vec2 Out_Tex1;\n";
    else if (flags & FLAG_SPHERE_ENVMAP)
        buff << "varying mediump vec3 Out_Refl;\n";

    if (flags & (FLAG_ENVMAP | FLAG_SPHERE_ENVMAP | FLAG_CAMERA_BASED_NORMALS | FLAG_FOG | FLAG_WATER | FLAG_SPHERE_XFORM | ped_spec))
        buff << "uniform vec3 CameraPosition;\n";

    if (flags & FLAG_FOG)
    {
        buff    << "varying mediump float Out_FogAmt;"
                << "uniform vec3 FogDistances;";
    }

    if (flags & FLAG_WATER)
    {
        buff    << "uniform vec3 WaterSpecs;"
                << "varying mediump vec2 Out_WaterDetail;"
                << "varying mediump vec2 Out_WaterDetail2;"
                << "varying mediump float Out_WaterAlphaBlend;";
    }

    if (flags & FLAG_COLOR2)
    {
        buff    << "attribute vec4 Color2;"
                << "uniform lowp float ColorInterp;";
    }

    if ( flags & (FLAG_COLOR | FLAG_LIGHTING))
        buff << "varying lowp vec4 Out_Color;";

    if ((flags & FLAG_LIGHT1) && (flags & (FLAG_ENVMAP | FLAG_SPHERE_ENVMAP | ped_spec)))
        buff << "varying lowp vec3 Out_Spec;";

    buff << "void main() {";
    if (flags & (FLAG_BONE3 | FLAG_BONE4))
    {
        buff    << "\tivec4 BlendIndexArray = ivec4(BoneIndices);"
                << "\tmat4 BoneToLocal;"
                << "\tBoneToLocal[0] = Bones[BlendIndexArray.x*3] * BoneWeight.x;"
                << "\tBoneToLocal[1] = Bones[BlendIndexArray.x*3+1] * BoneWeight.x;"
                << "\tBoneToLocal[2] = Bones[BlendIndexArray.x*3+2] * BoneWeight.x;"
                << "\tBoneToLocal[3] = vec4(0.0,0.0,0.0,1.0);"
                << "\tBoneToLocal[0] += Bones[BlendIndexArray.y*3] * BoneWeight.y;"
                << "\tBoneToLocal[1] += Bones[BlendIndexArray.y*3+1] * BoneWeight.y;"
                << "\tBoneToLocal[2] += Bones[BlendIndexArray.y*3+2] * BoneWeight.y;"
                << "\tBoneToLocal[0] += Bones[BlendIndexArray.z*3] * BoneWeight.z;"
                << "\tBoneToLocal[1] += Bones[BlendIndexArray.z*3+1] * BoneWeight.z;"
                << "\tBoneToLocal[2] += Bones[BlendIndexArray.z*3+2] * BoneWeight.z;";

        if (flags & FLAG_BONE4)
        {
            buff    << "\tBoneToLocal[0] += Bones[BlendIndexArray.w*3] * BoneWeight.w;"
                    << "\tBoneToLocal[1] += Bones[BlendIndexArray.w*3+1] * BoneWeight.w;"
                    << "\tBoneToLocal[2] += Bones[BlendIndexArray.w*3+2] * BoneWeight.w;";
        }
        buff << "\tvec4 WorldPos = ObjMatrix * (vec4(Position,1.0) * BoneToLocal);";
    }
    else
    {
        buff << "\tvec4 WorldPos = ObjMatrix * vec4(Position,1.0);";
    }

    if (flags & FLAG_SPHERE_XFORM)
    {
        buff    << "    vec3 ReflVector = WorldPos.xyz - CameraPosition.xyz;"
                << "\tvec3 ReflPos = normalize(ReflVector);"
                << "    ReflPos.xy = normalize(ReflPos.xy) * (ReflPos.z * 0.5 + 0.5);";


        buff << "\tgl_Position = vec4(ReflPos.xy, length(ReflVector) * 0.002, 1.0);";
    }
    else
    {
        buff << "\tvec4 ViewPos = ViewMatrix * WorldPos;";

        buff << "\tgl_Position = ProjMatrix * ViewPos;";
    }
//
    if (flags & FLAG_LIGHTING)
    {
        if (((flags & (FLAG_CAMERA_BASED_NORMALS | FLAG_ALPHA_TEST)) == (FLAG_CAMERA_BASED_NORMALS | FLAG_ALPHA_TEST)) && (flags & (FLAG_LIGHT1 | FLAG_LIGHT2 | FLAG_LIGHT3)))
            buff << "vec3 WorldNormal = normalize(vec3(WorldPos.xy - CameraPosition.xy, 0.0001)) * 0.85;";
        else
        {
            if (flags & (FLAG_BONE3 | FLAG_BONE4))
                buff << "vec3 WorldNormal = mat3(ObjMatrix) * (Normal * mat3(BoneToLocal));";
            else
                buff << "vec3 WorldNormal = (ObjMatrix * vec4(Normal,0.0)).xyz;";
        }
    }
    else
    {
        if (flags & (FLAG_ENVMAP | FLAG_SPHERE_ENVMAP))
            buff << "vec3 WorldNormal = vec3(0.0, 0.0, 0.0);";
    }
    if (flags & FLAG_FOG)
        buff << "Out_FogAmt = clamp((length(WorldPos.xyz - CameraPosition.xyz) - FogDistances.x) * FogDistances.z, 0.0, 1.0);";
    //   buff << "Out_FogAmt = clamp((length(WorldPos.xyz - CameraPosition.xyz) - FogDistances.x) * FogDistances.z, 0.0, 0.90);";

    if (flags & FLAG_TEX0) {
        char* tex;

        if (flags & FLAG_PROJECT_TEXCOORD)
            tex = "TexCoord0.w == 0.0 ? TexCoord0.xy : TexCoord0.xy / TexCoord0.w";

        else if (flags & FLAG_COMPRESSED_TEXCOORD)
            tex = "TexCoord0 / 512.0";
        else
            tex = "TexCoord0";

        if (flags & FLAG_TEXMATRIX)
            buff << "Out_Tex0 = (NormalMatrix * vec3(" << tex << ", 1.0)).xy;";
        else
            buff << "Out_Tex0 = " << tex << ";";
    }

    if (flags & (FLAG_ENVMAP | FLAG_SPHERE_ENVMAP))
    {
        buff    << "vec3 reflVector = normalize(WorldPos.xyz - CameraPosition.xyz);"
                << "reflVector = reflVector - 2.0 * dot(reflVector, WorldNormal) * WorldNormal;";

        if (flags & FLAG_SPHERE_ENVMAP)
            buff << "Out_Refl = reflVector;";
        else
            buff << "Out_Tex1 = vec2(length(reflVector.xy), (reflVector.z * 0.5) + 0.25);";
    }

    if (flags & FLAG_COLOR2)
    {
        buff << "lowp vec4 InterpColor = mix(GlobalColor, Color2, ColorInterp);";
        vertexColor = "InterpColor";
    }
    else
    {
        vertexColor = "GlobalColor";
    }

    if (flags & FLAG_LIGHTING) {
        //VTX_EMIT("half3 Out_LightingColor;");

        buff    << "vec3 ambEmissLight = vec3(0.0, 0.0, 0.0);"
                << "vec3 diffColor = vec3(0.0, 0.0, 0.0);";

        if (flags & FLAG_COLOR_EMISSIVE) {
            if(flags & (FLAG_LIGHT1 | FLAG_LIGHT2 | FLAG_LIGHT3)){
                // TOTAL HACK for 3d markers (looks like we can catch them here).
                // material color is not white but prelight wasn't adjusted.
                // we happen to know that diffuse light is white, so MaterialDiffuse should be
                // the unmodified material color and we can multiply with it.
                buff << "ambEmissLight = AmbientLightColor * MaterialAmbient.xyz + " << vertexColor << ".xyz * MaterialDiffuse.xyz;";
            }else
            if (flags & FLAG_CAMERA_BASED_NORMALS){
                // This happens to objects with alpha test.
                // trees in particular tend have their vertex colors cranked to white
                // so let's try to tone that down a bit
                buff    << "vec3 vertClamped = clamp(" << vertexColor <<".xyz, 0.0, 0.5);"
                        // NB: AmbientLightColor is DirectionalLightColourFromDay here
                        << "ambEmissLight = AmbientLightColor * MaterialAmbient.xyz + vertClamped;";
                // dunno what they were going for with this. flat shading because they messed up the prelight?
                //	VTX_EMIT("ambEmissLight = AmbientLightColor * MaterialAmbient.xyz * 1.5;");
            }else
                buff << "ambEmissLight = AmbientLightColor * MaterialAmbient.xyz + " << vertexColor <<".xyz;";
        } else {
            buff << "ambEmissLight = AmbientLightColor * MaterialAmbient.xyz + MaterialEmissive.xyz;";
        }

        // Diffuse Light
        if (flags & (FLAG_LIGHT1 | FLAG_LIGHT2 | FLAG_LIGHT3)) {
            if (flags & FLAG_LIGHT1) {
                if (GetMobileEffectSetting() == 3 && (flags & (FLAG_BACKLIGHT | FLAG_BONE3 | FLAG_BONE4)))
                    buff << "diffColor += (max(dot(DirLightDirection, WorldNormal), 0.0) + max(dot(DirBackLightDirection, WorldNormal), 0.0)) * DirLightDiffuseColor;";
                else
                    buff << "diffColor += max(dot(DirLightDirection, WorldNormal), 0.0) * DirLightDiffuseColor;";
            }
            if (flags & FLAG_LIGHT2)
                buff << "diffColor += max(dot(DirLight2Direction, WorldNormal), 0.0) * DirLight2DiffuseColor;";
            if (flags & FLAG_LIGHT3)
                buff << "diffColor += max(dot(DirLight3Direction, WorldNormal), 0.0) * DirLight3DiffuseColor;";

            buff << "diffColor *= MaterialDiffuse.xyz;";
        }

        // Final Color
        if (flags & (FLAG_COLOR | FLAG_LIGHTING)) {
            buff    << "Out_Color = vec4(ambEmissLight + diffColor, MaterialAmbient.w * " << vertexColor <<".w);"
                    << "Out_Color = clamp(Out_Color, 0.0, 1.0);";
        }
    } else {
        if (flags & (FLAG_COLOR | FLAG_LIGHTING))
            buff << "Out_Color = " << vertexColor << ";";
    }

    if (!RQCaps->unk_08 && (flags & FLAG_LIGHT1)) {
        if (flags & ( FLAG_ENVMAP | FLAG_SPHERE_ENVMAP))
        {
            char* v9 = const_cast<char *>(RQCaps->isMaliChip ? "9.0" : "10.0");

            buff    << "float specAmt = max(pow(dot(reflVector, DirLightDirection), " << v9 << "), 0.0) * EnvMapCoefficient * 2.0;"
                    << "Out_Spec = specAmt * DirLightDiffuseColor;";
        }
        else if (flags & ped_spec)
        {
            char* v9 = const_cast<char *>(RQCaps->isMaliChip ? "5.0" : "4.0");

            buff    << "vec3 reflVector = normalize(WorldPos.xyz - CameraPosition.xyz);"
                    << "reflVector = reflVector - 2.0 * dot(reflVector, WorldNormal) * WorldNormal;"
                    << "float specAmt = max(pow(dot(reflVector, DirLightDirection), " << v9 << "), 0.0) * 0.125;"
                    << "Out_Spec = specAmt * DirLightDiffuseColor;";
        }
    }

    if (flags & FLAG_WATER)
    {
        buff    << "Out_WaterDetail = (Out_Tex0 * 4.0) + vec2(WaterSpecs.x * -0.3, WaterSpecs.x * 0.21);"
                << "Out_WaterDetail2 = (Out_Tex0 * -8.0) + vec2(WaterSpecs.x * 0.12, WaterSpecs.x * -0.05);"
                << "Out_WaterAlphaBlend = distance(WorldPos.xy, CameraPosition.xy) * WaterSpecs.y;";
    }

    buff << "}";

    strncpy(vertexSrc.shaderSrc, buff.str().c_str(), sizeof(vertexSrc.shaderSrc) - 1);
    vertexSrc.shaderSrc[sizeof(vertexSrc.shaderSrc) - 1] = '\0';
}