#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include "lib/terminal.h"
#include "keycodes.h"
#include <sys/select.h>

// This is a wrapper around the low level read function that respects
// the VTIME property set by termios as opposed to getchar(), which does not.
// Also, it returns a integer, as opposed to a 1 byte char allowing us to do the
// offset trickery described later on
int readch() 
{
    char ch;
    int result = read(STDIN_FILENO, &ch, 1);

    // User has typed something
    if (result == 1)
    {
        // The casting is necessary because
        // we need to apply offsets to its
        // decimal representation
        return (unsigned char) ch;
    }
    // Timeout while reading key
    else if (result == 0)
    {
        return -1;
    }
    // stdin was closed or other error
    else if (result == -1)
    {
        return -2;
    }
}

// https://stackoverflow.com/questions/3276546/how-to-implement-getch-function-of-c-in-linux
// reads from keypress, doesn't echo, blocks until timeout, modified by Enzo
int getch()
{
    // Creates two uninitialized termios structs representing the 
    // previous terminal configuration and the new one.
    struct termios oldattr, newattr;
    int ch;
    // Puts the state of the file descriptior referencer by STDIN_FILENO
    // into the memory address of the termios old attributes 
    tcgetattr( STDIN_FILENO, &oldattr ); 
    // We copy it over to the new one
    newattr = oldattr;
    // We make bitwise operations as termios makes uses of flags to define
    // terminal behavior. This bitwise operation essentially disables canonical mode and echoing the character
    // ICANON - Canonical mode, which buffers the line until enter
    // ECHO - Echoes back the input
    // | We apply bitwise OR to the two flags 
    // ~ we negate them
    // &= We apply a bitwise and assignment to the current c_lflag
    newattr.c_lflag &= ~( ICANON | ECHO );
    // Minimum buffer size before releasing characters
    // What this actually mean is that this disables the
    // necessity of a key press
    newattr.c_cc[VMIN] = 0;
    // This sets the timeout to .5 seconds
    newattr.c_cc[VTIME] = 5;
    // We set the attributes defined previously for STDIN_FILENO
    // We make sure the changes are applied immediatelly
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    // The normal behavior of the getchar() function is governed by the terminal
    // driver which controls when the character stream is released to the program
    // However, getchar(), as a wrapper of the read function, utilizes an internal buffer
    // which in turn overwrites the VTIME value passed to the terminal attributes.
    // That's why we have our own wrapper that uses the low level function read
    ch = readch();
    // Handling arrow keys would require the use of a library called ncurses
    // which would demand the use of an api in front of terminal commands
    // I do not want to do this because this is a small project, so we are going
    // to do some trickery here
    while (ch == ESCAPE)
    {
        ch = readch();

        if (ch == ESCAPE)
        {
            continue;
        }

        if (ch == LEFT_SQUARED_BRACKET)
        {
            ch = readch();
            ch += ESCAPE_SEQUENCE_OFFSET;
            break;
        }

        ch += ESCAPE_MODE_OFFSET;
    }
    // we reset the terminal attributes to the old ones
    // so that we exit raw mode and do not have problems
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
};