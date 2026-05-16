#include "jniutil.h"
#include "game/game.h"
extern CGame *pGame;

JNIEnv* CJavaWrapper::GetEnv()
{
    JNIEnv* env = nullptr;
    int getEnvStat = javaVM->GetEnv((void**)& env, JNI_VERSION_1_4);

    if (getEnvStat == JNI_EDETACHED)
    {
        FLog("GetEnv: not attached");
        if (javaVM->AttachCurrentThread(&env, NULL) != 0)
        {
            FLog("Failed to attach");
            return nullptr;
        }
    }
    if (getEnvStat == JNI_EVERSION)
    {
        FLog("GetEnv: version not supported");
        return nullptr;
    }

    if (getEnvStat == JNI_ERR)
    {
        FLog("GetEnv: JNI_ERR");
        return nullptr;
    }

    return env;
}

CJavaWrapper::CJavaWrapper(JNIEnv *env, jobject activity)
{
    this->activity = env->NewGlobalRef(activity);

    jclass clas = env->GetObjectClass(activity);
    if(!clas)
    {
        FLog("no clas");
        return;
    }

    s_showLoadingScreen = env->GetMethodID(clas, "showLoadingScreen", "()V");
    s_hideLoadingScreen = env->GetMethodID(clas, "hideLoadingScreen", "()V");

    s_setPauseState = env->GetMethodID(clas, "setPauseState", "(Z)V");
    
    s_ShowDialog = env->GetMethodID(clas, "showDialog", "(II[B[B[B[B)V");

	s_showInputLayout = env->GetMethodID(clas, "showKeyboard", "()V");
    s_hideInputLayout = env->GetMethodID(clas, "hideKeyboard", "()V");

    s_exitGame = env->GetMethodID(clas, "exitGame", "()V");

    s_showEditObject = env->GetMethodID(clas, "showEditObject", "()V");
    s_hideEditObject = env->GetMethodID(clas, "hideEditObject", "()V");

    s_showTabWindow = env->GetMethodID(clas, "showTabWindow", "()V");
    s_setTabStat = env->GetMethodID(clas, "setTabStat", "(ILjava/lang/String;II)V");

    s_updateHudInfo = env->GetMethodID(clas, "updateHudInfo", "(IIIIIIIIII)V");
    s_showHud = env->GetMethodID(clas, "showHud", "()V");
    s_hideHud = env->GetMethodID(clas, "hideHud", "()V");

    s_updateSpeedInfo = env->GetMethodID(clas, "updateSpeedInfo", "(IIIIIIII)V");
    s_showSpeed = env->GetMethodID(clas, "showSpeed", "()V");
    s_hideSpeed = env->GetMethodID(clas, "hideSpeed", "()V");

    env->DeleteLocalRef(clas);
}

void CJavaWrapper::ShowKeyboard()
{
    JNIEnv* p;
    javaVM->GetEnv((void**)&p, JNI_VERSION_1_6);
    p->CallVoidMethod(activity, s_showInputLayout);
    EXCEPTION_CHECK(p);
}

void CJavaWrapper::HideKeyboard()
{
    JNIEnv* p;
    javaVM->GetEnv((void**)&p, JNI_VERSION_1_6);
    p->CallVoidMethod(activity, s_hideInputLayout);
    EXCEPTION_CHECK(p);
}

void CJavaWrapper::ShowLoadingScreen()
{
    JNIEnv* p;
    javaVM->GetEnv((void**)&p, JNI_VERSION_1_6);
    p->CallVoidMethod(activity, s_showLoadingScreen);
    EXCEPTION_CHECK(p);
}

void CJavaWrapper::HideLoadingScreen()
{
    JNIEnv* p;
    javaVM->GetEnv((void**)&p, JNI_VERSION_1_6);
    p->CallVoidMethod(activity, s_hideLoadingScreen);
    EXCEPTION_CHECK(p);
}

void CJavaWrapper::SetPauseState(bool pause)
{
    JNIEnv* p;
    javaVM->GetEnv((void**)&p, JNI_VERSION_1_6);
    p->CallVoidMethod(activity, s_setPauseState, pause);
    EXCEPTION_CHECK(p);
}

void CJavaWrapper::ShowDialog(int dialogStyle, int dialogID, char* title, char* text, char* button1, char* button2)
{
	JNIEnv* env;
    javaVM->GetEnv((void**)&env, JNI_VERSION_1_6);

	if (!env)
	{
		FLog("No env");
		return;
	}

	std::string sTitle(title);
	std::string sText(text);
	std::string sButton1(button1);
	std::string sButton2(button2);

	jbyteArray jstrTitle = as_byte_array((unsigned char*)sTitle.c_str(), sTitle.length());
	jbyteArray jstrText = as_byte_array((unsigned char*)sText.c_str(), sText.length());
	jbyteArray jstrButton1 = as_byte_array((unsigned char*)sButton1.c_str(), sButton1.length());
	jbyteArray jstrButton2 = as_byte_array((unsigned char*)sButton2.c_str(), sButton2.length());

	env->CallVoidMethod(activity, s_ShowDialog, dialogID, dialogStyle, jstrTitle, jstrText, jstrButton1, jstrButton2);

	EXCEPTION_CHECK(env);
}

void CJavaWrapper::UpdateHudInfo(int health, int armour, int weaponid, int ammo, int ammoinclip,
                                 int money, int eat, int drink, int bankMoney, int playerid)
{
    JNIEnv* env;
    javaVM->GetEnv((void**)&env, JNI_VERSION_1_6);

    if (!env)
    {
        FLog("No env");
        return;
    }

    env->CallVoidMethod(this->activity, this->s_updateHudInfo,
                        health, armour, weaponid, ammo, ammoinclip,
                        money, eat, drink, bankMoney, playerid);
}

void CJavaWrapper::ShowHud()
{
    JNIEnv* env;
    javaVM->GetEnv((void**)&env, JNI_VERSION_1_6);

    if (!env)
    {
        FLog("No env");
        return;
    }
    //g_pJavaWrapper->ShowNotification(4, "HUD показан.", 5, "", "Хорошо");
   // env->CallVoidMethod(this->activity, this->s_showHud);
}

void CJavaWrapper::HideHud()
{
    JNIEnv* env;
    javaVM->GetEnv((void**)&env, JNI_VERSION_1_6);

    if (!env)
    {
        FLog("No env");
        return;
    }
    //g_pJavaWrapper->ShowNotification(4, "HUD скрыт.", 5, "", "Хорошо");
   // env->CallVoidMethod(this->activity, this->s_hideHud);
}

void CJavaWrapper::ShowSpeed()
{
    JNIEnv* env;
    javaVM->GetEnv((void**)&env, JNI_VERSION_1_6);

    if (!env)
    {
        FLog("No env");
        return;
    }
    env->CallVoidMethod(this->activity, this->s_showSpeed);
}

void CJavaWrapper::HideSpeed()
{
    JNIEnv* env;
    javaVM->GetEnv((void**)&env, JNI_VERSION_1_6);

    if (!env)
    {
        FLog("No env");
        return;
    }
    env->CallVoidMethod(this->activity, this->s_hideSpeed);
}

void CJavaWrapper::UpdateSpeedInfo(int speed, int fuel, int hp, int mileage, int engine, int light, int belt, int lock)
{
    JNIEnv* env;
    javaVM->GetEnv((void**)&env, JNI_VERSION_1_6);;

    if (!env)
    {
        FLog("No env");
        return;
    }

    env->CallVoidMethod(this->activity, this->s_updateSpeedInfo, speed, fuel, hp, mileage, engine, light, belt, lock);
}

void CJavaWrapper::ShowTabWindow()
{
    JNIEnv* env;
    javaVM->GetEnv((void**)&env, JNI_VERSION_1_6);

    if (!env)
    {
        FLog("No env");
        return;
    }
    env->CallVoidMethod(this->activity, this->s_showTabWindow);
}

void CJavaWrapper::SetTabStat(int id, char* name, int score, int ping) {

    JNIEnv* env;
    javaVM->GetEnv((void**)&env, JNI_VERSION_1_6);

    if (!env)
    {
        FLog("No env");
        return;
    }

    jclass strClass = env->FindClass("java/lang/String");
    jmethodID ctorID = env->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");
    jstring encoding = env->NewStringUTF("UTF-8");

    jbyteArray bytes = env->NewByteArray(strlen(name));
    env->SetByteArrayRegion(bytes, 0, strlen(name), (jbyte*)name);
    jstring jname = (jstring) env->NewObject(strClass, ctorID, bytes, encoding);

    env->CallVoidMethod(this->activity, this->s_setTabStat, id, jname, score, ping);
}

void CJavaWrapper::exitGame() {

    JNIEnv* env;
    javaVM->GetEnv((void**)&env, JNI_VERSION_1_6);

    if (!env)
    {
        FLog("No env");
        return;
    }

    env->CallVoidMethod(this->activity, this->s_exitGame);
}

void CJavaWrapper::ShowEditObject() {

    JNIEnv* env;
    javaVM->GetEnv((void**)&env, JNI_VERSION_1_6);

    if (!env)
    {
        FLog("No env");
        return;
    }

    env->CallVoidMethod(this->activity, this->s_showEditObject);
}

void CJavaWrapper::HideEditObject() {

    JNIEnv* env;
    javaVM->GetEnv((void**)&env, JNI_VERSION_1_6);

    if (!env)
    {
        FLog("No env");
        return;
    }

    env->CallVoidMethod(this->activity, this->s_hideEditObject);
}