#pragma once

#include <list>
#include "../gui.h"

class Chat : public ListBox
{
public:
    Chat();

    void addChatMessage(const std::string& message, const std::string& nick, const ImColor& nick_color);
    void addInfoMessage(const std::string& format, ...);
    void addDebugMessage(const std::string& format, ...);
    void addClientMessage(const std::string& message, const ImColor& color);

    virtual void draw(ImGuiRenderer* renderer) override;

    virtual void activateEvent(bool active) override;
    virtual void touchPopEvent() override;
    virtual void keyboardEvent(const std::string& input) override;

    [[maybe_unused]] static bool commandClient(const std::string& command);

    static std::string sDevTagNick;

    void toggleVisibility() { m_showingChat = !m_showingChat; }
    bool isVisible() const { return m_showingChat; }

private:
    void addMessage(const std::string& messsage, const ImColor& color = ImColor(1.0f, 1.0f, 1.0f));
    void addPlayerMessage(const std::string& message, const std::string& nick, const ImColor& nick_color);

    bool m_showingChat = true;

    class MessageItem : public ListBoxItem
    {
    public:
        MessageItem(const std::string& message, const ImColor& color)
        {
            m_text = new Label(message, color, true, UISettings::fontSize() / 2);
            this->addChild(m_text);
        }

        void performLayout() override
        {
            m_text->performLayout();
            m_text->setPosition(ImVec2(0.0f, (height() - m_text->height()) / 2));
        }

    private:
        Label* m_text;
    };

    class PlayerMessageItem : public ListBoxItem
    {
    public:
        PlayerMessageItem(const std::string& message, const std::string& nick, const ImColor& nick_color)
        {
            m_nick = new Label(nick, nick_color, true, UISettings::fontSize() / 2);
            m_text = new Label(": " + message, ImColor(1.0f, 1.0f, 1.0f), true, UISettings::fontSize() / 2);
            this->addChild(m_nick);
            this->addChild(m_text);
        }

        void performLayout() override
        {
            m_nick->performLayout();
            m_nick->setPosition(ImVec2(0.0f, (height() - m_nick->height()) / 2));

            m_text->performLayout();
            m_text->setPosition(ImVec2(m_nick->position().x + m_nick->width(), (height() - m_text->height()) / 2));
        }

    private:
        Label* m_nick;
        Label* m_text;
    };
};
