#include <ncurses.h>

#include <algorithm> 
#include <string>
#include <vector>

#include "keys.h"

#define MAX_STRING_LEN 5

std::vector<std::string> arr;
int x = 0, y = 0;
bool running = true;

void doMoveCursor();
void doControlKey(int c);
void doInput(int c);
void render();
void insertChar(int c);

int main()
{
    arr.push_back(std::string());
    initscr();
    noecho();
	raw(); /* stops CTRL + C, etc */
 
    int c = -1;
    while(running)
    {
		c = getch();
		doInput(c);
        render();
    }

    endwin();	
    return 0;
}

void doInput(int c)
{

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
		        if(x == 0) 
				{
		            if(y != 0)
					{
						arr.erase(arr.begin() + y);
		                y--;
		                x = arr[y].length();
		            }
		        } 
				else 
				{
		            arr[y].erase(--x, 1);
		        }
		        break;
			default:
				insertChar(c);
		}
	}
}

//arrow key form '0x1b' then '[' then ('A' || 'B' || 'C' || 'D')
void doMoveCursor() 
{
	getch(); //skip '[' character
	switch(getch()) 
	{
		case 'A' : //up arrow 
			if(y != 0) 
			{
				y--;
				x = std::min(x, (int) arr[y].length());
			}
			else x = 0;
			break;
		case 'B' : //down arrow 
			if(y < arr.size() - 1)
			{ 
				y++;
				x = std::min(x, (int) arr[y].length());
			} 
			else x = arr[y].length();
			break;
		case 'C' : //right arrow 
			if(x < arr[y].length())
				x++;
			else if(y != arr.size() - 1) 
			{
				y++;
				x = 0;
			}
			break;
		case 'D' : //left arrow 
			if(x != 0) 
				x--;
			else if(y != 0) 
			{
				y--;
				x = arr[y].length();
			}
			break;
	}
}

void doControlKey( int c) 
{
	switch(c)
	{
		case TAB:
			insertChar(' ');
			break;
		case NEW_LINE:
		case CARRIAGE_RETURN: //for some reason these count as control keys
			y++;
			x = 0;
			arr.push_back(std::string());
			break;
		case CTRL('c'):
			running = false;
			break;
	}
}

void insertChar(int c) 
{
	if(arr[y].length() < MAX_STRING_LEN)
	{
		arr[y].insert(x, 1, (char)c);
		x++;// = x + 1 == MAX_STRING_LEN ? x : x + 1;
	}
}

void render()
{
    clear();
    for(int i = 0; i < arr.size(); i++)
        printw("%s\n", arr[i].c_str());
	move(y, x);
 }
