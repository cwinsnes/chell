#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <dirent.h>

#include "lineedit.h"

#define ESC_VAL '\033'
#define ESC_NUL 0x00
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
#define KEY_TAB 9
#define KEY_NEWLINE '\n'

static struct termios old_termios;
static struct termios current_termios;

const char* DEFAULT_TAB_COMPLETE_FOLDER = ".";

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


void _step_backward(__attribute__((unused)) char **buffer, size_t *bufindex, __attribute__((unused)) size_t *buflen)
{
  if (*bufindex > 0) {
    *bufindex -= 1;
  }
}

int _filter_folders(const struct dirent *folder) {
  if (strcmp(".", folder -> d_name) == 0) {
    return 0;
  } else if  (strcmp("..", folder -> d_name) == 0) {
    return 0;
  }
  return 1;
}

void _tab_complete(char **buffer, size_t *bufindex, size_t *buflen)
{
  /*
   * TODO: Divide this into smaller chunks
   * TODO: Also include tab completion towards commands
   */
  char *slash;
  const char *folder;
  char *str = *buffer;
  size_t word_beginning = *bufindex;
  char tmp_str[*buflen + 1];

  // Copy only current word into tmp_str
  while (word_beginning > 0 && !isspace(str[word_beginning-1])) {
    word_beginning -= 1;
  }
  strncpy(tmp_str, str+word_beginning, *bufindex-word_beginning);
  tmp_str[*bufindex-word_beginning] = '\0';

  if ((slash = strrchr(tmp_str, '/'))) {
    *slash = '\0';
    folder = tmp_str;
    slash += 1;
  } else {
    slash = tmp_str;
    folder = DEFAULT_TAB_COMPLETE_FOLDER;
  }

  struct dirent **matching_files;
  struct dirent *most_recent;
  int num_files = scandir(folder, &matching_files, _filter_folders, alphasort);
  if (num_files == -1) {
    return;
  }

  int n = num_files;
  for (int i=0; i<num_files; ++i) {
    if (strncmp(slash, matching_files[i]->d_name, strlen(slash)) != 0) {
      n -= 1;
      free(matching_files[i]);
      matching_files[i] = NULL;
    } else {
      most_recent = matching_files[i];
    }
  }
  if (n > 1) {
    printf("\n");
    for (int i=0; i<num_files; ++i) {
        if (matching_files[i] != NULL) {
          printf("%s\t", matching_files[i]->d_name);
          free(matching_files[i]);
        }
    }
    printf("\n");
  } else {
    strcpy(str+*bufindex-strlen(slash), most_recent->d_name);
    size_t save_bufindex = *bufindex;
    *bufindex = *bufindex + strlen(most_recent->d_name)-strlen(slash);
    *buflen += *bufindex - save_bufindex;
  }
  free(matching_files);
}

void _step_forward(__attribute__((unused)) char **buffer, size_t *bufindex, size_t *buflen)
{
  if (*bufindex < *buflen) {
    *bufindex += 1;
  }
}

void _step_word_forward(char **buffer, size_t *bufindex, size_t *buflen)
{
  char *str = *buffer;
  if (isspace(str[*bufindex])) {
    while(isspace(str[*bufindex]) && (*bufindex < *buflen)) {
      *bufindex += 1;
    }

  }
  while(!isspace(str[*bufindex]) && (*bufindex < *buflen)) {
    *bufindex += 1;
  }
}

void _step_word_backward(char **buffer, size_t *bufindex, __attribute__((unused)) size_t *buflen)
{
  char *str = *buffer;
  if (isspace(str[*bufindex])) {
    while(isspace(str[*bufindex]) && (*bufindex > 0)) {
      *bufindex -= 1;
    }

  }
  while(!isspace(str[*bufindex]) && (*bufindex > 0)) {
    *bufindex -= 1;
  }
}

void _handle_esc(char **buffer, size_t *bufindex, size_t *buflen)
{
  char c = getc(stdin);
  if (c == '[') {
    c = getc(stdin);
  }
  switch(c) {
  case ESC_UP:
    break;
  case ESC_DOWN:
    break;
  case ESC_LEFT:
    _step_backward(buffer, bufindex, buflen);
    break;
  case ESC_RIGHT:
    _step_forward(buffer, bufindex, buflen);
    break;
  case 'f':
    _step_word_forward(buffer, bufindex, buflen);
    break;
  case 'b':
    _step_word_backward(buffer, bufindex, buflen);
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
    case ESC_NUL:
    case ESC_VAL:
      _handle_esc(&buffer, &bufindex, &buflen);
      break;
    case CTRL_B:
      _step_backward(&buffer, &bufindex, &buflen);
      break;
    case CTRL_F:
      _step_forward(&buffer, &bufindex, &buflen);
      break;
    case KEY_TAB:
      _tab_complete(&buffer, &bufindex, &buflen);
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
