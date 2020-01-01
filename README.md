compile with (from project root folder):
g++ src/main.cpp -o bin/textEditor -lncurses
g++ src/main.cpp src/TextEditor.cpp src/Screen.cpp -o bin/textEditor -lncurses
