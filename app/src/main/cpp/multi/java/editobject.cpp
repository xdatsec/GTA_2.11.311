//
// Created by admin on 17.08.2023.
//

#include "editobject.h"
#include <jni.h>
#include "main.h"

#include "../game/game.h"
#include "net/netgame.h"
#include "gui/gui.h"
#include "jniutil.h"

extern CJavaWrapper *pJavaWrapper;
extern UI *pUI;
extern CNetGame *pNetGame;

void CObjectEditor::startEditPlayerAttach(int slot)
{
    if(!pNetGame->GetPlayerPool()->GetLocalPlayer()->GetPlayerPed()->GetObjectSlotState(slot)){
        pUI->chat()->addDebugMessage("Invalid attach slot %d", slot);
        return;
    }
    CObjectEditor::iEditedId = slot;
    CObjectEditor::editType = TYPE_PLAYER_ATTACH;
    CObjectEditor::time = GetTickCount();

    CObjectEditor::showGui();
}

void CObjectEditor::startEditObject(uint16_t objectId)
{
    auto pObject = pNetGame->GetObjectPool()->GetAt(objectId);
    if(!pObject) {
        pUI->chat()->addDebugMessage("Invalid object id %d", objectId);
        return;
    }
    CObjectEditor::iEditedId = objectId;

    CObjectEditor::editType = TYPE_OBJECT;

    CObjectEditor::time = GetTickCount();

    CObjectEditor::showGui();
}

void CObjectEditor::showGui() {
    pJavaWrapper->ShowEditObject();

    bIsToggle = true;
}


extern "C"
JNIEXPORT void JNICALL
Java_com_sampmobile_game_main_ui_AttachEdit_Exit(JNIEnv *env, jobject thiz) {
    CPlayerPed* pPlayer = pNetGame->GetPlayerPool()->GetLocalPlayer()->GetPlayerPed();
    int slot = CObjectEditor::iEditedId;

    CObjectEditor::iEditedId = INVALID_EDITED_SLOT;
    if(CObjectEditor::editType == CObjectEditor::TYPE_PLAYER_ATTACH) {
        CObjectEditor::SendOnEditAttach(
                0,
                slot,
                pPlayer->m_attachedObjectInfo[slot].iModel,
                pPlayer->m_attachedObjectInfo[slot].iBoneID,
                pPlayer->m_attachedObjectInfo[slot].vecOffset,
                pPlayer->m_attachedObjectInfo[slot].vecRot,
                pPlayer->m_attachedObjectInfo[slot].vecScale
        );
    }

    if(CObjectEditor::editType == CObjectEditor::TYPE_OBJECT)
    {
        auto pObject = pNetGame->GetObjectPool()->GetAt(CObjectEditor::iEditedId);
        if(!pObject)
        {
            CObjectEditor::bIsToggle = false;
            pJavaWrapper->HideEditObject();
        }

        MATRIX4X4 matrix4X4;
        pObject->GetMatrix(&matrix4X4);
        auto pos = matrix4X4.pos;
        VECTOR rot;
        pObject->GetRotation(&rot.X, &rot.Y, &rot.Z);

        RakNet::BitStream bsSend;

        bsSend.Write((bool)     CObjectEditor::bPlayerObj);
        bsSend.Write((uint16_t) CObjectEditor::iEditedId);
        bsSend.Write((uint32_t) 0);
        bsSend.Write((float)    pos.X);
        bsSend.Write((float)    pos.Y);
        bsSend.Write((float)    pos.Z);
        bsSend.Write((float)    rot.X);
        bsSend.Write((float)    rot.Y);
        bsSend.Write((float)    rot.Z);

        pNetGame->GetRakClient()->RPC(&RPC_EditObject, &bsSend, HIGH_PRIORITY, RELIABLE, 0, false, UNASSIGNED_NETWORK_ID, nullptr);
    }

    CObjectEditor::bIsToggle = false;
    pJavaWrapper->HideEditObject();
}

void CObjectEditor::SendOnEditAttach(int response, int index, int modelid, int bone, VECTOR offset, VECTOR rot, VECTOR scale){

    RakNet::BitStream bsSend;

    bsSend.Write((uint32_t)response);
    bsSend.Write((uint32_t)index);
    bsSend.Write((uint32_t)modelid);
    bsSend.Write((uint32_t)bone);
    bsSend.Write(offset);
    bsSend.Write(rot);
    bsSend.Write(scale);
    bsSend.Write((uint32_t)0);
    bsSend.Write((uint32_t)0);

    pNetGame->GetRakClient()->RPC(&RPC_EditAttachedObject, &bsSend, HIGH_PRIORITY, RELIABLE, 0, false, UNASSIGNED_NETWORK_ID, nullptr);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_sampmobile_game_main_ui_AttachEdit_AttachClick(JNIEnv *env, jobject thiz, jint button_type,
                                                  jboolean button_id) {

    auto pPlayer = pNetGame->GetPlayerPool()->GetLocalPlayer()->GetPlayerPed();
    int slot = 0;
    if(CObjectEditor::editType == CObjectEditor::TYPE_PLAYER_ATTACH) {
        slot = CObjectEditor::iEditedId;

        if (!pNetGame->GetPlayerPool()->GetLocalPlayer()->GetPlayerPed()->GetObjectSlotState(
                slot)) {
            CObjectEditor::time = 0;
            pJavaWrapper->HideEditObject();
            return;
        }
    }

    CObject *pObject = nullptr;
    if(CObjectEditor::editType == CObjectEditor::TYPE_OBJECT) {
        pObject = pNetGame->GetObjectPool()->GetAt(CObjectEditor::iEditedId);
        if (!pObject) {
            CObjectEditor::time = 0;
            pJavaWrapper->HideEditObject();
            return;
        }
    }

    if(button_type == 0) { // право/лево
        float value = (button_id)?(0.006f):(-0.006f);

        if(CObjectEditor::editType == CObjectEditor::TYPE_PLAYER_ATTACH) {
            pPlayer->m_attachedObjectInfo[slot].vecOffset.Z += value;
        }
        if(CObjectEditor::editType == CObjectEditor::TYPE_OBJECT) {
            MATRIX4X4 matrix;
            pObject->GetMatrix(&matrix);
            auto pos = matrix.pos;
            pos.X += value;

            pObject->TeleportTo(pos.X, pos.Y, pos.Z);
            pObject->UpdateRwMatrixAndFrame();
        }
    }

    if(button_type == 1) { // вверх/низ
        float value = (button_id)?(0.006f):(-0.006f);

        if(CObjectEditor::editType == CObjectEditor::TYPE_PLAYER_ATTACH) {
            pPlayer->m_attachedObjectInfo[slot].vecOffset.X += value;
        }
        if(CObjectEditor::editType == CObjectEditor::TYPE_OBJECT) {
            MATRIX4X4 matrix;
            pObject->GetMatrix(&matrix);
            auto pos = matrix.pos;
            pos.Z += value;

            pObject->TeleportTo(pos.X, pos.Y, pos.Z);
            pObject->UpdateRwMatrixAndFrame();
        }
    }
    if(button_type == 2) { // Y
        float value = (button_id)?(0.006f):(-0.006f);

        if(CObjectEditor::editType == CObjectEditor::TYPE_PLAYER_ATTACH) {
            pPlayer->m_attachedObjectInfo[slot].vecOffset.Y += value;
        }
        if(CObjectEditor::editType == CObjectEditor::TYPE_OBJECT) {
            MATRIX4X4 matrix;
            pObject->GetMatrix(&matrix);
            auto pos = matrix.pos;
            pos.Y += value;

            pObject->TeleportTo(pos.X, pos.Y, pos.Z);
            pObject->UpdateRwMatrixAndFrame();
        }
    }
    if(button_type == 3) { // scale
        float value = (button_id)?(0.006f):(-0.006f);

        if(CObjectEditor::editType == CObjectEditor::TYPE_PLAYER_ATTACH) {
            pPlayer->m_attachedObjectInfo[slot].vecScale.X += value;
            pPlayer->m_attachedObjectInfo[slot].vecScale.Y += value;
            pPlayer->m_attachedObjectInfo[slot].vecScale.Z += value;
        }
    }
    if(button_type == 4) { // rot x
        float value = (button_id)?(0.1f):(-0.1f);

        if(CObjectEditor::editType == CObjectEditor::TYPE_PLAYER_ATTACH) {
            pPlayer->m_attachedObjectInfo[slot].vecRot.X += value;
        }
        if(CObjectEditor::editType == CObjectEditor::TYPE_OBJECT) {
            float x, y, z;
            pObject->GetRotation(&x, &y, &z);
            x += value;

            if(pObject)
                pObject->InstantRotate(x, y, z);
        }
    }
    if(button_type == 5) { // rot y
        float value = (button_id)?(0.1f):(-0.1f);

        if(CObjectEditor::editType == CObjectEditor::TYPE_PLAYER_ATTACH) {
            pPlayer->m_attachedObjectInfo[slot].vecRot.Y += value;
        }
        if(CObjectEditor::editType == CObjectEditor::TYPE_OBJECT) {
            float x, y, z;
            pObject->GetRotation(&x, &y, &z);
            y += value;

            if(pObject)
                pObject->InstantRotate(x, y, z);
        }
    }
    if(button_type == 6) { // rot z
        float value = (button_id)?(1.0f):(-1.0f);

        if(CObjectEditor::editType == CObjectEditor::TYPE_PLAYER_ATTACH) {
            pPlayer->m_attachedObjectInfo[slot].vecRot.Z += value;
        }
        if(CObjectEditor::editType == CObjectEditor::TYPE_OBJECT) {

            float x, y, z;
            pObject->GetRotation(&x, &y, &z);
            z += value;

            if(pObject)
                pObject->InstantRotate(x, y, z);
        }
    }

    if(GetTickCount() - CObjectEditor::time > 250) {
        CObjectEditor::time = GetTickCount();

        if (CObjectEditor::editType == CObjectEditor::TYPE_PLAYER_ATTACH) {
            CObjectEditor::SendOnEditAttach(
                    2,
                    slot,
                    pPlayer->m_attachedObjectInfo[slot].iModel,
                    pPlayer->m_attachedObjectInfo[slot].iBoneID,
                    pPlayer->m_attachedObjectInfo[slot].vecOffset,
                    pPlayer->m_attachedObjectInfo[slot].vecRot,
                    pPlayer->m_attachedObjectInfo[slot].vecScale
            );
        }
        if (CObjectEditor::editType == CObjectEditor::TYPE_OBJECT) {
            auto pObject = pNetGame->GetObjectPool()->GetAt(CObjectEditor::iEditedId);

            MATRIX4X4 matrix4X4;
            pObject->GetMatrix(&matrix4X4);
            auto pos = matrix4X4.pos;
            VECTOR rot;
            pObject->GetRotation(&rot.X, &rot.Y, &rot.Z);

            RakNet::BitStream bsSend;

            bsSend.Write((bool) CObjectEditor::bPlayerObj);
            bsSend.Write((uint16_t) CObjectEditor::iEditedId);
            bsSend.Write((uint32_t) 2);
            bsSend.Write((float) pos.X);
            bsSend.Write((float) pos.Y);
            bsSend.Write((float) pos.Z);
            bsSend.Write((float) rot.X);
            bsSend.Write((float) rot.Y);
            bsSend.Write((float) rot.Z);

            pNetGame->GetRakClient()->RPC(&RPC_EditObject, &bsSend, HIGH_PRIORITY, RELIABLE, 0,
                                          false, UNASSIGNED_NETWORK_ID, nullptr);
        }
    }
}
extern "C"
JNIEXPORT void JNICALL
Java_com_sampmobile_game_main_ui_AttachEdit_Save(JNIEnv *env, jobject thiz) {
    CPlayerPed* pPlayer = pNetGame->GetPlayerPool()->GetLocalPlayer()->GetPlayerPed();
    int slot = CObjectEditor::iEditedId;

    if(CObjectEditor::editType == CObjectEditor::TYPE_PLAYER_ATTACH) {
        CObjectEditor::SendOnEditAttach(
                1,
                slot,
                pPlayer->m_attachedObjectInfo[slot].iModel,
                pPlayer->m_attachedObjectInfo[slot].iBoneID,
                pPlayer->m_attachedObjectInfo[slot].vecOffset,
                pPlayer->m_attachedObjectInfo[slot].vecRot,
                pPlayer->m_attachedObjectInfo[slot].vecScale
        );
    }
    if(CObjectEditor::editType == CObjectEditor::TYPE_OBJECT)
    {
        auto pObject = pNetGame->GetObjectPool()->GetAt(CObjectEditor::iEditedId);
        if(!pObject)
            goto exit;

        MATRIX4X4 matrix4X4;
        pObject->GetMatrix(&matrix4X4);
        auto pos = matrix4X4.pos;
        VECTOR rot;
        pObject->GetRotation(&rot.X, &rot.Y, &rot.Z);

        RakNet::BitStream bsSend;

        bsSend.Write((bool)     CObjectEditor::bPlayerObj);
        bsSend.Write((uint16_t) CObjectEditor::iEditedId);
        bsSend.Write((uint32_t) CObjectEditor::eResponse::EDIT_RESPONSE_FINAL);
        bsSend.Write((float)    pos.X);
        bsSend.Write((float)    pos.Y);
        bsSend.Write((float)    pos.Z);
        bsSend.Write((float)    rot.X);
        bsSend.Write((float)    rot.Y);
        bsSend.Write((float)    rot.Z);

        pNetGame->GetRakClient()->RPC(&RPC_EditObject, &bsSend, HIGH_PRIORITY, RELIABLE, 0, false, UNASSIGNED_NETWORK_ID, nullptr);
    }

    exit:
    CObjectEditor::bIsToggle = false;
    pJavaWrapper->HideEditObject();
    CObjectEditor::time = 0;
}