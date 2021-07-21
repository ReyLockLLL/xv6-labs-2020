#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define END 35

/// MAYBE HELPER METHOD?
void primeHelper(int p[2]);

int main() {
  // p = get a number from left neighbor
  // print p
  // loop:
  //   n = get a number from left neighbor
  //   if (p does not divide n)
  //      send n to right neighbor
  int i; 
  int p[2];

  if (pipe(p) < 0)
      exit(1);
  // We need to pass 2 - 35 to the other side of the pipe. 
  if (fork() == 0) {    
	primeHelper(p);
  } else {
    close(p[0]);
    for (i = 2; i <= END; i++) {
	    write(p[1], &i, sizeof(int)); 
    }
	close(p[1]);
	wait(0);
  }  
  exit(0);
}

void primeHelper(int p[2]) {
  int prime;
  int length;
  int readnum; 
  int newp[2];
  close(p[1]);   // No need to write to the left pipe.
  length = read(p[0], &prime, sizeof(int));
  if (length == 0) {
    close(p[0]);
	exit(0);
  }
  printf("prime %d\n", prime);
  // Create a new pipe, and pass the number to it
  if (pipe(newp) < 0) 
    exit(1);
  // read return 0 iff for all fds referring to the write end are closed
  if (fork() == 0) {
    close(p[0]);
   	primeHelper(newp);
  } else {
    close(newp[0]);
    // read number p, and pass it to the right neighbor. 
	while ((length = read(p[0], &readnum, sizeof(int))) != 0) {
	  if (readnum % prime != 0) {
		write(newp[1], &readnum, sizeof(int));
	  }
	}
	close(newp[1]);
	close(p[0]);
	wait(0);
  }
  exit(0);
}


