#pragma once

#include "Screen.h"
#include "Hud.h"

class TextEditor 
{
    public:
        TextEditor();
        ~TextEditor();

        void run();

    private:
        bool m_Running;
        Screen* m_Screen;
        HUD* m_Hud;
};