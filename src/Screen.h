#pragma once

#include <string>
#include <vector>

#define MAX_STRING_LEN 5

struct Cursor { int x; int y; };

class Screen 
{
    public:
        Screen();
        ~Screen();

        void init();

        void pollInputs();

        void render();
        void drawCursor();

        inline bool hasCtrlC() { return m_CtrlC; }

    private:
        void doBackspace();
        void doMoveCursor(int c);
        void doControlKey(int c);
        void insertChar(int c);
        void drawHUD();

    private:
        Cursor m_Cursor;
        std::vector<std::string*> m_Lines;

        //flags
        bool m_CtrlC = false;
};