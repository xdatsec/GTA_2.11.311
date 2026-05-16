#include "main.h"
#include "gui/gui.h"
#include "button.h"

extern bool OpenButton;

//============== Default Button =================//
Button::Button(const std::string& caption, float font_size)
{
    m_callback = nullptr;

    m_label = new Label(caption, ImColor(1.0f, 1.0f, 1.0f), false, font_size);
    this->addChild(m_label);

    m_color = UISettings::buttonColor();
    m_colorFocused = UISettings::buttonFocusedColor();
}

void Button::performLayout()
{
    float padding = UISettings::padding();

    m_label->performLayout();
    this->setSize(m_label->size() + ImVec2(padding * 2, padding));

    m_label->setPosition((size() - m_label->size()) / 2);
}

void Button::draw(ImGuiRenderer* renderer)
{
    // بەتنە راوندید کراوە و outline لابراوە
    renderer->drawRect(
            absolutePosition(),
            absolutePosition() + size(),
            focused() ? m_colorFocused : m_color,
            true,
            20.0f // ✅ radius
    );

    Widget::draw(renderer);
}

void Button::touchPopEvent()
{
    if (m_callback) m_callback();
}


//============== Custom Button =================//
CButton::CButton(const std::string& caption, float font_size)
{
    m_callback = nullptr;

    m_label = new Label(caption, ImColor(1.0f, 1.0f, 1.0f), false, font_size);
    this->addChild(m_label);

    m_color = UISettings::buttonColor();
    m_colorFocused = UISettings::buttonFocusedColor();
}

void CButton::performLayout()
{
    float padding = UISettings::padding();

    m_label->performLayout();
    this->setSize(m_label->size() + ImVec2(padding * 2, padding));

    m_label->setPosition((size() - m_label->size()) / 2);
}

void CButton::draw(ImGuiRenderer* renderer)
{
    if (!OpenButton) return;

    // راوندید و بێ outline
    renderer->drawRect(
            absolutePosition(),
            absolutePosition() + size(),
            focused() ? m_colorFocused : m_color,
            true,
            20.0f
    );

    Widget::draw(renderer);
}

void CButton::touchPopEvent()
{
    if (!OpenButton) return;
    if (m_callback) m_callback();
}


//============== OButton =================//
OButton::OButton(const std::string& caption, float font_size)
{
    m_callback = nullptr;

    m_label = new Label(caption, ImColor(1.0f, 1.0f, 1.0f), false, font_size);
    this->addChild(m_label);

    m_color = UISettings::buttonColor();
    m_colorFocused = UISettings::buttonFocusedColor();
}

void OButton::performLayout()
{
    float padding = UISettings::padding();

    m_label->performLayout();
    this->setSize(m_label->size() + ImVec2(padding * 2, padding));

    m_label->setPosition((size() - m_label->size()) / 2);
}

void OButton::draw(ImGuiRenderer* renderer)
{
    if (OpenButton)
    {
        // گرتنی پۆزیشنی لادراو
        this->setPosition(ImVec2(-150.0f, -150.0f));
        return;
    }

    // راوندید و بێ outline
    renderer->drawRect(
            absolutePosition(),
            absolutePosition() + size(),
            focused() ? m_colorFocused : m_color,
            true,
            20.0f
    );

    Widget::draw(renderer);

    // دووبارە پۆزیشنی ئاسایی
    this->setPosition(ImVec2(15.0f, 15.0f));
}

void OButton::touchPopEvent()
{
    if (m_callback) m_callback();
}
