#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
 
int main(int argc, char * argv[]) {
  if (argc > 2){
	fprintf(2, "Error: too many arguments.\n");
	exit(1);
  } else if (argc <= 1) {
	fprintf(2, "Error: missing an argument.\n");
	exit(1);
  }
  // Here the argc is 1.
  int ticks = atoi(argv[1]);
  sleep(ticks);

  exit(0);
}
