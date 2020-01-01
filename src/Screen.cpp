#include "Screen.h"

#include <algorithm> 
#include <ncurses.h>

#include "keys.h"

Screen::Screen()
    : m_Cursor({0, 0}), m_Lines(std::vector<std::string*>())
{
    m_Lines.push_back(new std::string());
}

Screen::~Screen()
{
    endwin();
}

void Screen::init()
{
    initscr();
    noecho();
	raw(); /* stops CTRL + C, etc */
}

void Screen::pollInputs()
{
    int c = getch();
    if(c == ESCAPE_CHAR) //only works outside of switch
	{
		doMoveCursor();
	} 
	else if (c == CTRL(c)) //cant be put into switch
	{
		doControlKey(c);
	}
	else 
	{
		switch(c) 
		{
            case BACKSPACE:
		    case DEL:
		        doBackspace();
                break;
			default:
				insertChar(c);
		}
	}
}

void Screen::render()
{
    clear();
    for(int i = 0; i < m_Lines.size(); i++)
        printw("%s\n", m_Lines[i]->c_str());
	move(m_Cursor.y, m_Cursor.x);
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

//arrow key form '0x1b' then '[' then ('A' || 'B' || 'C' || 'D')
void Screen::doMoveCursor()
{
    getch(); //skip '[' character
	switch(getch()) 
	{
		case 'A' : //up arrow 
			if(m_Cursor.y != 0) 
			{
				m_Cursor.y--;
				m_Cursor.x = std::min(m_Cursor.x, (int) m_Lines[m_Cursor.y]->length());
			}
			else m_Cursor.x = 0;
			break;
		case 'B' : //down arrow 
			if(m_Cursor.y < m_Lines.size() - 1)
			{ 
				m_Cursor.y++;
				m_Cursor.x = std::min(m_Cursor.x, (int) m_Lines[m_Cursor.y]->length());
			} 
			else m_Cursor.x = m_Lines[m_Cursor.y]->length();
			break;
		case 'C' : //right arrow 
			if(m_Cursor.x < m_Lines[m_Cursor.y]->length())
				m_Cursor.x++;
			else if(m_Cursor.y != m_Lines.size() - 1) 
			{
				m_Cursor.y++;
				m_Cursor.x = 0;
			}
			break;
		case 'D' : //left arrow 
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