#pragma once

#define BACKSPACE           0x8
#define DEL                 0x7f

#define NEW_LINE            0xa
#define CARRIAGE_RETURN     0xd

#define ESCAPE_CHAR         0x1b
#define CONTROL_CHAR        0x1f

#define TAB                 0x9

#define CTRL(c) ((c) & CONTROL_CHAR)