#include "TextEditor.h"

TextEditor::TextEditor()
    : m_Screen(new Screen()), m_Hud(new HUD())
{
    m_Screen->init();

    m_Hud->render();
    m_Screen->drawCursor();
}

TextEditor::~TextEditor()
{
    delete m_Screen;
    delete m_Hud;
}

void TextEditor::run()
{
    m_Running = true;
    while(m_Running) {
        m_Screen->pollInputs();

        m_Screen->render();

        m_Hud->render();

        m_Screen->drawCursor();

        if(m_Screen->hasCtrlC()) m_Running = false;
    }
}