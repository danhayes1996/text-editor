#include <ncurses.h>

#include <vector>
#include <string>
#include <algorithm> 

#define MAX_STRING_LEN 5
#define ESCAPE_CHAR '\033'
#define CTRL(c) ((c) & 037)

std::vector<std::string> arr;
int x = 0, y = 0;

void doMoveCursor();
void doControlKey(int c);
void doInput(int c);
void render();

int main()
{
    arr.push_back(std::string());
    initscr();
    noecho();
 
    int c = -1;
    while((c = getch()) != 'X')
    {
		doInput(c);
        render();
    }

    endwin();
	
    return 0;
}

void doInput(int c)
{

	if(c == ESCAPE_CHAR)
	{
		doMoveCursor();
	} 
	else 
	{
		switch(c) 
		{
		    case 8:
		    case 127:
		        if(x == 0) 
				{
		            if(y != 0)
					{
		                x = 5;
		                y--;
		            }
		        } 
				else 
				{
		            arr[y].erase(--x, 1);
		        }
		        break;
		    case 10:
		    case 13:
		        y++;
				x = 0;
				arr.push_back(std::string());
		        break;
			/*case CTRL('r'):
				doControlKey(c);
				break;
			*/
		    default:
				if(x < MAX_STRING_LEN)
				{
					arr[y].insert(x, 1, (char)c);
					x++;// = x + 1 == MAX_STRING_LEN ? x : x + 1;
				}
		}
	}
}

//arrow key form '\033' then '[' then ('A' || 'B' || 'C' || 'D')
void doMoveCursor() 
{
	getch(); //skip '[' character
	switch(getch()) {
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

void doControlKey(int c) 
{
}

void render()
{
    clear();
    for(int i = 0; i < arr.size(); i++)
        printw("%s\n", arr[i].c_str());
	move(y, x);
 }
