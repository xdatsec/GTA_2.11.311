#include "../main.h"
#include "font.h"
#include "../vendor/armhook/patch.h"

void CFont::Initialise() {
	CHook::CallFunction<void>(g_libGTASA + 0x55BC78);
}

void CFont::AsciiToGxtChar(const char* ascii, uint16_t* gxt)
{
	return ((void(*)(const char*, uint16_t*))(g_libGTASA + 0x5D5254))(ascii, gxt);
}

void CFont::SetScale(float x, float y)
{
	*(float*)(g_libGTASA + 0xC72090 + 0x4) = x;
	*(float*)(g_libGTASA + 0xC72090 + 0x8) = y;
}

void CFont::SetColor(uint32_t* dwColor)
{
	return ((void(*)(uint32_t*))(g_libGTASA + 0x5D7E54))(dwColor);
}

void CFont::SetJustify(uint8_t justify)
{
	return ((void(*)(uint8_t))(g_libGTASA + 0x5D823C))(justify);
}

void CFont::SetOrientation(uint8_t orientation)
{
	return ((void(*)(uint8_t))(g_libGTASA + 0x5D824C))(orientation);
}

void CFont::SetWrapX(float wrapX)
{
	return ((void(*)(float))(g_libGTASA + 0x6CF0B0))(wrapX);
}

void CFont::SetCentreSize(float size)
{
	return ((void(*)(float))(g_libGTASA + 0x5D8118))(size);
}
void Font_SetRightJustifyWrap(float wrap)
{
	//?wrong address
	//((int (*)(float))(g_libGTASA + (VER_x32 ? 0x05AB268 + 1 : 0x0053384C)))(wrap);
	//0053384C
}

void CFont::SetBackground(uint8_t bBackground, uint8_t bOnlyText)
{
	return ((void(*)(uint8_t, uint8_t))(g_libGTASA + 0x5D81FC))(bBackground, bOnlyText);
}

void CFont::SetBackgroundColor(uint32_t* dwColor)
{
	return ((void(*)(uint32_t*))(g_libGTASA + 0x5D8210))(dwColor);
}

void CFont::SetProportional(uint8_t prop)
{
	return ((void(*)(uint8_t))(g_libGTASA + 0x5D81EC))(prop);
}

void CFont::SetDropColor(uint32_t* dwColor)
{
	return ((void(*)(uint32_t*))(g_libGTASA + 0x5D8148))(dwColor);
}

void CFont::SetDropShadowPosition(uint8_t pos)
{
	return ((void(*)(uint8_t))(g_libGTASA + 0x5D5A50))(pos);
}

/*
void CFont::PrintString(float fX, float fY, const uint16_t* szText)
{
	return ((void(*)(float, float, const uint16_t*))(g_libGTASA + 0x5353B4 + 1))(fX, fY, szText);
}
*/

void CFont::PrintString(float posX, float posY, const char* string)
{
	uint16_t* gxt_string = new uint16_t[0xFF];
	CFont::AsciiToGxtChar(string, gxt_string);
	((void (*)(float, float, uint16_t*))(g_libGTASA + 0x5D6EC4))(posX, posY, gxt_string);
	delete gxt_string;
	((void (*)())(g_libGTASA + 0x5D6090))();//53411C ; _DWORD CFont::RenderFontBuffer(CFont *__hidden this)
}

void CFont::SetFontStyle(uint8_t style)
{
	return ((void(*)(uint8_t))(g_libGTASA + 0x5D8054))(style);
}

void CFont::SetEdge(uint8_t edge)
{
	return ((void(*)(uint8_t))(g_libGTASA + 0x5D81B4))(edge);
}