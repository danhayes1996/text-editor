#include <ncurses.h>

#include <vector>
#include <iostream>

#define MAX_STRING_LEN 5
#define ESCAPE_CHAR '\033'
#define CTRL(c) ((c) & 037)

std::vector<char*> arr;
int x = 0, y = 0;
//char* demo;


void doMoveCursor();
void doControlKey(int c);
void doInput(int c);
void render();

int main()
{
    //demo = new char[5];

    arr.push_back(new char[MAX_STRING_LEN]);
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

	if(c == ESCAPE_CHAR){
		doMoveCursor();
	} else {
		switch(c) 
		{
		    case 8:
		    case 127:
		        if(x == 0) {
		            if(y != 0){
		                x = 5;
		                y--;
		            }
		        }else {
		            arr[y][--x] = '\0';
		        }
		        break;
		    case 10:
		    case 13:
		        y++;
				x = 0;
				arr.push_back(new char[MAX_STRING_LEN]);
		        break;
			/*case CTRL('r'):
				doControlKey(c);
				break;
			*/
		    default:
				if(x != MAX_STRING_LEN){
					arr[y][x] = c;
					x = x + 1 == MAX_STRING_LEN ? x : x + 1;
				}
		}
	}
}

//arrow key form '\033' then '[' then ('A' || 'B' || 'C' || 'D')
void doMoveCursor() {
	getch(); //skip [ character
	switch(getch()) {
		case 'A' : //up arrow 
			if(y != 0) y--;
			else x = 0;
			break;
		case 'B' : //down arrow 
			if(y != arr.size() - 1) y++; 
			else x = MAX_STRING_LEN - 1;
			break;
		case 'C' : //right arrow 
			if(x != MAX_STRING_LEN - 1) x++;
			else if(y != arr.size() - 1) {
				y++;
				x = 0;
			}
			break;
		case 'D' : //left arrow 
			if(x != 0) x--;
			else if(y != 0) {
				y--;
				x = MAX_STRING_LEN - 1;
			}
			break;
	}
}

void doControlKey(int c) {
	arr[y][x++] = '1';
}

void render()
{
    clear();
    for(int i = 0; i < arr.size(); i++) {
        printw("%s\n", arr[i]);

    }
	move(y, x);
        // printw("\n\nx: %i", x);
}
