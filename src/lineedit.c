#include "lineedit.h"
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>

#define ESC_VAL '\033'
#define ESC_UP 'A'
#define ESC_DOWN 'B'
#define ESC_RIGHT 'C'
#define ESC_LEFT 'D'
#define CTRL_B 0x2
#define CTRL_F 0x6
#define DEFAULT_BUF_SIZE 32
#define KEY_ESC 27
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


void _step_backward(size_t *bufindex)
{
  if (*bufindex > 0) {
    *bufindex -= 1;
  }
}

void _step_forward(size_t *bufindex, size_t *buflen)
{
  if (*bufindex < *buflen) {
    *bufindex += 1;
  }
}

void _handle_esc(char **buffer, size_t *bufindex, size_t *buflen)
{
  getc(stdin); // Skip '['
  char c = getc(stdin);
  switch(c) {
  case ESC_UP:
    break;
  case ESC_DOWN:
    break;
  case ESC_LEFT:
    _step_backward(bufindex);
    break;
  case ESC_RIGHT:
    _step_forward(bufindex, buflen);
    break;
  }
}


char* ch_readline(const char *prompt, size_t promptlen)
{
  _ch_readline_setup();
  size_t i;
  char* buffer = malloc(sizeof(char) * DEFAULT_BUF_SIZE);
  buffer[DEFAULT_BUF_SIZE - 1] = '\0';
  size_t bufindex = 0;
  size_t buflen = 0;
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
    case ESC_VAL:
      _handle_esc(&buffer, &bufindex, &buflen);
      break;
    case CTRL_B:
      _step_backward(&bufindex);
      break;
    case CTRL_F:
      _step_forward(&bufindex, &buflen);
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
      buflen -= 1;
      break;
    default:
      for (i=buflen+1; i>bufindex; --i) {
	buffer[i] = buffer[i-1];
      }
      buffer[bufindex] = c;
      bufindex += 1;
      buflen += 1;

      if (buflen >= bufsize) {
	bufsize *= 2;
	buffer = realloc(buffer, bufsize);
      }
    }

    buffer[buflen] = '\0';
    clearline();
    printf("%s%s", prompt, buffer);
    cursorpos(bufindex+promptlen);
    fflush(stdout);

  } while (c != '\n');
  putc('\n', stdout);
  _ch_readline_teardown();

  return buffer;
}
