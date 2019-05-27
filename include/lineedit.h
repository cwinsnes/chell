#ifndef CHELL_LINEEDIT_H
#define CHELL_LINEEDIT_H

#include <stdio.h>

/**
 * Resets the terminal formatting entirely.
 */
#define RSTT "\x1B[0m"

/**
 * Color choices.
 * The codes sets the foreground color to the indicated color.
 */
#define BLKT "\x1B[30m"
#define REDT "\x1B[31m"
#define GRNT "\x1B[32m"
#define YELT "\x1B[33m"
#define BLUT "\x1B[34m"
#define WHTT "\x1B[37m"

/**
 * Text formatting.
 * BFTT allows for bold text.
 */
#define BFTT "\x1B[1m"

#define clearline() printf("%c[2K\r", 27)
#define cursorpos(x) printf("\r\033[%luC", (x))

/**
 * Reads a line from the stdin interactively until a newline has been found.
 * Note that the returned string needs to be freed.
 *
 * @param prompt A prompt to keep at the beginning of the input line.
 * @param promptlen The length of the prompt as seen on the screen,
 *                  excluding any escape characters such as colors.
 * @result A string with the read line from stdin.
 */
char* ch_readline(const char *, size_t);
#endif
