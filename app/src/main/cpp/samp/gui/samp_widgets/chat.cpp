#include "chat.h"
#include "../gui.h"
#include "../../main.h"
#include "../../game/game.h"
#include "../../net/netgame.h"
#include "../settings.h"
#include "java/jniutil.h"
#include "../../playertags.h"

extern UI* pUI;
extern CGame* pGame;
extern CNetGame* pNetGame;
extern CSettings* pSettings;
extern CJavaWrapper* pJavaWrapper;

Chat* pChat = nullptr;

Chat::Chat()
        : ListBox(), m_showingChat(true)
{
    pChat = this;
}

void Chat::addMessage(const std::string& message, const ImColor& color)
{
    if (!m_showingChat) return;

    float maxWidth = UISettings::m_chatSize.x - 10.0f;
    float charWidth = 7.0f;

    std::string line;
    std::string word;
    for (size_t i = 0; i <= message.size(); i++)
    {
        char c = (i < message.size()) ? message[i] : ' ';
        if (c == ' ' || c == '\n')
        {
            if ((line.size() + word.size()) * charWidth > maxWidth)
            {
                MessageItem* item = new MessageItem(line, color);
                this->addItem(item);
                line = word + " ";
            }
            else
            {
                line += word + " ";
            }

            word.clear();

            if (c == '\n')
            {
                MessageItem* item = new MessageItem(line, color);
                this->addItem(item);
                line.clear();
            }
        }
        else
        {
            word += c;
        }
    }

    if (!line.empty())
    {
        MessageItem* item = new MessageItem(line, color);
        this->addItem(item);
    }

    while (this->itemsCount() > UISettings::chatMaxMessages())
        this->removeItem(0);

    this->setScrollY(1.0f);
}

void Chat::addChatMessage(const std::string& message, const std::string& nick, const ImColor& nick_color)
{
    if (!m_showingChat) return;

    float nickWidth = nick.size() * 7.0f;
    float maxWidth = UISettings::m_chatSize.x - 10.0f - nickWidth;
    float charWidth = 7.0f;

    std::string line;
    std::string word;
    for (size_t i = 0; i <= message.size(); i++)
    {
        char c = (i < message.size()) ? message[i] : ' ';
        if (c == ' ' || c == '\n')
        {
            if ((line.size() + word.size()) * charWidth > maxWidth)
            {
                PlayerMessageItem* item = new PlayerMessageItem(line, nick, nick_color);
                this->addItem(item);
                line = word + " ";
            }
            else
            {
                line += word + " ";
            }

            word.clear();

            if (c == '\n')
            {
                PlayerMessageItem* item = new PlayerMessageItem(line, nick, nick_color);
                this->addItem(item);
                line.clear();
            }
        }
        else
        {
            word += c;
        }
    }

    if (!line.empty())
    {
        PlayerMessageItem* item = new PlayerMessageItem(line, nick, nick_color);
        this->addItem(item);
    }

    while (this->itemsCount() > UISettings::chatMaxMessages())
        this->removeItem(0);

    this->setScrollY(1.0f);
}

void Chat::addInfoMessage(const std::string& format, ...)
{
    char tmp_buf[512];
    va_list args;
    va_start(args, format);
    vsprintf(tmp_buf, format.c_str(), args);
    va_end(args);

    addMessage(std::string(tmp_buf), ImColor(0x00, 0xc8, 0xc8));
}

void Chat::addDebugMessage(const std::string& format, ...)
{
    char tmp_buf[512];
    va_list args;
    va_start(args, format);
    vsprintf(tmp_buf, format.c_str(), args);
    va_end(args);

    addMessage(std::string(tmp_buf), ImColor(0xbe, 0xbe, 0xbe));
}

void Chat::addClientMessage(const std::string& message, const ImColor& color)
{
    addMessage(message, color);
}

void Chat::touchPopEvent()
{
    if (pUI && pUI->playertablist() && pUI->playertablist()->visible()) return;
    pUI->keyboard()->show(this);
}

void Chat::keyboardEvent(const std::string& input)
{
    if (input.empty() || !pNetGame) return;

    if (input[0] == '/')
    {
        if (!commandClient(input))
            pNetGame->SendChatCommand(input.c_str());
    }
    else
        pNetGame->SendChatMessage(input.c_str());
}

bool Chat::commandClient(const std::string& command)
{
    if (command == "/chat")
    {
        if (pChat) pChat->toggleVisibility();
        return true;
    }

    return false;
}

void Chat::draw(ImGuiRenderer* renderer)
{
    if (!m_showingChat) return;
    ListBox::draw(renderer);
}

void Chat::activateEvent(bool active)
{
    this->setScrollable(active);
}
