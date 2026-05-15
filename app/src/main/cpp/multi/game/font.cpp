#include "../main.h"
#include "font.h"
#include "patch.h"

void CFont::Initialise() {
    CHook::CallFunction<void>(g_libGTASA + 0x53F24C);
}

void CFont::AsciiToGxtChar(const char* ascii, uint16_t* gxt)
{
    return ((void(*)(const char*, uint16_t*))(g_libGTASA + /*0x532D00*/0x5B7858))(ascii, gxt);
}


void CFont::SetScale(float x, float y)
{
    *(float*)(g_libGTASA + 0xC58938 + 0x4) = x;
    *(float*)(g_libGTASA + 0xC58938 + 0x8) = y;
}

void CFont::SetColor(uint32_t *dwColor)
{
    return ((void(*)(uint32_t*))(g_libGTASA + 0x5BA544))(dwColor);
}

void CFont::SetJustify(uint8_t justify)
{
    return ((void(*)(uint8_t))(g_libGTASA + 0x5BA930))(justify);
}

void CFont::SetOrientation(uint8_t orientation)
{
    return ((void(*)(uint8_t))(g_libGTASA + 0x5BA940))(orientation);
}

void CFont::SetWrapX(float wrapX)
{
    return ((void(*)(float))(g_libGTASA + 0x5BA80C))(wrapX);
}

void CFont::SetCentreSize(float size)
{
    return ((void(*)(float))(g_libGTASA + 0x5BA81C))(size);
}

void CFont::SetBackground(uint8_t bBackground, uint8_t bOnlyText)
{
    return ((void(*)(uint8_t, uint8_t))(g_libGTASA + 0x5BA8F0))(bBackground, bOnlyText);
}

void CFont::SetBackgroundColor(uint32_t *dwColor)
{
    return ((void(*)(uint32_t*))(g_libGTASA + 0x5BA904))(dwColor);
}

void CFont::SetProportional(uint8_t prop)
{
    return ((void(*)(uint8_t))(g_libGTASA + 0x5BA8E0))(prop);
}

void CFont::SetDropColor(uint32_t* dwColor)
{
    return ((void(*)(uint32_t*))(g_libGTASA + 0x5BA84C))(dwColor);
}

void CFont::SetDropShadowPosition(uint8_t pos)
{
    return ((void(*)(uint8_t))(g_libGTASA + 0x5B80C0))(pos);
}

void CFont::PrintString(float fX, float fY, const uint16_t* szText)
{
    return ((void(*)(float, float, const uint16_t*))(g_libGTASA + 0x5B954C))(fX, fY, szText);
}

/*void CFont::PrintString(float posX, float posY, const char* string)
{
	uint16_t* gxt_string = new uint16_t[0xFF];
	CFont::AsciiToGxtChar(string, gxt_string);
	((void (*)(float, float, uint16_t*))(g_libGTASA + 0x5AA190 + 1))(posX, posY, gxt_string);
	delete gxt_string;
	((void (*)())(g_libGTASA + 0x5A90B0 + 1))();//53411C ; _DWORD CFont::RenderFontBuffer(CFont *__hidden this)
}*/

void CFont::SetFontStyle(uint8_t style)
{
    return ((void(*)(uint8_t))(g_libGTASA + 0x5BA750))(style);
}

void CFont::SetEdge(uint8_t edge)
{
    return ((void(*)(uint8_t))(g_libGTASA + 0x5BA8AC))(edge);
}