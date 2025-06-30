// This is a header file. It helps keep the code well organized
// We have to define this include guards so that if this header
// gets included more than once in our program, we do not get 
// duplication erros
#ifndef RENDER_H
#define RENDER_H

// Controls wheter text rendered is bold or not 
// depending on a flag
void html_strong(int open);
// Prints a line of text, alias to printf
void html_span(char line[]);
// Breaks a line in the console
void html_br();
// Renders a paragraph of text
void html_p(char line[]);
// Renders a null terminating array of strings to the console
void render(char* lines[]);
// Clears the screen using ANSI escape codes
void clear_screen();
// Returns the current height of the document displayed on screen
int get_current_height(); // Always remember to add semicolon
// Renders a single char to the screen. used in for loop iteration
void html_single_char(char letter);
int move_cursor(int row, int column) ;

#endif