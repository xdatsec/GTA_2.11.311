#pragma once

#include "main.h"

#define EXCEPTION_CHECK(env) \
	if ((env)->ExceptionCheck()) \ 
	{ \
		(env)->ExceptionDescribe(); \
		(env)->ExceptionClear(); \
		return; \
	}

class CJavaWrapper
{
public:
    CJavaWrapper(JNIEnv *env, jobject activity);
    ~CJavaWrapper() {};
    JNIEnv* GetEnv();
    
    const char* GetClipboardString();
    void SetPauseState(bool pause);
    void ShowLoadingScreen();
    void HideLoadingScreen();

    void ShowTabWindow();
    void SetTabStat(int id, char* name, int score, int ping);

    
    void ShowKeyboard();
    void HideKeyboard();

    void UpdateHudInfo(int health, int armour, int weaponid, int ammo, int ammoinclip, int money, int eat, int drink, int bankMoney, int playerid);
    void ShowHud();
    void HideHud();

    void UpdateSpeedInfo(int speed, int fuel, int hp, int mileage, int engine, int light, int belt, int lock);
    void ShowSpeed();
    void HideSpeed();

	void exitGame();

    jbyteArray as_byte_array(unsigned char* buf, int len) {
        JNIEnv* p;
        javaVM->GetEnv((void**)&p, JNI_VERSION_1_6);
		jbyteArray array = p->NewByteArray (len);
		p->SetByteArrayRegion (array, 0, len, reinterpret_cast<jbyte*>(buf));
		return array;
	}

    void ShowDialog(int dialogStyle, int dialogID, char* title, char* text, char* button1, char* button2);

	void ShowEditObject();
	void HideEditObject();

    jobject activity;
    jmethodID s_setPauseState;
    jmethodID s_showLoadingScreen;
    jmethodID s_hideLoadingScreen;
    jmethodID s_ShowDialog;
    jmethodID s_showInputLayout;
    jmethodID s_hideInputLayout;
	jmethodID s_exitGame;
	jmethodID s_showEditObject;
	jmethodID s_hideEditObject;
    jmethodID s_showTabWindow;
    jmethodID s_setTabStat;
    jmethodID s_showHud;
    jmethodID s_hideHud;
    jmethodID s_updateHudInfo;

    jmethodID s_updateSpeedInfo;
    jmethodID s_showSpeed;
    jmethodID s_hideSpeed;
};

extern CJavaWrapper* g_pJavaWrapper;