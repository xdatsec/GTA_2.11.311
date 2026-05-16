#include <GLES3/gl32.h>
#include "main.h"
#include "patch.h"

#define FLAG_ALPHA_TEST           0x01
#define FLAG_LIGHTING             0x02
#define FLAG_ALPHA_MODULATE       0x04
#define FLAG_COLOR_EMISSIVE       0x08
#define FLAG_COLOR                0x10
#define FLAG_TEX0                 0x20
#define FLAG_ENVMAP               0x40          // normal envmap
#define FLAG_BONE3                0x80
#define FLAG_BONE4                0x100
#define FLAG_CAMERA_BASED_NORMALS 0x200
#define FLAG_FOG                  0x400
#define FLAG_TEXBIAS              0x800
#define FLAG_BACKLIGHT            0x1000
#define FLAG_LIGHT1               0x2000
#define FLAG_LIGHT2               0x4000
#define FLAG_LIGHT3               0x8000
#define FLAG_DETAILMAP            0x10000
#define FLAG_COMPRESSED_TEXCOORD  0x20000
#define FLAG_PROJECT_TEXCOORD     0x40000
#define FLAG_WATER                0x80000
#define FLAG_COLOR2               0x100000
#define FLAG_SPHERE_XFORM         0x800000      // this renders the scene as a sphere map for vehicle reflections
#define FLAG_SPHERE_ENVMAP        0x1000000     // spherical real-time envmap
#define FLAG_TEXMATRIX            0x2000000
#define FLAG_GAMMA                0x4000000

#define PXL_EMIT(...)                        \
  do {                                       \
    snprintf(tmp, sizeof(tmp), __VA_ARGS__); \
    strcat(pxlbuf, tmp);                     \
    strcat(pxlbuf, "\n");                    \
  } while (0)


#define VTX_EMIT(...)                        \
  do {                                       \
    snprintf(tmp, sizeof(tmp), __VA_ARGS__); \
    strcat(vtxbuf, tmp);                     \
    strcat(vtxbuf, "\n");                    \
  } while (0)

char pxlbuf[1024 * 10];
char vtxbuf[1024 * 10];

char* byte_617290 = 0; //pxl
char* byte_617258 = 0;

char* byte_619294 = 0; //vertex

#define PXL_SOURCE(str) \
		strcat(byte_617290, str) \

#define VTX_SOURCE(str) \
        strcat(byte_619294, str) \


void BuildVertexSource(int flags) {
    int uVar1;
    int uVar2;
    int uVar3;
    int uVar4;
    char *pcVar5;
    char *pcVar6;
    int uVar7;
    int uVar8;
    int uVar9;
    char acStack_102c[4096];
    int iStack_2c;

    snprintf(acStack_102c, 0x200, "#version 100\n");
    strcat((char *)&vtxbuf, acStack_102c);
    snprintf(acStack_102c, 0x200, "precision highp float;");
    strcat((char *)&vtxbuf, acStack_102c);
    snprintf(acStack_102c, 0x200, "uniform mat4 ProjMatrix;");
    strcat((char *)&vtxbuf, acStack_102c);
    snprintf(acStack_102c, 0x200, "uniform mat4 ViewMatrix;");
    strcat((char *)&vtxbuf, acStack_102c);
    snprintf(acStack_102c, 0x200, "uniform mat4 ObjMatrix;");
    strcat((char *)&vtxbuf, acStack_102c);
    uVar1 = flags & 2;
    if (uVar1 != 0) {
        snprintf(acStack_102c, 0x200, "uniform lowp vec3 AmbientLightColor;");
        strcat((char *)&vtxbuf, acStack_102c);
        snprintf(acStack_102c, 0x200, "uniform lowp vec4 MaterialEmissive;");
        strcat((char *)&vtxbuf, acStack_102c);
        snprintf(acStack_102c, 0x200, "uniform lowp vec4 MaterialAmbient;");
        strcat((char *)&vtxbuf, acStack_102c);
        snprintf(acStack_102c, 0x200, "uniform lowp vec4 MaterialDiffuse;");
        strcat((char *)&vtxbuf, acStack_102c);
        if ((int) (flags << 0x12) < 0) {
            snprintf(acStack_102c, 0x200, "uniform lowp vec3 DirLightDiffuseColor;");
            strcat((char *)&vtxbuf, acStack_102c);
            snprintf(acStack_102c, 0x200, "uniform vec3 DirLightDirection;");
            strcat((char *)&vtxbuf, acStack_102c);
            if ( (flags & 0x1180) != 0 && CHook::CallFunction<int>("_Z22GetMobileEffectSettingv") != 0 ) {
                snprintf(acStack_102c, 0x200, "uniform vec3 DirBackLightDirection;");
                strcat((char *)&vtxbuf, acStack_102c);
            }
        }
        if ((int) (flags << 0x11) < 0) {
            snprintf(acStack_102c, 0x200, "uniform lowp vec3 DirLight2DiffuseColor;");
            strcat((char *)&vtxbuf, acStack_102c);
            snprintf(acStack_102c, 0x200, "uniform vec3 DirLight2Direction;");
            strcat((char *)&vtxbuf, acStack_102c);
        }
        if ((int) (flags << 0x10) < 0) {
            snprintf(acStack_102c, 0x200, "uniform lowp vec3 DirLight3DiffuseColor;");
            strcat((char *)&vtxbuf, acStack_102c);
            snprintf(acStack_102c, 0x200, "uniform vec3 DirLight3Direction;");
            strcat((char *)&vtxbuf, acStack_102c);
        }
    }
    snprintf(acStack_102c, 0x200, "attribute vec3 Position;");
    strcat((char *)&vtxbuf, acStack_102c);
    snprintf(acStack_102c, 0x200, "attribute vec3 Normal;");
    strcat((char *)&vtxbuf, acStack_102c);
    uVar7 = flags & 0x20;
    if (uVar7 != 0) {
        if ((int) (flags << 0xd) < 0) {
            pcVar5 = "attribute vec4 TexCoord0;";
        } else {
            pcVar5 = "attribute vec2 TexCoord0;";
        }
        snprintf(acStack_102c, 0x200, "%s", pcVar5);
        strcat((char *)&vtxbuf, acStack_102c);
    }
    snprintf(acStack_102c, 0x200, "attribute vec4 GlobalColor;");
    strcat((char *)&vtxbuf, acStack_102c);
    uVar2 = flags & 0x180;
    if (uVar2 != 0) {
        snprintf(acStack_102c, 0x200, "attribute vec4 BoneWeight;");
        strcat((char *)&vtxbuf, acStack_102c);
        snprintf(acStack_102c, 0x200, "attribute vec4 BoneIndices;");
        strcat((char *)&vtxbuf, acStack_102c);
        snprintf(acStack_102c, 0x200, "uniform highp vec4 Bones[%d];", 3 * *(uint32_t *)(g_libGTASA + (VER_x32 ? 0x6B8BAC:0x896140)));
        strcat((char *)&vtxbuf, acStack_102c);
    }
    if (uVar7 != 0) {
        snprintf(acStack_102c, 0x200, "varying mediump vec2 Out_Tex0;");
        strcat((char *)&vtxbuf, acStack_102c);
    }
    if ((flags & 0x2000000) != 0) {
        snprintf(acStack_102c, 0x200, "uniform mat3 NormalMatrix;");
        strcat((char *)&vtxbuf, acStack_102c);
    }
    uVar9 = flags & 0x1000040;
    if (uVar9 != 0) {
        snprintf(acStack_102c, 0x200, "uniform lowp float EnvMapCoefficient;");
        strcat((char *)&vtxbuf, acStack_102c);
        if ((int) (flags << 0x19) < 0) {
            pcVar5 = "varying mediump vec2 Out_Tex1;";
        } else {
            pcVar5 = "varying mediump vec3 Out_Refl;";
        }
        snprintf(acStack_102c, 0x200, "%s", pcVar5);
        strcat((char *)&vtxbuf, acStack_102c);
    }
    if ((flags & 0x9880640) != 0) {
        snprintf(acStack_102c, 0x200, "uniform vec3 CameraPosition;");
        strcat((char *)&vtxbuf, acStack_102c);
    }
    uVar3 = flags & 0x400;
    if (uVar3 != 0) {
        snprintf(acStack_102c, 0x200, "varying mediump float Out_FogAmt;");
        strcat((char *)&vtxbuf, acStack_102c);
        snprintf(acStack_102c, 0x200, "uniform vec3 FogDistances;");
        strcat((char *)&vtxbuf, acStack_102c);
        snprintf(acStack_102c, 0x200, "varying mediump vec4 Out_ClipSpace;");
        strcat((char *)&vtxbuf, acStack_102c);
    }
    if ((flags & 0x80000) != 0) {
        snprintf(acStack_102c, 0x200, "uniform vec3 WaterSpecs;");
        strcat((char *)&vtxbuf, acStack_102c);
        snprintf(acStack_102c, 0x200, "varying mediump vec2 Out_WaterDetail;");
        strcat((char *)&vtxbuf, acStack_102c);
        snprintf(acStack_102c, 0x200, "varying mediump vec2 Out_WaterDetail2;");
        strcat((char *)&vtxbuf, acStack_102c);
        snprintf(acStack_102c, 0x200, "varying mediump float Out_WaterAlphaBlend;");
        strcat((char *)&vtxbuf, acStack_102c);
    }
    if ((flags & 0x100000) != 0) {
        snprintf(acStack_102c, 0x200, "attribute vec4 Color2;");
        strcat((char *)&vtxbuf, acStack_102c);
        snprintf(acStack_102c, 0x200, "uniform lowp float ColorInterp;");
        strcat((char *)&vtxbuf, acStack_102c);
    }
    uVar4 = flags & 0x12;
    if (uVar4 != 0) {
        snprintf(acStack_102c, 0x200, "varying lowp vec4 Out_Color;");
        strcat((char *)&vtxbuf, acStack_102c);
    }
    uVar8 = flags & 0x2000;
    if (uVar8 != 0 && uVar9 != 0) {
        snprintf(acStack_102c, 0x200, "varying lowp vec3 Out_Spec;");
        strcat((char *)&vtxbuf, acStack_102c);
    }
    snprintf(acStack_102c, 0x200, "void main() {");
    strcat((char *)&vtxbuf, acStack_102c);
    if (uVar2 == 0) {
        pcVar5 = "\tvec4 WorldPos = ObjMatrix * vec4(Position,1.0);";
    } else {
        snprintf(acStack_102c, 0x200, "\tivec4 BlendIndexArray = ivec4(BoneIndices);");
        strcat((char *)&vtxbuf, acStack_102c);
        snprintf(acStack_102c, 0x200, "\tmat4 BoneToLocal;");
        strcat((char *)&vtxbuf, acStack_102c);
        snprintf(acStack_102c, 0x200,
                 "\tBoneToLocal[0] = Bones[BlendIndexArray.x*3] * BoneWeight.x;");
        strcat((char *)&vtxbuf, acStack_102c);
        snprintf(acStack_102c, 0x200,
                 "\tBoneToLocal[1] = Bones[BlendIndexArray.x*3+1] * BoneWeight.x;");
        strcat((char *)&vtxbuf, acStack_102c);
        snprintf(acStack_102c, 0x200,
                 "\tBoneToLocal[2] = Bones[BlendIndexArray.x*3+2] * BoneWeight.x;");
        strcat((char *)&vtxbuf, acStack_102c);
        snprintf(acStack_102c, 0x200, "\tBoneToLocal[3] = vec4(0.0,0.0,0.0,1.0);");
        strcat((char *)&vtxbuf, acStack_102c);
        snprintf(acStack_102c, 0x200,
                 "\tBoneToLocal[0] += Bones[BlendIndexArray.y*3] * BoneWeight.y;");
        strcat((char *)&vtxbuf, acStack_102c);
        snprintf(acStack_102c, 0x200,
                 "\tBoneToLocal[1] += Bones[BlendIndexArray.y*3+1] * BoneWeight.y;");
        strcat((char *)&vtxbuf, acStack_102c);
        snprintf(acStack_102c, 0x200,
                 "\tBoneToLocal[2] += Bones[BlendIndexArray.y*3+2] * BoneWeight.y;");
        strcat((char *)&vtxbuf, acStack_102c);
        snprintf(acStack_102c, 0x200,
                 "\tBoneToLocal[0] += Bones[BlendIndexArray.z*3] * BoneWeight.z;");
        strcat((char *)&vtxbuf, acStack_102c);
        snprintf(acStack_102c, 0x200,
                 "\tBoneToLocal[1] += Bones[BlendIndexArray.z*3+1] * BoneWeight.z;");
        strcat((char *)&vtxbuf, acStack_102c);
        snprintf(acStack_102c, 0x200,
                 "\tBoneToLocal[2] += Bones[BlendIndexArray.z*3+2] * BoneWeight.z;");
        strcat((char *)&vtxbuf, acStack_102c);
        if ((int) (flags << 0x17) < 0) {
            snprintf(acStack_102c, 0x200,
                     "\tBoneToLocal[0] += Bones[BlendIndexArray.w*3] * BoneWeight.w;");
            strcat((char *)&vtxbuf, acStack_102c);
            snprintf(acStack_102c, 0x200,
                     "\tBoneToLocal[1] += Bones[BlendIndexArray.w*3+1] * BoneWeight.w;"
            );
            strcat((char *)&vtxbuf, acStack_102c);
            snprintf(acStack_102c, 0x200,
                     "\tBoneToLocal[2] += Bones[BlendIndexArray.w*3+2] * BoneWeight.w;"
            );
            strcat((char *)&vtxbuf, acStack_102c);
        }
        pcVar5 = "\tvec4 WorldPos = ObjMatrix * (vec4(Position,1.0) * BoneToLocal);";
    }
    snprintf(acStack_102c, 0x200, "%s", pcVar5);
    strcat((char *)&vtxbuf, acStack_102c);
    if ((int) (flags << 4) < 0) {
        snprintf(acStack_102c, 0x200, "    vec3 ReflVector = WorldPos.xyz - CameraPosition.xyz;");
        strcat((char *)&vtxbuf, acStack_102c);
        snprintf(acStack_102c, 0x200,
                 "    float coefOffset = 1.0 - clamp(length(ReflVector.xy) / 45.0, 0.0, 1.0);");
        strcat((char *)&vtxbuf, acStack_102c);
        snprintf(acStack_102c, 0x200, "    coefOffset = -1000.0 * coefOffset;");
        strcat((char *)&vtxbuf, acStack_102c);
        snprintf(acStack_102c, 0x200, "\tvec3 ReflPos = normalize(ReflVector);");
        strcat((char *)&vtxbuf, acStack_102c);
        snprintf(acStack_102c, 0x200,
                 "    ReflPos.xy = normalize(ReflPos.xy) * (ReflPos.z * 0.5 + 0.5);");
        strcat((char *)&vtxbuf, acStack_102c);
        pcVar5 = "\tgl_Position = vec4(ReflPos.xy, coefOffset + length(ReflVector) * 0.002, 1.0);";
        LAB_000738d6:
        snprintf(acStack_102c, 0x200, "%s", pcVar5);
        LAB_000738e4:
        strcat((char *)&vtxbuf, acStack_102c);
    } else {
        if ((int) (flags << 8) < 0) {
            snprintf(acStack_102c, 0x200,
                     "    vec3 ReflVector = WorldPos.xyz - CameraPosition.xyz;");
            strcat((char *)&vtxbuf, acStack_102c);
            snprintf(acStack_102c, 0x200, "\tvec3 ReflPos = normalize(ReflVector);");
            strcat((char *)&vtxbuf, acStack_102c);
            snprintf(acStack_102c, 0x200,
                     "    ReflPos.xy = normalize(ReflPos.xy) * (ReflPos.z * 0.5 + 0.5);");
            strcat((char *)&vtxbuf, acStack_102c);
            pcVar5 = "\tgl_Position = vec4(ReflPos.xy, length(ReflVector) * 0.002, 1.0);";
            goto LAB_000738d6;
        }
        snprintf(acStack_102c, 0x200, "\tvec4 ViewPos = ViewMatrix * WorldPos;");
        strcat((char *)&vtxbuf, acStack_102c);
        snprintf(acStack_102c, 0x200, "\tgl_Position = ProjMatrix * ViewPos;");
        strcat((char *)&vtxbuf, acStack_102c);
        if (uVar3 != 0) {
            snprintf(acStack_102c, 0x200, "\tOut_ClipSpace = gl_Position;");
            goto LAB_000738e4;
        }
    }
    if (uVar1 == 0) {
        if (uVar9 != 0) {
            pcVar5 = "vec3 WorldNormal = vec3(0.0, 0.0, 0.0);";
            goto LAB_00073930;
        }
    } else {
        if (((flags & 0x201) == 0x201) && ((flags & 0xe000) != 0)) {
            pcVar5 = "vec3 WorldNormal = normalize(vec3(WorldPos.xy - CameraPosition.xy, 0.0001)) * 0.85;";
        } else if (uVar2 == 0) {
            pcVar5 = "vec3 WorldNormal = (ObjMatrix * vec4(Normal,0.0)).xyz;";
        } else {
            pcVar5 = "vec3 WorldNormal = mat3(ObjMatrix) * (Normal * mat3(BoneToLocal));";
        }
        LAB_00073930:
        snprintf(acStack_102c, 0x200, "%s", pcVar5);
        strcat((char *)&vtxbuf, acStack_102c);
    }
    if (((g_libGTASA != -0x617250) && (*(int *) (g_libGTASA + 0x617258) == 0)) &&
        (uVar3 != 0)) {
        snprintf(acStack_102c, 0x200,
                 "Out_FogAmt = clamp((length(WorldPos.xyz - CameraPosition.xyz) - FogDistances.x) * FogDistances.z, 0.0, 1.0);"
        );
        strcat((char *)&vtxbuf, acStack_102c);
    }
    if (uVar7 != 0) {
        if ((int) (flags << 0xd) < 0) {
            pcVar5 = "TexCoord0.xy / TexCoord0.w";
        } else {
            pcVar5 = "TexCoord0 / 512.0";
            if (-1 < (int) (flags << 0xe)) {
                pcVar5 = "TexCoord0";
            }
        }
        if ((flags & 0x2000000) == 0) {
            pcVar6 = "Out_Tex0 = %s;";
        } else {
            pcVar6 = "Out_Tex0 = (NormalMatrix * vec3(%s, 1.0)).xy;";
        }
        snprintf(acStack_102c, 0x200, pcVar6, pcVar5);
        strcat((char *)&vtxbuf, acStack_102c);
    }
    if (uVar9 != 0) {
        snprintf(acStack_102c, 0x200,
                 "vec3 reflVector = normalize(WorldPos.xyz - CameraPosition.xyz);");
        strcat((char *)&vtxbuf, acStack_102c);
        snprintf(acStack_102c, 0x200,
                 "reflVector = reflVector - 2.0 * dot(reflVector, WorldNormal) * WorldNormal;");
        strcat((char *)&vtxbuf, acStack_102c);
        if ((int) (flags << 7) < 0) {
            pcVar5 = "Out_Refl = reflVector;";
        } else {
            pcVar5 = "Out_Tex1 = vec2(length(reflVector.xy), (reflVector.z * 0.5) + 0.25);";
        }
        snprintf(acStack_102c, 0x200, "%s", pcVar5);
        strcat((char *)&vtxbuf, acStack_102c);
    }
    if ((flags & 0x100000) == 0) {
        pcVar5 = "GlobalColor";
    } else {
        snprintf(acStack_102c, 0x200,
                 "lowp vec4 InterpColor = mix(GlobalColor, Color2, ColorInterp);");
        strcat((char *)&vtxbuf, acStack_102c);
        pcVar5 = "InterpColor";
    }
    uVar7 = uVar8 >> 0xd ^ 1;
    if (uVar1 == 0) {
        if (uVar4 == 0) goto LAB_00073bca;
        snprintf(acStack_102c, 0x200, "Out_Color = %s;", pcVar5);
    } else {
        snprintf(acStack_102c, 0x200, "vec3 Out_LightingColor;");
        strcat((char *)&vtxbuf, acStack_102c);
        if ((int) (flags << 0x1c) < 0) {
            if ((int) (flags << 0x16) < 0) {
                pcVar6 = "Out_LightingColor = AmbientLightColor * MaterialAmbient.xyz * 1.5;";
                goto LAB_00073af8;
            }
            snprintf(acStack_102c, 0x200,
                     "Out_LightingColor = AmbientLightColor * MaterialAmbient.xyz * 1.5 + %s.xyz;",
                     pcVar5);
        } else {
            pcVar6 =
                    "Out_LightingColor = AmbientLightColor * MaterialAmbient.xyz * 1.5 + MaterialEmissive.xyz;";
            LAB_00073af8:
            snprintf(acStack_102c, 0x200, "%s", pcVar6);
        }
        strcat((char *)&vtxbuf, acStack_102c);
        if ((flags & 0xe000) != 0) {
            if (uVar8 != 0) {
                if (((flags & 0x1180) == 0) || (*(uint *) (g_libGTASA + 0x8ed870) < 2)) {
                    pcVar6 =
                            "Out_LightingColor += max(dot(DirLightDirection, WorldNormal), 0.0) * DirLightDiffuseColor;";
                } else {
                    pcVar6 =
                            "Out_LightingColor += (max(dot(DirLightDirection, WorldNormal), 0.0) + max(dot(DirBackLightDirection, WorldNormal), 0.0)) * DirLightDiffuseColor;";
                }
                snprintf(acStack_102c, 0x200, "%s", pcVar6);
                strcat((char *)&vtxbuf, acStack_102c);
            }
            if ((int) (flags << 0x11) < 0) {
                snprintf(acStack_102c, 0x200,
                         "Out_LightingColor += max(dot(DirLight2Direction, WorldNormal), 0.0) * DirLight2DiffuseColor;"
                );
                strcat((char *)&vtxbuf, acStack_102c);
            }
            if ((int) (flags << 0x10) < 0) {
                snprintf(acStack_102c, 0x200,
                         "Out_LightingColor += max(dot(DirLight3Direction, WorldNormal), 0.0) * DirLight3DiffuseColor;"
                );
                strcat((char *)&vtxbuf, acStack_102c);
            }
        }
        if (uVar4 == 0) goto LAB_00073bca;
        if ((int)(flags << 0x1b) < 0) {
            snprintf(acStack_102c,0x200,
                     "Out_Color = vec4((Out_LightingColor.xyz + %s.xyz * 1.5) * MaterialDiffuse.xyz, (MaterialAmbient.w) * %s.w);"
                    ,pcVar5,pcVar5);
        }
        else {
            snprintf(acStack_102c,0x200,
                     "Out_Color = vec4(Out_LightingColor * MaterialDiffuse.xyz, MaterialAmbient.w * %s.w );"
                    ,pcVar5);
        }
        strcat((char *)&vtxbuf,acStack_102c);
        snprintf(acStack_102c,0x200,"Out_Color = clamp(Out_Color, 0.0, 1.0);");
        strcat((char *)&vtxbuf,acStack_102c);
    }
    strcat((char *)&vtxbuf, acStack_102c);
    LAB_00073bca:
    uVar1 = *(uint8_t * )(g_libGTASA + 0x617258);
    if (uVar9 == 0) {
        uVar7 = 1;
    }
    if (uVar1 != 0) {
        uVar1 = 1;
    }
    if ((uVar1 | uVar7) == 0) {
        snprintf(acStack_102c, 0x200,
                 "float specAmt = max(pow(dot(reflVector, DirLightDirection), %.1f), 0.0) * EnvMapCoefficient * 0.5;"
        );
        strcat((char *)&vtxbuf, acStack_102c);
        snprintf(acStack_102c, 0x200, "Out_Spec = specAmt * DirLightDiffuseColor;");
        strcat((char *)&vtxbuf, acStack_102c);
    }
    if ((flags & 0x80000) != 0) {
        snprintf(acStack_102c, 0x200,
                 "Out_WaterDetail = (Out_Tex0 * 4.0) + vec2(WaterSpecs.x * -0.3, WaterSpecs.x * 0.21);");
        strcat((char *)&vtxbuf, acStack_102c);
        snprintf(acStack_102c, 0x200,
                 "Out_WaterDetail2 = (Out_Tex0 * -8.0) + vec2(WaterSpecs.x * 0.12, WaterSpecs.x * -0.05);"
        );
        strcat((char *)&vtxbuf, acStack_102c);
        snprintf(acStack_102c, 0x200,
                 "Out_WaterAlphaBlend = distance(WorldPos.xy, CameraPosition.xy) * WaterSpecs.y;");
        strcat((char *)&vtxbuf, acStack_102c);
    }
    snprintf(acStack_102c, 0x200, "}");
    strcat((char *)&vtxbuf, acStack_102c);
    //Log("test gtasa vert: %s", g_libGTASA + 0x619294);
}

void BuildPixelSource(int flags) {
    uint uVar1;
    char *pcVar2;
    uint uVar3;
    uint uVar4;
    char acStack_102c[4096];
    int iStack_2c;

    snprintf(acStack_102c, 0x200, "#version 100\n");
    strcat((char *)&pxlbuf, acStack_102c);
    snprintf(acStack_102c, 0x200, "precision mediump float;");
    strcat((char *)&pxlbuf, acStack_102c);
    uVar4 = flags & 0x20;
    if (uVar4 != 0) {
        snprintf(acStack_102c, 0x200, "uniform sampler2D Diffuse;");
        strcat((char *)&pxlbuf, acStack_102c);
        snprintf(acStack_102c, 0x200, "varying mediump vec2 Out_Tex0;");
        strcat((char *)&pxlbuf, acStack_102c);
    }
    uVar1 = flags & 0x1000040;
    if (uVar1 == 0) {
        if ((int) (flags << 0xf) < 0) {
            snprintf(acStack_102c, 0x200, "uniform sampler2D EnvMap;");
            strcat((char *)&pxlbuf, acStack_102c);
            pcVar2 = "uniform float DetailTiling;";
            goto LAB_00072780;
        }
    } else {
        snprintf(acStack_102c, 0x200, "uniform sampler2D EnvMap;");
        strcat((char *)&pxlbuf, acStack_102c);
        snprintf(acStack_102c, 0x200, "uniform lowp float EnvMapCoefficient;");
        strcat((char *)&pxlbuf, acStack_102c);
        if ((int) (flags << 0x19) < 0) {
            pcVar2 = "varying mediump vec2 Out_Tex1;";
        } else {
            pcVar2 = "varying mediump vec3 Out_Refl;";
        }
        LAB_00072780:
        snprintf(acStack_102c, 0x200, "%s", pcVar2);
        strcat((char *)&pxlbuf, acStack_102c);
    }
    if ((flags & 0x400) != 0) {
        snprintf(acStack_102c, 0x200, "varying mediump float Out_FogAmt;");
        strcat((char *)&pxlbuf, acStack_102c);
        snprintf(acStack_102c, 0x200, "uniform lowp vec3 FogColor;");
        strcat((char *)&pxlbuf, acStack_102c);
        snprintf(acStack_102c, 0x200, "uniform sampler2D FogMap;");
        strcat((char *)&pxlbuf, acStack_102c);
        snprintf(acStack_102c, 0x200, "varying lowp vec4 Out_ClipSpace;");
        strcat((char *)&pxlbuf, acStack_102c);
    }
    if ((flags & 0x12) != 0) {
        snprintf(acStack_102c, 0x200, "varying lowp vec4 Out_Color;");
        strcat((char *)&pxlbuf, acStack_102c);
    }
    if ((flags & 4) != 0) {
        snprintf(acStack_102c, 0x200, "uniform lowp float AlphaModulate;");
        strcat((char *)&pxlbuf, acStack_102c);
    }
    uVar3 = flags & 0x80000;
    if (uVar3 != 0) {
        snprintf(acStack_102c, 0x200, "varying mediump vec2 Out_WaterDetail;");
        strcat((char *)&pxlbuf, acStack_102c);
        snprintf(acStack_102c, 0x200, "varying mediump vec2 Out_WaterDetail2;");
        strcat((char *)&pxlbuf, acStack_102c);
        snprintf(acStack_102c, 0x200, "varying mediump float Out_WaterAlphaBlend;");
        strcat((char *)&pxlbuf, acStack_102c);
    }
    if ((flags & 0x2000) != 0) {
        snprintf(acStack_102c, 0x200, "varying lowp vec3 Out_Spec;");
        strcat((char *)&pxlbuf, acStack_102c);
    }
    if (uVar4 != 0) {
        snprintf(acStack_102c, 0x200,
                 "bool isSky(vec4 targetColor){vec2 textureCoord = vec2(0.0, 1.0);vec4 colorAtPosition = texture2D(Diffuse, textureCoord);const float colorThreshold = 0.02;return abs(colorAtPosition.r-targetColor.r)<=colorThreshold&&abs(colorAtPosition.g-targetColor.g)<=colorThreshold&&abs(colorAtPosition.b-targetColor.b)<=colorThreshold&&colorAtPosition.a==1.0;} "
        );
        strcat((char *)&pxlbuf, acStack_102c);
    }
    snprintf(acStack_102c, 0x200, "void main() {");
    strcat((char *)&pxlbuf, acStack_102c);
    snprintf(acStack_102c, 0x200, "lowp vec4 fcolor;");
    strcat((char *)&pxlbuf, acStack_102c);
    if ((flags & 0x400) != 0) {
        snprintf(acStack_102c, 0x200,
                 "lowp vec2 ndc = (Out_ClipSpace.xy/Out_ClipSpace.w) / 2.0 + 0.5;");
        strcat((char *)&pxlbuf, acStack_102c);
        snprintf(acStack_102c, 0x200, "lowp vec2 fogTexCoords = vec2(ndc.x, ndc.y);");
        strcat((char *)&pxlbuf, acStack_102c);
        snprintf(acStack_102c, 0x200, "lowp vec4 fogColorNew = texture2D(FogMap, fogTexCoords);");
        strcat((char *)&pxlbuf, acStack_102c);
    }
    if (uVar4 == 0) {
        if ((flags & 0x12) == 0) {
            pcVar2 = "fcolor = 0.0;";
        } else {
            pcVar2 = "fcolor = Out_Color;";
        }
        LAB_00072a7e:
        snprintf(acStack_102c, 0x200, "%s", pcVar2);
        LAB_00072a90:
        strcat((char *)&pxlbuf, acStack_102c);
    } else {
        if ((int) (flags << 0x14) < 0) {
            snprintf(acStack_102c, 0x200,
                     "lowp vec4 diffuseColor = texture2D(Diffuse, Out_Tex0, -2.5);");
        } else if (*(uint8_t *)(g_libGTASA + (VER_x32 ? 0x6B8BA4:0x896138))) {
            snprintf(acStack_102c, 0x200,
                     "lowp vec4 diffuseColor = texture2D(Diffuse, Out_Tex0, -0.5);");
        } else if (!*(uint8_t *)(g_libGTASA + (VER_x32 ? 0x6B8BA4:0x896138))) {
            snprintf(acStack_102c, 0x200,
                     "lowp vec4 diffuseColor = texture2D(Diffuse, Out_Tex0, -0.5);");
        } else {
            snprintf(acStack_102c, 0x200, "lowp vec4 diffuseColor = texture2D(Diffuse, Out_Tex0);");
        }
        strcat((char *)&pxlbuf, acStack_102c);
        snprintf(acStack_102c, 0x200, "lowp vec4 newDiffuseColor = diffuseColor;");
        strcat((char *)&pxlbuf, acStack_102c);
        snprintf(acStack_102c, 0x200, "fcolor = newDiffuseColor;");
        strcat((char *)&pxlbuf, acStack_102c);
        if ((flags & 0x12) == 0) {
            if (uVar3 == 0) goto joined_r0x00072a98;
            pcVar2 = "fcolor.a += Out_WaterAlphaBlend;";
            goto LAB_00072a7e;
        }
        if ((int) (flags << 0xf) < 0) {
            if (uVar3 != 0) {
                snprintf(acStack_102c, 0x200,
                         "float waterDetail = texture2D(EnvMap, Out_WaterDetail, -1.0).x + texture2D(EnvMap, Out_WaterDetail2, -1.0).x;"
                );
                strcat((char *)&pxlbuf, acStack_102c);
                snprintf(acStack_102c, 0x200,
                         "fcolor *= vec4(Out_Color.xyz * waterDetail * 1.1, Out_Color.w);");
                strcat((char *)&pxlbuf, acStack_102c);
                snprintf(acStack_102c, 0x200, "fcolor.a += Out_WaterAlphaBlend;");
                strcat((char *)&pxlbuf, acStack_102c);
                goto joined_r0x00072a98;
            }
            snprintf(acStack_102c, 0x200,
                     "fcolor *= vec4(Out_Color.xyz * texture2D(EnvMap, Out_Tex0.xy * DetailTiling, -0.5).xyz * 2.0, Out_Color.w);"
            );
            goto LAB_00072a90;
        }
        snprintf(acStack_102c, 0x200, "fcolor *= Out_Color;");
        strcat((char *)&pxlbuf, acStack_102c);
        if (uVar3 != 0) {
            snprintf(acStack_102c, 0x200, "fcolor.a += Out_WaterAlphaBlend;");
            goto LAB_00072a90;
        }
    }
    joined_r0x00072a98:
    if ((int) (flags << 0x19) < 0) {
        snprintf(acStack_102c, 0x200,
                 "fcolor.xyz = mix(fcolor.xyz, texture2D(EnvMap, Out_Tex1).xyz, EnvMapCoefficient);");
        strcat((char *)&pxlbuf, acStack_102c);
    }
    if ((int) (flags << 7) < 0) {
        snprintf(acStack_102c, 0x200,
                 "vec2 ReflPos = normalize(Out_Refl.xy) * (Out_Refl.z * 0.5 + 0.5);");
        strcat((char *)&pxlbuf, acStack_102c);
        snprintf(acStack_102c, 0x200, "ReflPos = (ReflPos * vec2(0.5,0.5)) + vec2(0.5,0.5);");
        strcat((char *)&pxlbuf, acStack_102c);
        snprintf(acStack_102c, 0x200, "lowp vec4 ReflTexture = texture2D(EnvMap, ReflPos);");
        strcat((char *)&pxlbuf, acStack_102c);
        snprintf(acStack_102c, 0x200, "float newEnvMapCoef = EnvMapCoefficient + 0.1;");
        strcat((char *)&pxlbuf, acStack_102c);
        snprintf(acStack_102c, 0x200,
                 "fcolor.xyz = mix(fcolor.xyz, ReflTexture.xyz, newEnvMapCoef);");
        strcat((char *)&pxlbuf, acStack_102c);
        snprintf(acStack_102c, 0x200, "fcolor.w += ReflTexture.b * 0.125;");
        strcat((char *)&pxlbuf, acStack_102c);
    }
    if ( (flags & 0x1000040) == 1 && (flags & 0x2000) != 0 && !*(uint8_t *)(g_libGTASA + (VER_x32 ? 0x6B8BA4:0x896138)) ) {
        uVar3 = uVar1;
        if (uVar1 != 0) {
            uVar3 = flags & 0x2000;
        }
        if (uVar1 != 0 && uVar3 != 0) {
            snprintf(acStack_102c, 0x200, "fcolor.xyz += Out_Spec;");
            strcat((char *)&pxlbuf, acStack_102c);
        }
        if ((flags & 0x400) != 0) {
            snprintf(acStack_102c, 0x200,
                     "fcolor.xyz = mix(fcolor.xyz, FogColor.xyz, Out_FogAmt);");
            strcat((char *)&pxlbuf, acStack_102c);
        }
    }
    if ((int) (flags << 5) < 0) {
        snprintf(acStack_102c, 0x200, "fcolor.xyz += fcolor.xyz * 0.5;");
        strcat((char *)&pxlbuf, acStack_102c);
    }
    snprintf(acStack_102c, 0x200, "gl_FragColor = fcolor;");
    strcat((char *)&pxlbuf, acStack_102c);
    if ((flags & 1) == 0) goto LAB_00072d64;
    snprintf(acStack_102c, 0x200, "/*ATBEGIN*/");
    strcat((char *)&pxlbuf, acStack_102c);
    if ((*(int *) (g_libGTASA + (VER_x32 ? 0x5ddd30:0x85DC6C)) == 9) && (uVar4 != 0)) {
        if ((flags & 0x800) != 0) {
            pcVar2 = "if (diffuseColor.a < 0.8) { discard; }";
            goto LAB_00072cce;
        }
        if (-1 < (int) (flags << 0x16)) {
            pcVar2 = "if (diffuseColor.a < 0.2) { discard; }";
            goto LAB_00072cce;
        }
        snprintf(acStack_102c, 0x200, "gl_FragColor.a = Out_Color.a;");
        strcat((char *)&pxlbuf, acStack_102c);
        pcVar2 = "if (diffuseColor.a < 0.5) { discard; }";
        LAB_00072d34:
        snprintf(acStack_102c, 0x200, "%s", pcVar2);
    } else {
        if ((flags & 0x800) == 0) {
            if ((int) (flags << 0x16) < 0) {
                snprintf(acStack_102c, 0x200, "if (gl_FragColor.a < 0.5) { discard; }");
                strcat((char *)&pxlbuf, acStack_102c);
                pcVar2 = "gl_FragColor.a = Out_Color.a;";
                goto LAB_00072d34;
            }
            pcVar2 = "if (gl_FragColor.a < 0.2) { discard; }";
        } else {
            pcVar2 = "if (gl_FragColor.a < 0.8) { discard; }";
        }
        LAB_00072cce:
        snprintf(acStack_102c, 0x200, "%s", pcVar2);
    }
    strcat((char *)&pxlbuf, acStack_102c);
    snprintf(acStack_102c, 0x200, "/*ATEND*/");
    strcat((char *)&pxlbuf, acStack_102c);
    LAB_00072d64:
    if ((flags & 4) != 0) {
        snprintf(acStack_102c, 0x200, "gl_FragColor.a *= AlphaModulate;");
        strcat((char *)&pxlbuf, acStack_102c);
    }
    if (uVar4 != 0) {
        snprintf(acStack_102c, 0x200,
                 "if(isSky(vec4(95.0/255.0 * 1.2, 144.0/255.0 * 1.2, 188.0/255.0 * 1.2, 1.0)) || "
                 "isSky(vec4(125.0/255.0 * 1.2, 152.0/255.0 * 1.2, 212.0/255.0 * 1.2, 1.0)) || "
                 "isSky(vec4(164.0/255.0 * 1.2, 110.0/255.0 * 1.2, 91.0/255.0 * 1.2, 1.0)) || "
                 "isSky(vec4(20.0/255.0 * 1.2, 21.0/255.0 * 1.2, 28.0/255.0 * 1.2, 1.0))) { "
                 "gl_FragColor.rgb = diffuseColor.rgb; }"
        );
        strcat((char *)&pxlbuf, acStack_102c);
    }
    snprintf(acStack_102c, 0x200, "}");
    strcat((char *)&pxlbuf, acStack_102c);
    //Log("test gtasa pix: %s",g_libGTASA + 0x617290);
}

int RQShader__BuildSource(int flags, char **pxlsrc, char **vtxsrc) {
    pxlbuf[0] = '\0';
    vtxbuf[0] = '\0';

    BuildPixelSource(flags);
    BuildVertexSource(flags);

    *pxlsrc = pxlbuf;
    *vtxsrc = vtxbuf;

    return 1;
}

void SetUpGLHooks()
{
    CHook::Redirect("_ZN8RQShader11BuildSourceEjPPKcS2_", &RQShader__BuildSource);
}