#include <stdlib.h>
#include "lineedit.h"
#include "exec.h"
#include <unistd.h>
#include <libgen.h>
#include <regex.h>

#define MAXPATH 4096

int main(void)
{
  char *cwd = malloc(sizeof(char) * MAXPATH);
  char *prompt = malloc(sizeof(char) * MAXPATH * 2);
  char *user = getlogin();

  while (1) {
    if (!(getcwd(cwd, MAXPATH))) {
	perror("Error when reading current directory");
	exit(-1);
    }
    sprintf(prompt, "%s:%s:>", user, cwd);
    char *line = ch_readline(prompt);
    clearline();
    struct command *command = parse_command(line);
    execute_command(command);
    free_command(command);
    free(line);
  }
  return 0;
}
