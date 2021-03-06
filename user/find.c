#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{ 
  char *p;

  // Find first character after last slash.
  for (p = path + strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  return p;
}

void
find(char *path, char *file)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if ((fd = open(path, 0)) < 0) {
	fprintf(2, "find: cannot open current working dir\n");
	return;
  }

  if (fstat(fd, &st) < 0) {
	fprintf(2, "find: cannot stat current working dir\n");
	close(fd);
	return;
  }

  switch(st.type) {
	case T_FILE:
	  if (strcmp(file, fmtname(path)) == 0) {
		printf("%s\n", path);	
	  }
	  break;
	
	case T_DIR:
	  if (strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)) {
		printf("find: path too long\n");
		break;
	  }
	  strcpy(buf, path);
	  p = buf + strlen(buf);
	  *p++ = '/';
	  while (read(fd, &de, sizeof(de)) == sizeof(de)) {
		if (de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
		  continue;
		memmove(p, de.name, DIRSIZ);
		p[DIRSIZ] = 0;
		if (stat(buf, &st) < 0) {
		  printf("find: cannot stat %s\n", buf);
		  continue;
		}
		find(buf, file);
	  }
	  break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  if (argc < 3) {
	fprintf(2, "find: please input a specific name\n");
	exit(0);
  } else if (argc >= 4) {
	fprintf(2, "find: too much file name\n");
	exit(0);
  }
  find(argv[1], argv[2]);
  exit(0);
}
