#include "main.h"
#include "scoreboard.h"
#include "game/game.h"
#include "net/netgame.h"
#include "gui/gui.h"
#include "game/pad.h"
#include "vendor/imgui/imgui_internal.h"
//#include "CSettings.h"
#include "java/jniutil.h"
#include <jni.h>
#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

extern CNetGame* pNetGame;
extern CGame* pGame;
extern UI *pUI;
extern CSettings* pSettings;
extern CJavaWrapper *pJavaWrapper;

ImGuiWindowFlags fScoreBoardFlags =
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoCollapse;

CScoreBoard::CScoreBoard()
{
    m_iOffset = 0;
    m_bSorted = false;
    m_bToggle = false;
    m_iLastEndPlayer = 0;
    m_fScrollY = 0.0f;

    m_fScoreBoardSizeX = 846.0f;
    m_fScoreBoardSizeY = 614.0f;

    m_pPlayers = 0;
    m_pPlayerCount = 0;
}

CScoreBoard::~CScoreBoard() {}

void SwapPlayerInfo(PLAYER_SCORE_INFO* psi1, PLAYER_SCORE_INFO* psi2)
{
    PLAYER_SCORE_INFO plrinf;
    memcpy(&plrinf, psi1, sizeof(PLAYER_SCORE_INFO));
    memcpy(psi1, psi2, sizeof(PLAYER_SCORE_INFO));
    memcpy(psi2, &plrinf, sizeof(PLAYER_SCORE_INFO));
}



void CScoreBoard::Draw()
{
    ProcessUpdating();

    if (!m_bToggle) return;
    if (!m_pPlayers) return;

    PLAYERID endplayer = m_pPlayerCount;
    char szPlayerId[11], szScore[11], szPing[11], szServerAddress[512];
    unsigned char RGBcolors[3];

    std::string serverUTF8 = pNetGame->m_szHostName;

    sprintf(szServerAddress, "%s | Players: %d", serverUTF8.c_str(), m_pPlayerCount);

    ImGuiIO& io = ImGui::GetIO();
    ImVec2 size = ImGui::GetWindowSize();

    ImGui::SetNextWindowSize(ImVec2(m_fScoreBoardSizeX, m_fScoreBoardSizeY), NULL);
    ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));

    m_fWindowSizeX = ImGui::GetWindowSize().x;
    m_fWindowSizeY = ImGui::GetWindowSize().y;

    m_fDisplaySizeX = io.DisplaySize.x;
    m_fDisplaySizeY = io.DisplaySize.y;

    ImGui::Begin(szServerAddress, NULL, fScoreBoardFlags);

    ImGui::Columns(4, "Scoreboard");

    ImGui::Text("ID"); ImGui::SetColumnWidth(-1, m_fScoreBoardSizeX / 100 * 10); ImGui::NextColumn();
    ImGui::Text("Name"); ImGui::SetColumnWidth(-1, m_fScoreBoardSizeX / 100 * 60); ImGui::NextColumn();
    ImGui::Text("Score"); ImGui::SetColumnWidth(-1, m_fScoreBoardSizeX / 100 * 15); ImGui::NextColumn();
    ImGui::Text("Ping"); ImGui::SetColumnWidth(-1, m_fScoreBoardSizeX / 100 * 15); ImGui::NextColumn();

    for (int x = m_iOffset; x < endplayer; x++)
    {
        if (!m_bToggle || !m_pPlayers) break; // crash fix

        sprintf(szPlayerId, "%d", m_pPlayers[x].dwID);
        sprintf(szScore, "%d", m_pPlayers[x].iScore);
        sprintf(szPing, "%d", m_pPlayers[x].dwPing);

        RGBcolors[0] = (m_pPlayers[x].dwColor - 0xFF000000) >> 16;
        RGBcolors[1] = ((m_pPlayers[x].dwColor - 0xFF000000) & 0x00ff00) >> 8;
        RGBcolors[2] = ((m_pPlayers[x].dwColor - 0xFF000000) & 0x0000ff);

        ImGui::TextColored(ImColor(RGBcolors[0], RGBcolors[1], RGBcolors[2]), "%s", szPlayerId);
        ImGui::NextColumn();

        ImGui::TextColored(ImColor(RGBcolors[0], RGBcolors[1], RGBcolors[2]), "%s", m_pPlayers[x].szName);
        ImGui::NextColumn();

        ImGui::TextColored(ImColor(RGBcolors[0], RGBcolors[1], RGBcolors[2]), "%s", szScore);
        ImGui::NextColumn();

        ImGui::TextColored(ImColor(RGBcolors[0], RGBcolors[1], RGBcolors[2]), "%s", szPing);
        ImGui::NextColumn();
    }
    ImGui::Columns(1);

    ImGui::End();
}

void CScoreBoard::Toggle()
{
    /*m_bToggle = !m_bToggle;
    if (m_bToggle)
    {
        // Freeze player
        pNetGame->GetPlayerPool()->GetLocalPlayer()->GetPlayerPed()->TogglePlayerControllableWithoutLock(false);

        // Get player list
        pNetGame->UpdatePlayerScoresAndPings();

        CPlayerPool* pPlayerPool = pNetGame->GetPlayerPool();
        PLAYERID playercount = pPlayerPool->GetCount() + 1;
        m_pPlayerCount = playercount;

        if (m_iOffset > (playercount - 20)) m_iOffset = (playercount - 20);
        if (m_iOffset < 0) m_iOffset = 0;

        m_pPlayers = (PLAYER_SCORE_INFO*)malloc(playercount * sizeof(PLAYER_SCORE_INFO));
        memset(m_pPlayers, 0, playercount * sizeof(PLAYER_SCORE_INFO));
        m_pPlayers[0].dwID = pPlayerPool->GetLocalPlayerID();
        m_pPlayers[0].szName = pPlayerPool->GetLocalPlayerName();
        m_pPlayers[0].iScore = pPlayerPool->GetLocalPlayerScore();
        m_pPlayers[0].dwPing = pPlayerPool->GetLocalPlayerPing();
        m_pPlayers[0].dwColor = pPlayerPool->GetLocalPlayer()->GetPlayerColorAsARGB();
        PLAYERID i = 1, x;
        for (x = 0; x < MAX_PLAYERS; x++)
        {
            if (!pPlayerPool->GetSlotState(x)) continue;
            m_pPlayers[i].dwID = x;
            m_pPlayers[i].szName = pPlayerPool->GetPlayerName(x);
            m_pPlayers[i].iScore = pPlayerPool->GetRemotePlayerScore(x);
            m_pPlayers[i].dwPing = pPlayerPool->GetRemotePlayerPing(x);
            m_pPlayers[i].dwColor = pPlayerPool->GetAt(x)->GetPlayerColorAsARGB();
            m_pPlayers[i].iState = (int)pPlayerPool->GetAt(x)->GetState();

            i++;
        }*/
    // Freeze player
    pNetGame->GetPlayerPool()->GetLocalPlayer()->GetPlayerPed()->TogglePlayerControllable(false);

    Update();

    // Show Window
    pJavaWrapper->ShowTabWindow();
    pUI->buttonpanel()->setVisible(false);
    pGame->DisplayHUD(false);
    pJavaWrapper->HideHud();
}

void CScoreBoard::Close() {

    // Unfreeze player
    pNetGame->GetPlayerPool()->GetLocalPlayer()->GetPlayerPed()->TogglePlayerControllable(true);
    pUI->buttonpanel()->setVisible(true);
    pGame->DisplayHUD(true);
    pJavaWrapper->ShowHud();

    if (m_pPlayers)
    {
        memset(m_pPlayers, 0, m_pPlayerCount * sizeof(PLAYER_SCORE_INFO));
        free(m_pPlayers);
    }
    m_pPlayers = 0;
}

void CScoreBoard::Update() {

    // Get player list
    pNetGame->UpdatePlayerScoresAndPings();

    CPlayerPool* pPlayerPool = pNetGame->GetPlayerPool();
    PLAYERID playercount = pPlayerPool->GetTotalPlayers() + 1;
    m_pPlayerCount = playercount;

    if (m_iOffset > (playercount - 20)) m_iOffset = (playercount - 20);
    if (m_iOffset < 0) m_iOffset = 0;

    m_pPlayers = (PLAYER_SCORE_INFO*)malloc(playercount * sizeof(PLAYER_SCORE_INFO));
    memset(m_pPlayers, 0, playercount * sizeof(PLAYER_SCORE_INFO));
    m_pPlayers[0].dwID = pPlayerPool->GetLocalPlayerID();
    m_pPlayers[0].szName = pPlayerPool->GetLocalPlayerName();
    m_pPlayers[0].iScore = pPlayerPool->GetLocalPlayerScore();
    pJavaWrapper->SetTabStat(m_pPlayers[0].dwID, m_pPlayers[0].szName, m_pPlayers[0].iScore, m_pPlayers[0].dwPing);

    PLAYERID i = 1, x;
    for (x = 0; x < MAX_PLAYERS; x++)
    {
        if (!pPlayerPool->GetSlotState(x)) continue;
        m_pPlayers[i].dwID = x;
        m_pPlayers[i].szName = pPlayerPool->GetPlayerName(x);
        m_pPlayers[i].iScore = pPlayerPool->GetLocalPlayerScore();
        m_pPlayers[i].dwPing = pPlayerPool->GetLocalPlayerPing();
        pJavaWrapper->SetTabStat(m_pPlayers[i].dwID, m_pPlayers[i].szName, m_pPlayers[i].iScore, m_pPlayers[i].dwPing);
        i++;
    }
}

/*bool CScoreBoard::OnTouchEvent(int type, bool multi, float x, float y)
{
    if (!m_bToggle) return true;

    static bool bWannaCloseTab = false;

    switch (type)
    {
        case TOUCH_PUSH:
            if (
                    x < (m_fDisplaySizeX * 0.5f - m_fWindowSizeX) || x >(m_fDisplaySizeX * 0.5f + m_fWindowSizeX) ||
                    y < (m_fDisplaySizeY * 0.5f - m_fWindowSizeY) || y >(m_fDisplaySizeY * 0.5f + m_fWindowSizeY)
                    )
            {
                bWannaCloseTab = true;
            }
            break;
        case TOUCH_POP:
            if (
                    bWannaCloseTab &&
                    x < (m_fDisplaySizeX * 0.5f - m_fWindowSizeX) || x >(m_fDisplaySizeX * 0.5f + m_fWindowSizeX) ||
                    y < (m_fDisplaySizeY * 0.5f - m_fWindowSizeY) || y >(m_fDisplaySizeY * 0.5f + m_fWindowSizeY)
                    )
            {
                Toggle();
                bWannaCloseTab = false;
            }
            break;
    }
    return true;
} */

void CScoreBoard::ProcessUpdating()
{
    if (pNetGame)
    {
        if ((GetTickCount() - m_tickProcessingUpdate) >= 2000)
        {
            m_tickProcessingUpdate = GetTickCount();
            pNetGame->UpdatePlayerScoresAndPings();
        }
    }
}