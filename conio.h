/*
 * This header provides a common interface for the function of _kbhit and _getch (in <conio.h> on windows) for both Linux and windows
 */

int keyboard_hit();
int get_char();
void keyboard_recovery();
