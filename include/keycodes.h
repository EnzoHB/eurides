#ifndef KEYCODES_H
#define KEYCODES_H

// Special keys
#define ESCAPE 27
#define LEFT_SQUARED_BRACKET 91
#define ENTER 10
#define NONE -1

#define ESCAPE_MODE_OFFSET 1024
#define ESCAPE_SEQUENCE_OFFSET 2048

// Lowercase letters and digits
#define KEY_A 'a'
#define KEY_B 'b'
#define KEY_C 'c'
#define KEY_D 'd'
#define KEY_E 'e'
#define KEY_F 'f'
#define KEY_G 'g'
#define KEY_H 'h'
#define KEY_I 'i'
#define KEY_J 'j'
#define KEY_K 'k'
#define KEY_L 'l'
#define KEY_M 'm'
#define KEY_N 'n'
#define KEY_O 'o'
#define KEY_P 'p'
#define KEY_Q 'q'
#define KEY_R 'r'
#define KEY_S 's'
#define KEY_T 't'
#define KEY_U 'u'
#define KEY_V 'v'
#define KEY_W 'w'
#define KEY_X 'x'
#define KEY_Y 'y'
#define KEY_Z 'z'

#define KEY_0 '0'
#define KEY_1 '1'
#define KEY_2 '2'
#define KEY_3 '3'
#define KEY_4 '4'
#define KEY_5 '5'
#define KEY_6 '6'
#define KEY_7 '7'
#define KEY_8 '8'
#define KEY_9 '9'

#define ARROW_UP    65 + ESCAPE_SEQUENCE_OFFSET 
#define ARROW_DOWN  66 + ESCAPE_SEQUENCE_OFFSET 
#define ARROW_LEFT  68 + ESCAPE_SEQUENCE_OFFSET
#define ARROW_RIGHT 67 + ESCAPE_SEQUENCE_OFFSET

#define DELETE 127
#define BACK_BUTTON DELETE + ESCAPE_MODE_OFFSET

#endif