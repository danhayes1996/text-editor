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

        inline bool hasCtrlC() { return m_CtrlC; }

    private:
        void doMoveCursor();
        void doControlKey(int c);
        void insertChar(int c);

    private:
        Cursor m_Cursor;
        std::vector<std::string> m_Lines;

        //flags
        bool m_CtrlC = false;
};