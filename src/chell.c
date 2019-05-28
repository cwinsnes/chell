#include <stdlib.h>
#include "lineedit.h"
#include "hashm.h"
#include "exec.h"
#include "builtins.h"
#include <unistd.h>
#include <libgen.h>
#include <string.h>
#include <regex.h>

#define MAXPATH 4096

int main(void)
{
  char *cwd = malloc(sizeof(char) * MAXPATH);
  char *prompt = malloc(sizeof(char) * MAXPATH * 2);
  char *promptcpy = malloc(sizeof(char) * MAXPATH * 2);
  char *user = getlogin();

  struct hashmap *builtins = initialize_builtins();

  while (1) {
    if (!(getcwd(cwd, MAXPATH))) {
	perror("Error when reading current directory");
	exit(-1);
    }
    sprintf(prompt, "%s:%s%s%s%s%s:>", user, BLUT, BFTT, cwd, RSTT, BLKT);
    sprintf(promptcpy, "%s:%s:>", user, cwd);

    char *line = ch_readline(prompt, strlen(promptcpy));
    clearline();
    struct command *command = parse_command(line);
    execute_command(command, builtins);
    free_command(command);
    free(line);
  }
  return 0;
}
