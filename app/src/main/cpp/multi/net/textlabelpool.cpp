#include "../main.h"
#include "../game/game.h"
#include "netgame.h"

extern CGame* pGame;
extern CNetGame* pNetGame;

// 0.3.7
C3DTextLabelPool::C3DTextLabelPool()
{
	for (int i = 0; i < MAX_TEXT_LABELS; i++) {
		//m_TextLabels[i].pszText = nullptr;
		m_TextLabels[i] = nullptr;

		m_bSlotUsed[i] = false;
	}
}
// 0.3.7
C3DTextLabelPool::~C3DTextLabelPool()
{
	for (int i = 0; i < MAX_TEXT_LABELS; i++) {
		if (m_bSlotUsed[i]) {
			this->ClearLabel(i);
		}
	}
}
// 0.3.7
void C3DTextLabelPool::NewLabel(uint16_t wLabelId, TEXT_LABEL* pLabel) {

	if (wLabelId < MAX_TEXT_LABELS) {

		if (m_TextLabels[wLabelId])
		{
			delete m_TextLabels[wLabelId];
			m_TextLabels[wLabelId] = nullptr;
			m_bSlotUsed[wLabelId] = false;
		}

		//labelInfo.dwColor = (labelInfo.dwColor >> 8) | (labelInfo.dwColor << 24);
		//pTextLabel->pszText = new char[strlen(pLabel->pszText) + 1];
		//strcpy(pTextLabel->pszText, pLabel->pszText);
		TEXT_LABEL* pTextLabel = new TEXT_LABEL;
		pTextLabel->text = Encoding::cp2utf(pLabel->text);

		pTextLabel->dwColor = pLabel->dwColor;
		pTextLabel->vecPos.X = pLabel->vecPos.X;
		pTextLabel->vecPos.Y = pLabel->vecPos.Y;
		pTextLabel->vecPos.Z = pLabel->vecPos.Z;
		pTextLabel->fDistance = pLabel->fDistance;
		pTextLabel->bTestLOS = pLabel->bTestLOS;
		pTextLabel->playerId = pLabel->playerId;
		pTextLabel->vehicleId = pLabel->vehicleId;

		m_TextLabels[wLabelId] = pTextLabel;
		m_bSlotUsed[wLabelId] = true;
	}
}
// 0.3.7
void C3DTextLabelPool::ClearLabel(uint16_t wLabelId) {
	if (wLabelId < 0 || wLabelId >= MAX_TEXT_LABELS)
	{
		return;
	}
	m_bSlotUsed[wLabelId] = false;
	if (m_TextLabels[wLabelId])
	{
		delete m_TextLabels[wLabelId];
		m_TextLabels[wLabelId] = nullptr;
	}
}

void C3DTextLabelPool::Render(ImGuiRenderer* renderer)
{
	CPlayerPed *pPlayerPed = pGame->FindPlayerPed();
	if(!pPlayerPed) return;

	for (int i = 0; i < MAX_TEXT_LABELS; i++)
	{
		if (m_bSlotUsed[i]) {
            CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();
            if (!pPlayerPool) break;

            TEXT_LABEL *pTextLabel = m_TextLabels[i];

            VECTOR vecTextPos = pTextLabel->vecPos;

            if (pTextLabel->playerId != INVALID_PLAYER_ID) {
                if (pTextLabel->playerId == pPlayerPool->GetLocalPlayerID()) continue;

                if (pPlayerPool && pPlayerPool->GetSlotState(pTextLabel->playerId)) {
                    CRemotePlayer *pPlayer = pPlayerPool->GetAt(pTextLabel->playerId);
                    if (pPlayer && pPlayer->GetDistanceFromLocalPlayer() < pTextLabel->fDistance) {
                        CPlayerPed *pPlayerPed = pPlayer->GetPlayerPed();
                        if (pPlayerPed && pPlayerPed->IsAdded()) {
                            VECTOR matBone;
                            pPlayerPed->GetBonePosition(8, &matBone);

                            vecTextPos.X = matBone.X + pTextLabel->vecPos.X;
                            vecTextPos.Y = matBone.Y + pTextLabel->vecPos.Y;
                            vecTextPos.Z = matBone.Z + 0.23 + pTextLabel->vecPos.Z;

                            this->Draw(renderer, pTextLabel, vecTextPos, pTextLabel->text,
                                       pTextLabel->dwColor);

                        }
                    }
                }
            }
			if (pTextLabel->vehicleId != INVALID_VEHICLE_ID) {
				CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();
				if (pVehiclePool && pVehiclePool->GetSlotState(pTextLabel->vehicleId)) {
					CVehicle *pVehicle = pVehiclePool->GetAt(pTextLabel->vehicleId);
					if (pVehicle && pVehicle->IsAdded() &&
						pVehicle->GetDistanceFromLocalPlayerPed() < pTextLabel->fDistance) {
						MATRIX4X4 matVehicle;
						pVehicle->GetMatrix(&matVehicle);

						vecTextPos.X = matVehicle.pos.X + pTextLabel->vecPos.X;
						vecTextPos.Y = matVehicle.pos.Y + pTextLabel->vecPos.Y;
						vecTextPos.Z = matVehicle.pos.Z + pTextLabel->vecPos.Z;

						this->Draw(renderer, pTextLabel, vecTextPos, pTextLabel->text,
								   pTextLabel->dwColor);
					}
				}
			}

			if (pPlayerPed->GetDistanceFromPoint(pTextLabel->vecPos) <= pTextLabel->fDistance)
				this->Draw(renderer, pTextLabel, vecTextPos, pTextLabel->text,
						   pTextLabel->dwColor);
        }
	}
}

void C3DTextLabelPool::Draw(ImGuiRenderer* renderer, TEXT_LABEL* label, VECTOR vecPos, const std::string& text, uint32_t dwColor)
{
	VECTOR vPos;
	vPos.X = vecPos.X;
	vPos.Y = vecPos.Y;
	vPos.Z = vecPos.Z;

	int hitEntity = 0;
    if (label->bTestLOS) {
		CAMERA_AIM *pCam = GameGetInternalAim();
		if (!pCam)
		{
			return;
		}

        MATRIX4X4 matPlayer;
		pNetGame->GetPlayerPool()->GetLocalPlayer()->GetPlayerPed()->GetMatrix(&matPlayer);

		VECTOR vec;
		vec.X = pCam->pos1x;
		vec.Y = pCam->pos1y;
		vec.Z = pCam->pos1z;

		//bool isLineOfSightClear = ((bool (*)(VECTOR*, VECTOR*, int, int, int, int, int, int, int))(g_libGTASA + 0x423418 + 1))(&vec, &matPlayer.pos, 1, 0, 0, 1, 0, 0, 0);

		hitEntity = ScriptCommand(&get_line_of_sight,
								  vecPos.X, vecPos.Y, vecPos.Z,
								  pCam->pos1x, pCam->pos1y, pCam->pos1z,
								  1, 0, 0, 1, 0);
		/*if(!isLineOfSightClear)
		{
			LOGI("labelpool draw ok no render fuck you bitch");
			return;
		}*/
    }

	if (!label->bTestLOS || hitEntity) {
		if (pNetGame->GetPlayerPool()->GetLocalPlayer()->GetPlayerPed()->GetDistanceFromPoint(vecPos) <= label->fDistance) {
			VECTOR vecOut;
			// CSprite::CalcScreenCoors
			((void (*)(VECTOR *, VECTOR *, float *, float *, bool, bool)) (g_libGTASA + 0x5C5798 +
																		   1))(
					&vPos, &vecOut, 0, 0, 0, 0);
			if (vecOut.Z < 1.0f) return;

			std::stringstream ss_data(text);
			std::string s_row;
			while (std::getline(ss_data, s_row, '\n')) {
				ImVec2 sz = renderer->calculateTextSize(s_row, UISettings::fontSize() / 2);
				renderer->drawText(ImVec2(vecOut.X - (sz.x / 2), vecOut.Y),
								   __builtin_bswap32(dwColor | (0x000000FF)), s_row, true,
								   UISettings::fontSize() / 2);
				vecOut.Y += UISettings::fontSize() / 2;
			}
		}
	}
}