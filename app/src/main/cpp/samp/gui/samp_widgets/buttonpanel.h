#pragma once

class ButtonPanel : public Layout
{
public:
    ButtonPanel();

    void performLayout() override; // ⚡ اضافه کردن فانکشنی performLayout

    CButton* m_bH;
    OButton* m_bOpen;
    Button* m_bTab;
    Button* m_bSV;

private:
    Button* m_bAlt;
    Button* m_bY;
    Button* m_bN;
};
