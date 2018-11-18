#include <stdio.h>

#define clearline() printf("%c[2K\r", 27)
#define cursorpos(x) printf("\r\033[%luC", (x))

/**
 * Reads a line from the stdin interactively until a newline has been found.
 * Note that the returned string needs to be freed.
 *
 * @param prompt A prompt to keep at the beginning of the input line.
 * @result A string with the read line from stdin.
 */
char* ch_readline(const char *);
