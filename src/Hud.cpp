#include "Hud.h"

#include <ncurses.h>
#include <string>

HUD::HUD()
{

}

HUD::~HUD()
{

}

void HUD::render()
{
    int width, height;
    getmaxyx(stdscr, height, width);
    
    std::string hor = "+" + std::string(width - 2, '-') + "+";
    mvprintw(0, 0, hor.c_str());
    mvprintw(height - 1, 0, hor.c_str());

    for(int i = 0; i < height - 2; i++)
    {
        mvprintw(i + 1, 0, "|");
        mvprintw(i + 1, width - 1, "|");
    }
}