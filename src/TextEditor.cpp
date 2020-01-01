#include "TextEditor.h"

TextEditor::TextEditor()
    : m_Screen(new Screen())
{
    m_Screen->init();
}

TextEditor::~TextEditor()
{
    delete m_Screen;
}

void TextEditor::run()
{
    m_Running = true;
    while(m_Running) {
        m_Screen->pollInputs();

        m_Screen->render();

        if(m_Screen->hasCtrlC()) m_Running = false;
    }
}