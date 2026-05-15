#include "main.h"
#include "game/game.h"
#include "net/netgame.h"
#include "jniutil.h"

extern CGame *pGame;
extern UI *pUI;
extern CNetGame *pNetGame;

bool bNeedEnterVehicle = false;
bool OpenButton = false;
extern bool Mchat;

extern "C" {
	JNIEXPORT void JNICALL Java_com_sampmobile_game_main_ui_RadialMenu_sendCommand(JNIEnv* pEnv, jobject thiz, jbyteArray str)
	{
		jboolean isCopy = true;

		jbyte* pMsg = pEnv->GetByteArrayElements(str, &isCopy);
		jsize length = pEnv->GetArrayLength(str);

		std::string szStr((char*)pMsg, length);

		if(pNetGame) {
			pNetGame->SendChatCommand((char*)szStr.c_str());
		}

		pEnv->ReleaseByteArrayElements(str, pMsg, JNI_ABORT);
	}
}

extern "C" {
	JNIEXPORT void JNICALL Java_com_sampmobile_game_main_ui_RadialVehicles_sendCommandV(JNIEnv* pEnv, jobject thiz, jbyteArray str)
	{
		jboolean isCopy = true;

		jbyte* pMsg = pEnv->GetByteArrayElements(str, &isCopy);
		jsize length = pEnv->GetArrayLength(str);

		std::string szStr((char*)pMsg, length);

		if(pNetGame) {
			pNetGame->SendChatCommand((char*)szStr.c_str());
		}

		pEnv->ReleaseByteArrayElements(str, pMsg, JNI_ABORT);
	}
}
extern "C"
{
	JNIEXPORT void JNICALL
	Java_com_sampmobile_game_main_SAMP_ClickEnterPassengerButton(JNIEnv *env, jobject thiz) 
	{
		bNeedEnterVehicle = true;
	}
}
extern "C"
{
	JNIEXPORT void JNICALL
	Java_com_sampmobile_game_main_SAMP_ClickLockVehicleButton(JNIEnv *env, jobject thiz) 
	{
		pNetGame->SendChatCommand("/trancar");
	}
}

extern "C" {
	JNIEXPORT void JNICALL Java_com_sampmobile_game_main_SAMP_onClickButton(JNIEnv* pEnv, jobject thiz, jint action)
	{
		switch(action) 
		{
			case 1: // ALT
			{
				CPlayerPool* pPlayerPool = pNetGame->GetPlayerPool();
				if (pPlayerPool)
				{
					CLocalPlayer* pLocalPlayer;
					if (!pPlayerPool->GetLocalPlayer()->GetPlayerPed()->IsInVehicle() && !pPlayerPool->GetLocalPlayer()->GetPlayerPed()->IsAPassenger())
						LocalPlayerKeys.bKeys[ePadKeys::KEY_WALK] = true;
					else
						LocalPlayerKeys.bKeys[ePadKeys::KEY_FIRE] = true;
				}	
				break;
			}
			case 2: // F
				LocalPlayerKeys.bKeys[ePadKeys::KEY_SECONDARY_ATTACK] = true;
			break;
		}
	}		
}

extern "C"
{
	JNIEXPORT void JNICALL
	Java_com_sampmobile_game_main_SAMP_MostrarChat(JNIEnv *env, jobject thiz) 
	{
		if (Mchat == false) {
			if (pUI) { pUI->chat()->setVisible(true); Mchat = true; }
		}
		else {
			if (pUI) { pUI->chat()->setVisible(false); Mchat = false; }
		} 
	}
}

/*
		if (Tab == 0) {
			if (pUI) { pUI->playertablist()->show(); Tab = 1; }
		}
		else {
			Tab = 0;
		} 
*/