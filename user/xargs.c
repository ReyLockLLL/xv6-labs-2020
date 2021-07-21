#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"
 

#define MAXBUF 512

int
main(int argc, char * argv[])
{
  // read lines from the standard input.
  char *new_argv[MAXARG];
  char buf[MAXBUF];

  if (argc < 2) {
	fprintf(2, "usage: xargs missing arguments\n");
	exit(1);
  }
  if (argc + 1 > MAXARG) {
	fprintf(2, "xargs: too many arguments\n");
	exit(1);
  }

  // Copy the argv buffer to line
  for (int i = 1; i < argc; i++) {
	new_argv[i - 1] = argv[i];
  }
  new_argv[argc] = 0;
  char c;
  int i, len;
  while (1) {
    i = 0;	
	while (1) {
	  len = read(0, &c, 1);
	  if (len < 1)
	    break;
	  buf[i] = c;
	  if (c == '\n')
	    break;
	  i++;
	}
	if (i == 0)
	  break;
	buf[i] = 0;
  	new_argv[argc - 1] = buf;
    if (fork() == 0) {
	  // Call exec here.
	  exec(new_argv[0], new_argv);
	  exit(0);
	} else {
	  wait(0);
	}
  }
  exit(0);
}

