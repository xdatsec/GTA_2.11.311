#include "../../main.h"
#include "RenderWare.h"
#include "../vendor/armhook//patch.h"

RsGlobalType* RsGlobal;

/* rwcore.h */
RwCamera* (*RwCameraBeginUpdate)(RwCamera* camera);
RwCamera* (*RwCameraShowRaster)(RwCamera * camera, void *pDev, RwUInt32 flags);

RwRaster* 	(*RwRasterCreate)(RwInt32 width, RwInt32 height, RwInt32 depth, RwInt32 flags);
RwBool 		(*RwRasterDestroy)(RwRaster * raster);
RwRaster* 	(*RwRasterGetOffset)(RwRaster *raster, RwInt16 *xOffset, RwInt16 *yOffset);
RwInt32 	(*RwRasterGetNumLevels)(RwRaster * raster);
RwRaster* 	(*RwRasterSubRaster)(RwRaster * subRaster, RwRaster * raster, RwRect * rect);
RwRaster* 	(*RwRasterRenderFast)(RwRaster * raster, RwInt32 x, RwInt32 y);
RwRaster* 	(*RwRasterRender)(RwRaster * raster, RwInt32 x, RwInt32 y);
RwRaster* 	(*RwRasterRenderScaled)(RwRaster * raster, RwRect * rect);
RwRaster* 	(*RwRasterPushContext)(RwRaster * raster);
RwRaster* 	(*RwRasterPopContext)(void);
RwRaster* 	(*RwRasterGetCurrentContext)(void);
RwBool 		(*RwRasterClear)(RwInt32 pixelValue);
RwBool 		(*RwRasterClearRect)(RwRect * rpRect, RwInt32 pixelValue);
RwRaster* 	(*RwRasterShowRaster)(RwRaster * raster, void *dev, RwUInt32 flags);
RwUInt8* 	(*RwRasterLock)(RwRaster * raster, RwUInt8 level, RwInt32 lockMode);
RwRaster* 	(*RwRasterUnlock)(RwRaster * raster);
RwUInt8* 	(*RwRasterLockPalette)(RwRaster * raster, RwInt32 lockMode);
RwRaster* 	(*RwRasterUnlockPalette)(RwRaster * raster);
RwImage* 	(*RwImageCreate)(RwInt32 width, RwInt32 height, RwInt32 depth);
RwBool 		(*RwImageDestroy)(RwImage * image);
RwImage* 	(*RwImageAllocatePixels)(RwImage * image);
RwImage* 	(*RwImageFreePixels)(RwImage * image);
RwImage* 	(*RwImageCopy)(RwImage * destImage, const RwImage * sourceImage);
RwImage* 	(*RwImageResize)(RwImage * image, RwInt32 width, RwInt32 height);
RwImage* 	(*RwImageApplyMask)(RwImage * image, const RwImage * mask);
RwImage* 	(*RwImageMakeMask)(RwImage * image);
RwImage* 	(*RwImageReadMaskedImage)(const RwChar * imageName, const RwChar * maskname);
RwImage* 	(*RwImageRead)(const RwChar * imageName);
RwImage* 	(*RwImageWrite)(RwImage * image, const RwChar * imageName);
RwImage* 	(*RwImageSetFromRaster)(RwImage *image, RwRaster *raster);
RwRaster* 	(*RwRasterSetFromImage)(RwRaster *raster, RwImage *image);
RwRaster* 	(*RwRasterRead)(const RwChar *filename);
RwRaster* 	(*RwRasterReadMaskedRaster)(const RwChar *filename, const RwChar *maskname);
RwImage* 	(*RwImageFindRasterFormat)(RwImage *ipImage, RwInt32 nRasterType, RwInt32 *npWidth, RwInt32 *npHeight, RwInt32 *npDepth,RwInt32 *npFormat);

/* rwlpcore.h */
RwReal 		(*RwIm2DGetNearScreenZ)(void);
RwReal 		(*RwIm2DGetFarScreenZ)(void);
RwBool 		(*RwRenderStateGet)(RwRenderState state, void *value);
RwBool 		(*RwRenderStateSet)(RwRenderState state, void *value);
RwBool 		(*RwIm2DRenderLine)(RwIm2DVertex *vertices, RwInt32 numVertices, RwInt32 vert1, RwInt32 vert2);
RwBool 		(*RwIm2DRenderTriangle)(RwIm2DVertex *vertices, RwInt32 numVertices, RwInt32 vert1, RwInt32 vert2, RwInt32 vert3 );
RwBool 		(*RwIm2DRenderPrimitive)(RwPrimitiveType primType, RwIm2DVertex *vertices, RwInt32 numVertices);
RwBool 		(*RwIm2DRenderIndexedPrimitive)(RwPrimitiveType primType, RwIm2DVertex *vertices, RwInt32 numVertices, RwImVertexIndex *indices, RwInt32 numIndices);

/* rtpng.h */
RwImage*	(*RtPNGImageWrite)(RwImage* image, const RwChar* imageName);
RwImage* 	(*RtPNGImageRead)(const RwChar* imageName);

RwBool (*RwTextureDestroy)(RwTexture* texture);
void WriteMemory(uintptr_t dest, uintptr_t src, size_t size);
void InitRenderWareFunctions()
{
	FLog("Initializing RenderWare..");

	/* skeleton.h */
	RsGlobal = (RsGlobalType*)(g_libGTASA + 0xCB70C8);

	/* rwCore.h */
	*(void**)(&RwCameraBeginUpdate) 			= (void*)(g_libGTASA+ 0x755D0C);
	*(void**)(&RwCameraShowRaster)				= (void*)(g_libGTASA+0x755F4C);

	*(void **)(&RwRasterCreate) 				= (void*)(g_libGTASA+0x75C1E4);
	*(void **)(&RwRasterDestroy) 				= (void*)(g_libGTASA+0x75BEAC);
	*(void **)(&RwRasterGetOffset) 				= (void*)(g_libGTASA+0x75BC6C);
	*(void **)(&RwRasterGetNumLevels) 			= (void*)(g_libGTASA+0x75C088);
	*(void **)(&RwRasterSubRaster) 				= (void*)(g_libGTASA+0x75C15C);
	*(void **)(&RwRasterRenderFast)				= (void*)(g_libGTASA+0x75BD68);
	*(void **)(&RwRasterRender)					= (void*)(g_libGTASA+0x75BE04);
	*(void **)(&RwRasterRenderScaled)			= (void*)(g_libGTASA+0x75BC34);
	*(void **)(&RwRasterPushContext)			= (void*)(g_libGTASA+0x75BF1C);
	*(void **)(&RwRasterPopContext)				= (void*)(g_libGTASA+0x75C020);
	*(void **)(&RwRasterGetCurrentContext)		= (void*)(g_libGTASA+0x75BC10);
	*(void **)(&RwRasterClear)					= (void*)(g_libGTASA+0x75BCC0);
	*(void **)(&RwRasterClearRect)				= (void*)(g_libGTASA+0x75BDD0);
	*(void **)(&RwRasterShowRaster)				= (void*)(g_libGTASA+0x75C104);
	*(void **)(&RwRasterLock)					= (void*)(g_libGTASA+0x75C2C8);
	*(void **)(&RwRasterUnlock)					= (void*)(g_libGTASA+0x75BC80);
	*(void **)(&RwRasterLockPalette)			= (void*)(g_libGTASA+0x75BFB4);
	*(void **)(&RwRasterUnlockPalette)			= (void*)(g_libGTASA+0x75BE6C);
	*(void **)(&RwImageCreate)					= (void*)(g_libGTASA+0x759ACC);
	*(void **)(&RwImageDestroy)					= (void*)(g_libGTASA+0x759B44);
	*(void **)(&RwImageAllocatePixels)			= (void*)(g_libGTASA+0x759C08);
	*(void **)(&RwImageFreePixels)				= (void*)(g_libGTASA+0x759BC0);
	*(void **)(&RwImageCopy)					= (void*)(g_libGTASA+0x75A470);
	*(void **)(&RwImageResize)					= (void*)(g_libGTASA+0x759D14);
	*(void **)(&RwImageApplyMask)				= (void*)(g_libGTASA+0x75A090);
	*(void **)(&RwImageMakeMask)				= (void*)(g_libGTASA+0x759EEC);
	*(void **)(&RwImageReadMaskedImage)			= (void*)(g_libGTASA+0x75AFDC);
	*(void **)(&RwImageRead)					= (void*)(g_libGTASA+0x75A814);
	*(void **)(&RwImageWrite)					= (void*)(g_libGTASA+0x75AF0C);
	*(void **)(&RwImageSetFromRaster)			= (void*)(g_libGTASA+0x75B840);
	*(void **)(&RwRasterSetFromImage)			= (void*)(g_libGTASA+0x75B87C);
	*(void **)(&RwRasterRead)					= (void*)(g_libGTASA+0x75BA68);
	*(void **)(&RwRasterReadMaskedRaster)		= (void*)(g_libGTASA+0x75BB88);
	*(void **)(&RwImageFindRasterFormat)		= (void*)(g_libGTASA+0x75B8B8);

	/* rwlpcore.h */
	*(void **)(&RwIm2DGetNearScreenZ)			= (void*)(g_libGTASA+0x74DFFC);
	*(void **)(&RwIm2DGetFarScreenZ)			= (void*)(g_libGTASA+0x74E010);
	*(void **)(&RwRenderStateGet)				= (void*)(g_libGTASA+0x74E0B4);
	*(void **)(&RwRenderStateSet)				= (void*)(g_libGTASA+0x74E024);
	*(void **)(&RwIm2DRenderLine)				= (void*)(g_libGTASA+0x5E2948);
	*(void **)(&RwIm2DRenderTriangle)			= (void*)(g_libGTASA+0x5E2944);
	*(void **)(&RwIm2DRenderPrimitive)			= (void*)(g_libGTASA+0x5E293C);
	*(void **)(&RwIm2DRenderIndexedPrimitive)	= (void*)(g_libGTASA+0x5E2940);

	/* rtpng.h */
	*(void **)(&RtPNGImageWrite)				= (void*)(g_libGTASA+0x77C6AC);
	*(void **)(&RtPNGImageRead)					= (void*)(g_libGTASA+0x77CB14);

	*(void**)(&RwTextureDestroy) 				= (void*)(g_libGTASA+0x75D198);
}