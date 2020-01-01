#include "Screen.h"

#include <algorithm> 
#include <ncurses.h>

#include "keys.h"

#define X_OFFSET 2
#define Y_OFFSET 1

Screen::Screen()
    : m_Cursor({0, 0}), m_Lines(std::vector<std::string*>())
{
    m_Lines.push_back(new std::string());
}

Screen::~Screen()
{
    endwin();
    for(auto str : m_Lines)
        delete str;
}

void Screen::init()
{
    initscr();
    noecho();
	raw(); /* stops CTRL + C, etc */
    keypad(stdscr, true); /* Recognise arrow keys */
}

void Screen::pollInputs()
{
    int c = getch();
	if (c == CTRL(c)) //cant be put into switch
	{
		doControlKey(c);
	}
	else 
	{
		switch(c) 
		{
            case KEY_LEFT:
            case KEY_RIGHT:
            case KEY_UP:
            case KEY_DOWN:
                doMoveCursor(c);
                break;
            case KEY_BACKSPACE:
		        doBackspace();
                break;
            case KEY_DC:
                //do delete key
                break;
            case KEY_RESIZE:
                break;
			default:
				insertChar(c);
		}
	}
}

void Screen::doBackspace() 
{
    if(m_Cursor.x == 0) 
    {
        if(m_Cursor.y != 0)
        {
            if(m_Lines[m_Cursor.y]->length() == 0) //if empty line
            {
                m_Lines.erase(m_Lines.begin() + m_Cursor.y);
                m_Cursor.x = m_Lines[m_Cursor.y - 1]->length();
            } 
            else 
            { //delete at x = 0 when there are characters remaining on the current line
                int remainingSpace = MAX_STRING_LEN - m_Lines[m_Cursor.y - 1]->length();
                int toMove = std::min(remainingSpace, (int) m_Lines[m_Cursor.y]->length());
                m_Lines[m_Cursor.y - 1]->append(m_Lines[m_Cursor.y]->substr(0, toMove));
                
                if(toMove == m_Lines[m_Cursor.y]->length()) //if remaining chars will all fit in the line above
                    m_Lines.erase(m_Lines.begin() + m_Cursor.y);
                else
                    m_Lines[m_Cursor.y]->erase(0, toMove);
                m_Cursor.x = m_Lines[m_Cursor.y - 1]->length() - toMove;
            }
            m_Cursor.y--;
        }
    } 
    else 
    {
        m_Lines[m_Cursor.y]->erase(--m_Cursor.x, 1);
    }
}

void Screen::doMoveCursor(int c)
{
	switch(c)
	{
		case KEY_UP:
			if(m_Cursor.y != 0) 
			{
				m_Cursor.y--;
				m_Cursor.x = std::min(m_Cursor.x, (int) m_Lines[m_Cursor.y]->length());
			}
			else m_Cursor.x = 0;
			break;
		case KEY_DOWN: 
			if(m_Cursor.y < m_Lines.size() - 1)
			{ 
				m_Cursor.y++;
				m_Cursor.x = std::min(m_Cursor.x, (int) m_Lines[m_Cursor.y]->length());
			} 
			else m_Cursor.x = m_Lines[m_Cursor.y]->length();
			break;
		case KEY_RIGHT: 
			if(m_Cursor.x < m_Lines[m_Cursor.y]->length())
				m_Cursor.x++;
			else if(m_Cursor.y != m_Lines.size() - 1) 
			{
				m_Cursor.y++;
				m_Cursor.x = 0;
			}
			break;
		case KEY_LEFT: 
			if(m_Cursor.x != 0) 
				m_Cursor.x--;
			else if(m_Cursor.y != 0) 
			{
				m_Cursor.y--;
				m_Cursor.x = m_Lines[m_Cursor.y]->length();
			}
			break;
	}
}

void Screen::doControlKey(int c)
{
    switch(c)
	{
		case TAB:
			insertChar(' ');
			break;
        case NEW_LINE:
        case CARRIAGE_RETURN: //for some reason these count as control keys
                //move characters after x to new line
                m_Lines.insert(m_Lines.begin() + m_Cursor.y + 1, new std::string(*m_Lines[m_Cursor.y], m_Cursor.x));
                m_Lines[m_Cursor.y]->erase(m_Cursor.x);
                m_Cursor.x = 0;
                m_Cursor.y++;
                break;
		case CTRL('c'):
			m_CtrlC = true;
			break;
	}
}

void Screen::insertChar(int c)
{
    if(m_Lines[m_Cursor.y]->length() < MAX_STRING_LEN)
	{
		m_Lines[m_Cursor.y]->insert(m_Cursor.x, 1, (char)c);
		m_Cursor.x++;
	}
}

void Screen::render()
{
    clear();

    for(int i = 0; i < m_Lines.size(); i++)
        mvprintw(Y_OFFSET + i, X_OFFSET, "%s\n", m_Lines[i]->c_str());
}

void Screen::drawCursor()
{
	move(Y_OFFSET + m_Cursor.y, X_OFFSET + m_Cursor.x);
}