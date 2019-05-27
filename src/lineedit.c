#include "lineedit.h"
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>

#define DEFAULT_BUF_SIZE 32
#define KEY_BCKSPC 8
#define KEY_DELETE 127
#define KEY_NEWLINE '\n'

static struct termios old_termios;
static struct termios current_termios;

void _ch_readline_teardown(void)
{
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &old_termios);
}

void _ch_readline_setup(void)
{
  tcgetattr(STDIN_FILENO, &old_termios);
  tcgetattr(STDIN_FILENO, &current_termios);

  current_termios.c_lflag &= ~(ECHO | ICANON);
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &current_termios);

  // register the teardown in case of unexpected exit
  atexit(_ch_readline_teardown);
}


char* ch_readline(const char *prompt)
{
  _ch_readline_setup();
  size_t i;
  char* buffer = malloc(sizeof(char) * DEFAULT_BUF_SIZE);
  buffer[DEFAULT_BUF_SIZE - 1] = '\0';
  size_t bufindex = 0;
  size_t bufsize = DEFAULT_BUF_SIZE;
  char c;
  if (!prompt) {
    prompt = "";
  }

  clearline();
  printf("%s", prompt);
  do {
    c = getc(stdin);
    switch (c) {
    case KEY_NEWLINE:
      break;
    case KEY_DELETE: //Do same as backspace for now
    case KEY_BCKSPC:
      if (bufindex == 0) {
	break;
      }
      for (i=bufindex; i<(bufsize-1); i++) {
	buffer[i-1] = buffer[i];
      }
      bufindex -= 1;
      break;
    default:
      buffer[bufindex] = c;
      bufindex += 1;
    }

    buffer[bufindex] = '\0';
    clearline();
    printf("%s%s", prompt, buffer);
    cursorpos(bufindex+strlen(prompt));
    fflush(stdout);

  } while (c != '\n');
  putc('\n', stdout);
  _ch_readline_teardown();

  return buffer;
}
