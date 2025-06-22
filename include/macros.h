#ifndef MACROS_H
#define MACROS_H
// Here we define variable length arguments for this macro
// available as a C pre-processor in recent versions
// This macro has the purpose of faciliting the drawing of 
// menus as it adds a NULL terminator to the string panels
#define NULL ((void*)0)
#define DRAW(...) ((char*[]){ __VA_ARGS__, NULL })
#endif