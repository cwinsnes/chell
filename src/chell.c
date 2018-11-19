#include <stdlib.h>
#include "lineedit.h"
#include "exec.h"

int main(int argc, char **argv)
{
  while (1) {
    char *line = ch_readline("> ");
    clearline();
    struct command *command = parse_command(line);
    execute_command(command);
    free_command(command);
    free(line);
  }
  return 0;
}
