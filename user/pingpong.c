#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
  int p[2];
  char *byte = "p";
  char inbuf[1];
  int pid;

  // Creating a pipe.
  if (pipe(p) < 0) {
	exit(1);	
  }

  if (fork() == 0) {
	close(0);
	dup(p[0]);
	pid = getpid();
	printf("%d: received ping\n", pid);
	write(p[0], byte, 1);

	close(p[0]);
	close(p[1]);

  } else {
	// Now pid is not equal to 0.
	//close(1);
	dup(p[1]);
	// Now we can't access stdout! So printf is ahead of close(1).
	pid = getpid();
	wait(0);
	printf("%d: received pong\n", pid);
	
	read(p[1], inbuf, 1);
	close(p[0]);
	close(p[1]);
  }
  
  exit(0);
}
